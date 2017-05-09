#include "compressor_variable_static.hpp"
#include "dictionary_static.hpp"

Compressor_Variable_Static::Compressor_Variable_Static(std::string infname,
                                                 std::string outfname,
                                                 std::string statsfname,
                                                 int _maxSymwidth,
                                                 uint64_t _log_step,
                                                 uint64_t _inbuf_size)
    : Compressor_Variable(infname, outfname, statsfname,
                       _maxSymwidth, _log_step, _inbuf_size)
{ }

Dictionary *Compressor_Variable_Static::_createDict() {
    return new Dictionary_Static;
}
