#include "compressor.hpp"

#include "dictionary.hpp"

#ifndef __COMPRESSOR_VARIABLE_HPP__
#define __COMPRESSOR_VARIABLE_HPP__

class Compressor_Variable : public Compressor {
public:
    Compressor_Variable(std::string infname,
                     std::string outfname,
                     std::string statsfname,
                     int _maxSymwidth,
                     uint64_t _log_step = 1,
                     uint64_t _inbuf_size = 32 * 1024);
    ~Compressor_Variable();

protected:
    uint32_t symwidth, maxSymwidth;

    void _compress(std::vector<uint8_t> &input, BitSeq &output,
                   Dictionary *dict);
    void _extract(BitSeq &input, std::vector<uint8_t> &output,
                  Dictionary *dict);
};


#endif /* __COMPRESSOR_VARIABLE_HPP__ */
