#include <stdint.h>

#include <vector>
#include <iostream>

#ifndef __BITSEQ_H__
#define __BITSEQ_H__

typedef uint64_t BitSeq_Block;

class BitSeq {
public:
    BitSeq();
    BitSeq(std::istream &is);

    uint64_t size() const;

    uint64_t getBits(uint64_t start, uint64_t len) const;

    void append(uint64_t bits, uint64_t wid);

    void clear();

    void write(std::ostream &os) const;
    void write(std::ostream &os, uint64_t start, uint64_t len) const;

    friend std::ostream &operator<<(std::ostream &os, const BitSeq &bs);


private:
    std::vector<BitSeq_Block> data;
    uint64_t length;
};

#endif /* __BITSEQ_H__ */
