#include <stdint.h>
#include <vector>
#include <iostream>

#include <boost/functional/hash.hpp>

#ifndef __DICTIONARYENTRY_HPP__
#define __DICTIONARYENTRY_HPP__

enum EntryType { DATA, EOF_MARKER, FLUSH_MARKER };

typedef struct {
    enum EntryType type;
    std::vector<uint8_t> entry;
} DictionaryEntry;

// Comparison operator -- note that entries of type EOF_MARKER and FLUSH_MARKER
// are equal regardless of contents of entry

bool operator==(const DictionaryEntry &e1,
                const DictionaryEntry &e2);

// Functor for hashing, needed for unordered_map

struct DE_hash {
    uint64_t operator() (const DictionaryEntry &entry) const {
        uint64_t seed = 0;
        boost::hash_combine(seed, entry.type);
        for (auto v : entry.entry) {
            boost::hash_combine(seed, v);
        }
        return seed;
   }
};

#endif /* __DICTIONARYENTRY_HPP__ */
