#include <stdint.h>
#include <vector>

#ifndef __DICTIONARYENTRY_HPP__
#define __DICTIONARYENTRY_HPP__

enum EntryType { DATA, EOF_MARKER, NOT_FOUND };

typedef struct {
    enum EntryType type;
    std::vector<uint8_t> entry;
} DictionaryEntry;

// Comparison operator -- note that entries of type EOF_MARKER and FLUSH_MARKER
// are equal regardless of contents of entry

bool operator==(const DictionaryEntry &e1,
                const DictionaryEntry &e2);

#endif /* __DICTIONARYENTRY_HPP__ */
