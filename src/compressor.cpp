#include "compressor.hpp"

using std::string;
using std::ofstream;
using std::vector;

Compressor::Compressor(string infname, string outfname, string statsfname,
                                    uint64_t _log_step, uint64_t _inbuf_size)
    : infs   (infname,    std::ios::in  | std::ios::binary)
    , outfs  (outfname,   std::ios::out | std::ios::binary)
    , inbuf_size (_inbuf_size)
    , log_step (_log_step)
{
    if (!infs.is_open()) {
        std::cerr << "Unable to open file '" << infname << "' for reading.\n";
    }
    if (!outfs.is_open()) {
        std::cerr << "Unable to open file '" << outfname << "' for writing.\n";
    }

    if (statsfname != "") {
        statsfs = ofstream(statsfname, std::ios::out);

        if (!statsfs.is_open()) {
            std::cerr << "Unable to open file '"
                      << statsfname << "' for writing.\n";
        }

        s_step = 0;
        s_in   = 0;
        s_out  = 0;
    } else {
        log_step = 0;
    }
}

Compressor::~Compressor() {
    infs.close();
    outfs.close();
    if (statsfs.is_open()) {
        statsfs.close();
    }
}


void Compressor::compress() {
    infs.seekg(0, std::ios::end);
    uint64_t inLen = infs.tellg();
    infs.seekg(0, std::ios::beg);

    vector<uint8_t> input(inLen);
    infs.read(reinterpret_cast<char*>(&input[0]), inLen);

    BitSeq output;

    Dictionary *dict = _createDict();

    _compress(input, output, dict);

    output.write(outfs);
    // std::cout << *dict;
}

void Compressor::extract() {
    BitSeq input(infs);
    vector<uint8_t> output;

    Dictionary *dict = _createDict();

    _extract(input, output, dict);

    outfs.write(reinterpret_cast<const char*>(&output[0]), output.size());
    // std::cout << *dict;
}

void Compressor::log(uint64_t d_in, uint64_t d_out,
                     uint64_t dict_size, uint64_t entry_size, bool forceWrite) {
    if (log_step == 0) return;

    s_step++;
    s_in += d_in;
    s_out += d_out;

    if (s_step % log_step == 0 || forceWrite) {
        statsfs << s_step << ","
                << d_in   << ","
                << d_out  << ","
                << s_in   << ","
                << s_out  << ","
                << dict_size  << ","
                << entry_size << std::endl;
    }
}
