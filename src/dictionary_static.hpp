#include "dictionary.hpp"

#ifndef __DICTIONARY_STATIC_HPP__
#define __DICTIONARY_STATIC_HPP__

class Dictionary_Static : public Dictionary {
public:
    ~Dictionary_Static();
private:
    TrieNode *nextToReplace();
    TrieNode *nextToReplaceUpdate();
    void updateAccessed(TrieNode *n);
};

#endif /* __DICTIONARY_STATIC_HPP__ */
