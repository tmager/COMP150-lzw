#include "trienode.hpp"

TrieNode::TrieNode(uint8_t _sym, uint64_t _code,
                   uint64_t _data, TrieNode *_parent)
    : sym(_sym), code(_code), data(_data), parent(_parent)
{
    nchildren = 0;
    for (int i = 0; i < NCHILDREN; i++) {
        children[i] = nullptr;
    }
}

TrieNode::~TrieNode() {
    for (int i = 0; i < NCHILDREN; i++) {
        if (children[i] != nullptr) {
            delete children[i];
        }
    }
}

uint8_t TrieNode::getSymbol() {
    return sym;
}

uint64_t TrieNode::getCode() {
    return code;
}

uint64_t TrieNode::getData() {
    return data;
}

bool TrieNode::isLeaf() {
    return nchildren == 0;
}

bool TrieNode::hasChild(uint8_t childsym) {
    return children[childsym] != nullptr;
}

TrieNode *TrieNode::getChild(uint8_t childsym) {
    return children[childsym];
}

TrieNode *TrieNode::getParent() {
    return parent;
}

void TrieNode::setCode(uint64_t _code) {
    code = _code;
}

void TrieNode::setData(uint64_t _data) {
    data = _data;
}

void TrieNode::addChild(TrieNode *child) {
    children[child->getSymbol()] = child;
}

void TrieNode::removeChild(uint8_t childsym) {
    delete children[childsym];
    children[childsym] = nullptr;
}
