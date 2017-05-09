#include "compressor_variable.hpp"

#include <stdint.h>

#include "dictionary.hpp"

class Compressor_Variable_Static : public Compressor_Variable {
public:
    Compressor_Variable_Static(std::string infname,
                               std::string outfname,
                               std::string statsfname,
                               int _maxSymwidth,
                               uint64_t _log_step = 1,
                               uint64_t _inbuf_size = 32 * 1024);
protected:
    Dictionary *_createDict();
};
