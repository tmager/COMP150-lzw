#include <stdint.h>
#include <limits.h>
#include <iostream>
#include <vector>

#include "trienode.hpp"
#include "dictionaryentry.hpp"

#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#define DICT_MAX_WIDTH (63)
#define DICT_NOINSERT (ULONG_MAX)
#define DICT_NOT_FOUND (ULONG_MAX - 1)

class Dictionary {
public:
    Dictionary();
    void initialize();

    virtual ~Dictionary() { };

    uint64_t size();

    uint64_t getCodeLocal(uint8_t sym);
    DictionaryEntry getSymbols(uint64_t code, uint64_t w);
    uint64_t getEOF();
    uint64_t insertLocal(uint8_t sym, uint64_t w);
    uint64_t insertLocalLastSymbol(uint8_t sym, uint64_t w);
    void resetLocal();

    uint64_t insert(std::vector<uint8_t> syms, uint64_t w);

    friend std::ostream &operator<<(std::ostream &os, Dictionary &d);

protected:
    uint64_t elts;
    uint64_t nextFree;

    uint64_t eof_code;
    TrieNode symDict;
    std::vector<TrieNode *> codeDict;

    TrieNode *current, *getSymLast;

    void insertLocal_raw(uint8_t sym, uint64_t code);

    void removeLeaf(TrieNode *n);

    /* Defines the code replacement policy for the dictionary. Must return a
     * pointer to the leaf node that will be removed by the next replace
     * operation, or nullptr to indicate that no code should be replaced and the
     * insertion should be ignored. Passed the root node initially. */
    virtual TrieNode *nextToReplace() = 0;
    virtual TrieNode *nextToReplaceUpdate() = 0;

    /* Helper to define replacement policy. Called on each node every time it is
     * accessed. */
    virtual void updateAccessed(TrieNode *n) = 0;
};

#endif /* __DICTIONARY_H__ */
