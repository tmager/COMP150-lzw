#include <stdint.h>

#include <iostream>
#include <vector>
#include <unordered_map>

#include "dictionaryentry.hpp"

#ifndef __DICTIONARY_HPP__
#define __DICTIONARY_HPP__

#define DICTIONARY_MAX_WIDTH (63)
#define DICTIONARY_NOINSERT (ULONG_MAX)

class Dictionary {
public:
    // Dictionary();
    // ~Dictionary();

    virtual uint64_t size() = 0;

    virtual bool getEntry(uint64_t code, DictionaryEntry *entry) const = 0;
    virtual bool getCode(const DictionaryEntry &entry,
                                                uint64_t *code) const = 0;
    uint64_t keyWidth();

    uint64_t insert(const DictionaryEntry &entry, uint64_t w);
    // virtual void replaceEntry(uint64_t code, const DictionaryEntry &entry);
    // virtual void replaceCode(const DictionaryEntry &entry, uint64_t code);

    void clear();

    void initialize();

protected:
    uint64_t nextFree;
    virtual void rawInsert(const DictionaryEntry &entry, uint64_t code) = 0;
    virtual void rawClear() = 0;

private:

};

class Dictionary_Compress : public Dictionary {
public:
    uint64_t size();

    bool getEntry(uint64_t code, DictionaryEntry *entry) const;
    bool getCode(const DictionaryEntry &entry, uint64_t *code) const;

    // friend std::ostream &operator<<(std::ostream &os,
    //                                 const Dictionary_Compress &d);

private:
    std::unordered_map<DictionaryEntry, uint64_t, DE_hash> entries;

    void rawInsert(const DictionaryEntry &entry, uint64_t code);
    void rawClear();
};

class Dictionary_Extract : public Dictionary {
public:
    uint64_t size();

    bool getEntry(uint64_t code, DictionaryEntry *entry) const;
    bool getCode(const DictionaryEntry &entry, uint64_t *code) const;

    // friend std::ostream &operator<<(std::ostream &os,
    //                                 const Dictionary_Extract &d);

private:
    std::vector<DictionaryEntry> entries;

    void rawInsert(const DictionaryEntry &entry, uint64_t code);
    void rawClear();
};


#endif /* __DICTIONARY_HPP__ */
