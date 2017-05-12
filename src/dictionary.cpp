#include "dictionary.hpp"

#include "utils.hpp"

Dictionary::Dictionary()
    : elts(0)
    , nextFree(0)
    , symDict(0,0,0,nullptr)
    , current(&symDict)
    , getSymLast(&symDict)
{
    initialize();
}

void Dictionary::initialize() {
    for (uint64_t i = 0; i < 0x100; i++) {
        resetLocal();
        insertLocal_raw(i, nextFree++);
    }

    eof_code = nextFree++;
    // Add a space for the EOF code in the code dictionary
    codeDict.push_back(nullptr);
    elts++;

    resetLocal();
}

uint64_t Dictionary::size() {
    return elts;
}

uint64_t Dictionary::getCodeLocal(uint8_t sym) {
    TrieNode *n = current->getChild(sym);
    if (n == nullptr) {
        return DICT_NOT_FOUND;
    } else {
        current = n;
        updateAccessed(n);
        return n->getCode();
    }
}

DictionaryEntry Dictionary::getSymbols(uint64_t code, uint64_t w) {
    if (code >= codeDict.size()
            || (nextFree >> w != 0 && nextToReplace() != nullptr
                && code == nextToReplace()->getCode())) {
        return { NOT_FOUND, std::vector<uint8_t>() };
    }

    TrieNode *n = codeDict.at(code);
    if (n == nullptr) {
        if (code == eof_code) {
            return { EOF_MARKER, std::vector<uint8_t>() };
        } else {
            return { NOT_FOUND, std::vector<uint8_t>() };
        }
    } else {
        getSymLast = current;
        current = n;
        std::vector<uint8_t> syms;
        while (n->getParent() != nullptr) {
            updateAccessed(n);
            syms.push_back(n->getSymbol());
            n = n->getParent();
        }
        return { DATA, syms };
    }
}

uint64_t Dictionary::getEOF() {
    return eof_code;
}

uint64_t Dictionary::insertLocal(uint8_t sym, uint64_t w) {
    uint64_t code;
    if ((nextFree >> w) == 0) {
        code = nextFree++;
    } else {
        TrieNode *toReplace = nextToReplaceUpdate();
        if (toReplace == nullptr) {
            return DICT_NOINSERT;
        }
        code = toReplace->getCode();
        removeLeaf(toReplace);
        elts--;
    }
    insertLocal_raw(sym, code);
    updateAccessed(current);
    return code;
}

uint64_t Dictionary::insertLocalLastSymbol(uint8_t sym, uint64_t w) {
    uint64_t code;
    TrieNode *_current = current;
    current = getSymLast;
    code = insertLocal(sym, w);
    current = _current;
    return code;
}

void Dictionary::resetLocal() {
    current = &symDict;
}

uint64_t Dictionary::insert(std::vector<uint8_t> syms, uint64_t w) {
    resetLocal();
    for (auto it = syms.rbegin(); it != --(syms.rend()); it++) {
        if (getCodeLocal(*it) == DICT_NOT_FOUND) {
            std::cerr << *this;
            throw std::runtime_error
                            ("Attempted to insert orphan symbol sequence");
        }
        updateAccessed(current);
    }
    return insertLocal(syms.front(), w);
}

void Dictionary::insertLocal_raw(uint8_t sym, uint64_t code) {
    TrieNode *n = new TrieNode(sym, code, 0, current);
    current->addChild(n);
    current = n;
    if (code == codeDict.size()) {
        codeDict.push_back(n);
    } else {
        codeDict.at(code) = n;
    }
    elts++;
#ifdef DEBUG_DICT
    TrieNode *m = n;
    std::vector<uint8_t> cs;
    while (m->getParent() != nullptr) {
        cs.push_back(m->getSymbol());
        m = m->getParent();
    }
    std::cout << "Inserting leaf " << toStringReverse(cs) << std::endl;
#endif
}

void Dictionary::removeLeaf(TrieNode *n) {
#ifdef DEBUG_DICT
    TrieNode *m = n;
    std::vector<uint8_t> cs;
    while (m->getParent() != nullptr) {
        cs.push_back(m->getSymbol());
        m = m->getParent();
    }
    std::cout << "Removing leaf " << toStringReverse(cs) << std::endl;
#endif
    if (current == n) {
        std::cerr << "Current at leaf, moving\n";
        current = n->getParent();
    }
    if (getSymLast == n) {
        std::cerr << "gSL at leaf, moving\n";
        getSymLast = n->getParent();
    }
    codeDict.at(n->getCode()) = nullptr;
    n->getParent()->removeChild(n->getSymbol());
}


std::ostream &operator<<(std::ostream &os, Dictionary &d) {
    TrieNode *_current = d.current;
    TrieNode *_gSM = d.getSymLast;
    for (uint64_t i = 0; i < d.codeDict.size(); i++) {
        if (d.codeDict.at(i) == nullptr) {
            if (i == d.eof_code) {
                os << std::hex << i << std::dec << " : [EOF]\n";
            }
        } else {
            os << std::hex << i << std::dec <<
                " : " << toStringReverse(d.getSymbols(i,63).entry) << std::endl;
        }
    }
    d.current = _current;
    d.getSymLast = _gSM;
    return os;
}
