#include <stdint.h>
#include <fstream>
#include <string>
#include <vector>

#include "bitseq.hpp"
#include "dictionary.hpp"

#ifndef __COMPRESSOR_HPP__
#define __COMPRESSOR_HPP__

class Compressor {
public:
    Compressor(std::string infname,
               std::string outfname,
               std::string statsfname,
               uint64_t _log_step = 1,
               uint64_t _inbuf_size = 32 * 1024);

    virtual ~Compressor();

    void compress();
    void extract();

protected:
    std::ifstream infs;
    std::ofstream outfs, statsfs;
    uint64_t inbuf_size;
    uint64_t log_step;

    // Statistics
    uint64_t s_step, s_in, s_out;

    void log(uint64_t d_in, uint64_t d_out,
             uint64_t dict_size, uint64_t entry_size, bool forceWrite = false);

    virtual Dictionary *_createDict() = 0;

    virtual void _compress(std::vector<uint8_t> &input, BitSeq &output,
                                            Dictionary *dict)    = 0;
    virtual void _extract(BitSeq &input, std::vector<uint8_t> &output,
                                            Dictionary *dict)    = 0;
};


#endif /* __COMPRESSOR_HPP__ */
