#include "compressor.hpp"

#include <stdint.h>

class Compressor_fixed_static : public Compressor {
public:
    Compressor_fixed_static(std::string infname,
                            std::string outfname,
                            std::string statsfname,
                            int _symwidth,
                            uint64_t _inbuf_size = 32 * 1024,
                            uint64_t _log_step = 1);
    ~Compressor_fixed_static();

protected:
    uint32_t symwidth;

    void _compress(std::vector<uint8_t> &input, BitSeq &output,
                   Dictionary_Compress &dict);
    void _extract(BitSeq &input, std::vector<uint8_t> &output,
                  Dictionary_Extract &dict);
};
