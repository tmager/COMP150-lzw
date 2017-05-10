#include "bitseq.hpp"
#include <string>

#define BLOCKSIZE (8 * sizeof(BitSeq_Block))

static inline uint64_t extract(uint64_t n, uint64_t lsb, uint64_t w) {
    return (n << (8*sizeof(n) - w - lsb)) >> (8*sizeof(n) - w);
}

BitSeq::BitSeq() {
    length = 0;
}

BitSeq::BitSeq(std::istream &is) {
    is.seekg(0, is.end);
    length = 8 * is.tellg();
    is.seekg(0, is.beg);

    if (length % BLOCKSIZE != 0) {
        throw std::runtime_error
                            ("Input file length not a multiple of block size");
    }

    BitSeq_Block *databuf = new BitSeq_Block[length / BLOCKSIZE];
    is.read((char *)databuf, length / 8);

    data = std::vector<BitSeq_Block>(databuf,
                                     databuf + length / BLOCKSIZE);

    delete[] databuf;
}

uint64_t BitSeq::size() const {
    return length;
}

uint64_t BitSeq::getBits(uint64_t start, uint64_t len) const {
    if (len > 64) {
        throw std::range_error ("Attempted to extract more "
                                "bits than fit in return type");
    } else if (start + len > length) {
        std::cout << start << " " << len << std::endl;
        throw std::out_of_range ("Bit index out of range");
    }

    if (len == 0) { return 0; }

    uint64_t offset = BLOCKSIZE - start % BLOCKSIZE;
    uint64_t ret = 0;
    if (offset >= len) {
        ret = extract(data[start / BLOCKSIZE], offset - len, len);
    } else {
        ret  = extract(data[start / BLOCKSIZE], 0, offset) << (len - offset);
        ret |= extract(data[start / BLOCKSIZE + 1],
                       BLOCKSIZE - len + offset, len - offset);
    }

    return ret;
}

void BitSeq::append(uint64_t bits, uint64_t wid) {
    if (bits >> wid != 0 && wid != 64) {
        std::cerr << "[*W*] bits wider than wid, ignoring high-order bits...\n";
        bits = extract(bits, 0, wid);
    }

    uint64_t offset = (BLOCKSIZE - length % BLOCKSIZE) % BLOCKSIZE;

    if (offset == 0) {
        data.push_back(bits << (BLOCKSIZE - wid));
    } else if (offset >= wid) {
        data[length / BLOCKSIZE] |= bits << (offset - wid);
    } else {
        data[length / BLOCKSIZE] |= extract(bits, wid - offset, offset);
        data.push_back(extract(bits, 0, wid - offset)
                                                << (BLOCKSIZE - wid + offset));
    }

    length += wid;
}


void BitSeq::clear() {
    data.clear();
    length = 0;
}


void BitSeq::write(std::ostream &os) const {
    for (BitSeq_Block d : data) {
        os.write((char *)&d, sizeof(d));
    }
}

void BitSeq::write(std::ostream &os, uint64_t start, uint64_t len) const {
    if (start % BLOCKSIZE != 0 || len % BLOCKSIZE != 0) {
        throw std::runtime_error ("Attempted misaligned write");
    }

    for (uint64_t i = 0; i < len / BLOCKSIZE; i++) {
        BitSeq_Block d = data[start + i];
        os.write((char *)&d, sizeof(d));
    }
}


std::ostream &operator<<(std::ostream &os, const BitSeq &bs) {
    for (uint64_t idx = 0; idx < bs.data.size() - 1; idx++) {
        for (int offset = BLOCKSIZE - 1; offset >= 0; offset--) {
            os << (extract(bs.data[idx], offset, 1) == 0 ? '0' : '1');
        }
        os << " ";
    }

    if (bs.length % BLOCKSIZE != 0) {
        BitSeq_Block b = bs.data.at(bs.length / BLOCKSIZE);
        for (uint64_t offset = BLOCKSIZE - 1;
                offset >= BLOCKSIZE - bs.length % BLOCKSIZE; offset--) {
            os << (extract(b, offset, 1) == 0 ? '0' : '1');
        }
    }

    return os;
}
