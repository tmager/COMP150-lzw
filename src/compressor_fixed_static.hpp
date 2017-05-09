#include "compressor_fixed.hpp"

#include <stdint.h>

#include "dictionary.hpp"

class Compressor_Fixed_Static : public Compressor_Fixed {
public:
    Compressor_Fixed_Static(std::string infname,
                            std::string outfname,
                            std::string statsfname,
                            int _symwidth,
                            uint64_t _log_step = 1,
                            uint64_t _inbuf_size = 32 * 1024);
protected:
    Dictionary *_createDict();
};
