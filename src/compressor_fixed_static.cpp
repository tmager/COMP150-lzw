#include "compressor_fixed_static.hpp"
#include "dictionary_static.hpp"

Compressor_Fixed_Static::Compressor_Fixed_Static(std::string infname,
                                                 std::string outfname,
                                                 std::string statsfname,
                                                 int _symwidth,
                                                 uint64_t _log_step,
                                                 uint64_t _inbuf_size)
    : Compressor_Fixed(infname, outfname, statsfname,
                       _symwidth, _log_step, _inbuf_size)
{ }

Dictionary *Compressor_Fixed_Static::_createDict() {
    return new Dictionary_Static;
}
