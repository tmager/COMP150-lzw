#include "dictionary.hpp"

#include "utils.hpp"
#include "flags.hpp"

using std::cerr;
using std::endl;
using std::ios;

void Dictionary::initialize() {
    nextFree = 0;
    for (int i = 0; i < 0x100; i++) {
        insert({DATA, std::vector<uint8_t>(1,i)}, DICTIONARY_MAX_WIDTH);
    }
    insert({EOF_MARKER,   std::vector<uint8_t>()}, DICTIONARY_MAX_WIDTH);
    insert({FLUSH_MARKER, std::vector<uint8_t>()}, DICTIONARY_MAX_WIDTH);
}

uint64_t Dictionary::keyWidth() {
    if (nextFree == 0) return 0;

    uint64_t i = 0, j = nextFree - 1;
    while (j > 0) {
        j >>= 1;
        i++;
    }
    return i;
}

uint64_t Dictionary::insert(const DictionaryEntry &entry, uint64_t w) {
    if (nextFree >> w != 0) {
#ifdef DEBUG_DICT
        cerr << "[DICT] Failed to insert [" << toString(entry.entry)
             << "], code too wide (would be " << nextFree << ")\n";
#endif
        return DICTIONARY_NOINSERT;
    }

    rawInsert(entry, nextFree);
    return nextFree++;
}

void Dictionary::clear() {
    rawClear();
    initialize();
}





uint64_t Dictionary_Compress::size() {
    return entries.size();
}

uint64_t Dictionary_Extract::size() {
    return entries.size();
}


bool Dictionary_Compress::getEntry(uint64_t code,
                                   DictionaryEntry *entry) const {
    (void)code;
    (void)entry;

    cerr << "[ERR] Unimplemented: Dictionary_Compress::getEntry "
         << __FILE__ << " " << __LINE__ << endl;
    return false;
}

bool Dictionary_Extract::getEntry(uint64_t code,
                                   DictionaryEntry *entry) const {
    if (code >= nextFree) {
        return false;
    }

    *entry = entries.at(code);
    return true;
}

bool Dictionary_Compress::getCode(const DictionaryEntry &entry,
                                 uint64_t *code) const {
    auto k = entries.find(entry);
    if (k != entries.cend()) {
        *code = k->second;
        return true;
    } else {
        return false;
    }
}

bool Dictionary_Extract::getCode(const DictionaryEntry &entry,
                                 uint64_t *code) const {
    (void)code;
    (void)entry;

    cerr << "[ERR] Unimplemented: Dictionary_Compress::getEntry "
         << __FILE__ << " " << __LINE__ << endl;
    return false;
}

void Dictionary_Compress::rawInsert(const DictionaryEntry &entry,
                                    uint64_t code) {
#ifdef DEBUG_DICT
        cerr << "[DICT] Inserting [" << toString(entry.entry)
             << "] : " << code << endl;
#endif

    entries.insert({entry, code});
}


void Dictionary_Extract::rawInsert(const DictionaryEntry &entry,
                                   uint64_t code) {
#ifdef DEBUG_DICT
        cerr << "[DICT] Inserting " << code
             << " : [" << toString(entry.entry) << "]"  << endl;
#endif
    if (code != entries.size()) {
        cerr << "Attempted to insert non-contiguous key in dictionary.\n";
        abort();
    }

    entries.push_back(entry);
}

void Dictionary_Compress::rawClear() {
    entries.clear();
}

void Dictionary_Extract::rawClear() {
    entries.clear();
}
