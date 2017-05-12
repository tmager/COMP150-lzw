#include "dictionary_static.hpp"

Dictionary_Static::~Dictionary_Static() { }

TrieNode *Dictionary_Static::nextToReplace() {
    return nullptr;
}
TrieNode *Dictionary_Static::nextToReplaceUpdate() {
    return nullptr;
}

void Dictionary_Static::updateAccessed(TrieNode *n) {
    (void)n;
}
