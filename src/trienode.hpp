#include <stdint.h>

#ifndef __TRIENODE_HPP__
#define __TRIENODE_HPP__

#define NCHILDREN (256)

class TrieNode {
public:
    TrieNode(uint8_t _sym, uint64_t _code,
             uint64_t _data, TrieNode *_parent);
    ~TrieNode();

    uint8_t getSymbol();
    uint64_t getCode();
    uint64_t getData();

    bool isLeaf();
    bool hasChild(uint8_t childsym);
    TrieNode *getChild(uint8_t childsym);
    TrieNode *getParent();

    void setCode(uint64_t _code);
    void setData(uint64_t _data);

    void addChild(TrieNode *child);
    void removeChild(uint8_t childsym);

private:
    uint8_t sym;
    uint64_t code, data;

    int nchildren;
    TrieNode *parent;
    TrieNode *children[NCHILDREN];
};


#endif /* __TRIENODE_HPP__ */
