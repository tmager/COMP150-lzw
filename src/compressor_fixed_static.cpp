#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "compressor_fixed_static.hpp"
#include "bitseq.hpp"
#include "dictionary.hpp"
#include "utils.hpp"
#include "flags.hpp"

using std::vector;

#define COMPRESS_BUFFER_SIZE (64*1024) // 64kB
#define EXTRACT_BUFFER_SIZE (64*1024) // 64kB

Compressor_fixed_static::Compressor_fixed_static(std::string infname,
                                                 std::string outfname,
                                                 std::string statsfname,
                                                 int _symwidth,
                                                 uint64_t _inbuf_size,
                                                 uint64_t _log_step)
    : Compressor(infname, outfname, statsfname, _inbuf_size, _log_step)
    , symwidth(_symwidth)
{
    if (symwidth > DICTIONARY_MAX_WIDTH) {
        throw std::runtime_error ("Dictionary symbols too wide");
    }
}

Compressor_fixed_static::~Compressor_fixed_static() { }


void Compressor_fixed_static::_compress(std::vector<uint8_t> &input,
                                        BitSeq &output,
                                        Dictionary_Compress &dict) {
    uint64_t code, newCode;
    vector<uint8_t> current;

    for (uint64_t i = 0; i < input.size(); i++) {
        // std::cout << inbuf[i];
        current.push_back(input[i]);

        // If current is not in dictionary, output it
        if (!dict.getCode({DATA, current}, &code)) {
            newCode = dict.insert({DATA, current}, symwidth);

            current.pop_back();
            if (!dict.getCode({DATA, current}, &code)) {
                std::cerr << "Got a string with no predecessor in table: ["
                          << toString(current) << "] " << std::endl;
                abort();
            }
            output.append(code, symwidth);
#ifdef DEBUG_COMPRESS
            fprintf(stderr, "[IO] Appending %lx [width %d]\n",
                    code, symwidth);
#endif

            log(8 * current.size(), symwidth, dict.size(),
                (newCode == DICTIONARY_NOINSERT ? 0 : 8 * current.size() + 8));

            // Reset current to include character after end of
            // unrecognized sequence.
            current.clear();
            current.push_back(input[i]);
        }
    }

    // Flush remaining string in current
    if (!dict.getCode({DATA, current}, &code)) {
        throw std::runtime_error
                        ("Remaining characters have no predecessor in table");
    }
    output.append(code, symwidth);
    log(8 * current.size(), symwidth, dict.size(), 0);

    // Add EOF marker to output
    if (!dict.getCode({EOF_MARKER, vector<uint8_t>()}, &code)) {
        throw std::runtime_error ("Dictionary missing EOF entry");
    }
    output.append(code, symwidth);
}

void Compressor_fixed_static::_extract(BitSeq &input,
                                       std::vector<uint8_t> &output,
                                       Dictionary_Extract &dict)     {
    bool hitEOF = false;
    uint64_t decodePos = 0;
    DictionaryEntry lastEntry = {FLUSH_MARKER, std::vector<uint8_t>()};

    while (!hitEOF) {
        uint64_t code = input.getBits(decodePos, symwidth);

#ifdef DEBUG_EXTRACT
        fprintf(stderr, "[EX] Got code %lx [position %lx : %lx]\n",
                code, decodePos, decodePos + symwidth);
#endif
        decodePos += symwidth;

        DictionaryEntry entry;
        if (dict.getEntry(code, &entry)) { // Code is in dictionary
            switch (entry.type) {
            case DATA:
                if (lastEntry.type == DATA) {
                    lastEntry.entry.push_back(entry.entry.front());
                    dict.insert(lastEntry, symwidth);
                }
                for (auto v : entry.entry) {
                    output.push_back(v);
                }
                break;

            case EOF_MARKER:
                hitEOF = true;
                break;

            default:
                std::cerr << "Got unexpected dictionary entry type\n";
            }
            lastEntry = entry;
        } else { // Code not in dictionary
            if (lastEntry.type != DATA) {
                throw std::runtime_error ("Unknown code " + std::to_string(code)
                                                + " following control symbol");
            }
            // Add first character in previous entry to end
            lastEntry.entry.push_back(lastEntry.entry.front());
            for (auto v : lastEntry.entry) {
                output.push_back(v);
            }

            dict.insert(lastEntry, symwidth);
        }
    }
}
