#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string.h>

#include "compressor.hpp"
#include "compressor_fixed_static.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

enum MODE {COMPRESS, EXTRACT};

void usage(char *exec) {
    cerr << "Usage: " << exec
         << " [-c] [-x] [-m MODE] [-w WIDTH]"
         << " [-s STATS] [-i STATS_FREQ] [-o DEST] SRC\n";
}

int main(int argc, char **argv) {

    enum MODE mode = COMPRESS;
    string infname = "";
    string outfname = "";
    string compressor_name = "fs";
    string statsfname = "";
    uint64_t stats_freq = 1;
    uint64_t width = 12;

    int i;
    for (i = 1; i < argc - 1; i++) {
        if (string(argv[i]) == "-c") {
            mode = COMPRESS;
        } else if (string(argv[i]) == "-x") {
            mode = EXTRACT;
        } else if (string(argv[i]) == "-o") {
            outfname = argv[++i];
        } else if (string(argv[i]) == "-m") {
            compressor_name = argv[++i];
        } else if (string(argv[i]) == "-w") {
            std::istringstream(argv[++i]) >> width;
        } else if (string(argv[i]) == "-s") {
            statsfname = argv[++i];
        } else if (string(argv[i]) == "-i") {
            std::istringstream(argv[++i]) >> stats_freq;
        }
    }

    if (i == argc) {
        usage(argv[0]);
        return -1;
    }

    infname = argv[i];

    if (outfname == "") {
        switch (mode) {
        case COMPRESS:
            outfname = infname + "." + compressor_name;
            break;

        case EXTRACT:
            if (infname.substr(infname.find_last_of(".") + 1, string::npos)
                                                           == compressor_name) {
                outfname = infname.substr(0,infname.find_last_of("."));
            } else {
                outfname = infname + ".extr_" + compressor_name;
            }
            break;
        }
    }



    Compressor *C;

    if (compressor_name == "fs") {
        C = new Compressor_fixed_static(infname, outfname, statsfname,
                                                            width, stats_freq);
    } else {
        cerr << "Allowed compressors are: fs\n";
        abort();
    }

    if (mode == COMPRESS) {
        C->compress();
    } else {
        C->extract();
    }

    delete C;

    return 0;
}
