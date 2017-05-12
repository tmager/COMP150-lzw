#include "compressor.hpp"

#include <stdint.h>

#include "dictionary.hpp"

#ifndef __COMPRESSOR_FIXED_HPP__
#define __COMPRESSOR_FIXED_HPP__

class Compressor_Fixed : public Compressor {
public:
    Compressor_Fixed(std::string infname,
                     std::string outfname,
                     std::string statsfname,
                     int _symwidth,
                     uint64_t _log_step = 1,
                     uint64_t _inbuf_size = 32 * 1024);
    ~Compressor_Fixed();

protected:
    uint32_t symwidth;

    void _compress(std::vector<uint8_t> &input, BitSeq &output,
                   Dictionary *dict);
    void _extract(BitSeq &input, std::vector<uint8_t> &output,
                  Dictionary *dict);
};


#endif /* __COMPRESSOR_FIXED_HPP__ */
