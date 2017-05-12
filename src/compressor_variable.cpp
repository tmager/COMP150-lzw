#include "compressor_variable.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "bitseq.hpp"
#include "dictionary.hpp"
#include "dictionary_static.hpp"
#include "utils.hpp"
#include "flags.hpp"

using std::vector;
using std::cerr;


Compressor_Variable::Compressor_Variable(std::string infname,
                                   std::string outfname,
                                   std::string statsfname,
                                   int _maxSymwidth,
                                   uint64_t _log_step,
                                   uint64_t _inbuf_size)
    : Compressor(infname, outfname, statsfname, _log_step, _inbuf_size)
    , symwidth(9)
    , maxSymwidth(_maxSymwidth)
{
    if (maxSymwidth > DICT_MAX_WIDTH) {
        throw std::runtime_error ("Dictionary symbols too wide");
    }
}

Compressor_Variable::~Compressor_Variable() { }


void Compressor_Variable::_compress(std::vector<uint8_t> &input,
                                 BitSeq &output,
                                 Dictionary *dict) {
    uint64_t code, oldCode;
    uint64_t currentSize;
    dict->resetLocal();
    oldCode = DICT_NOT_FOUND;
    currentSize = 0;

    for (uint64_t i = 0; i < input.size(); i++) {
        code = dict->getCodeLocal(input[i]);

        // If current is not in dictionary, output it
        if (code == DICT_NOT_FOUND) {
            code = dict->insertLocal(input[i], maxSymwidth);

            output.append(oldCode, symwidth);
#ifdef DEBUG_COMPRESS
            fprintf(stderr, "[IO] Appending %lx [width %d]\n",
                    code, symwidth);
#endif

            log(8 * currentSize, symwidth, dict->size(),
                (code == DICT_NOINSERT ? 0 : 8 * currentSize + 8));

            // See if we need to expand code width
            if (code != DICT_NOINSERT && (code >> symwidth) != 0) {
#ifdef DEBUG_COMPRESS
                cerr << "[CM] Expanding before " << std::hex << code << std::dec
                          << "(" << input[i-1] << ")" << std::endl;
#endif
                symwidth++;
            }

            // Reset position in dictionary to include character at end of
            // unrecognized sequence.
            dict->resetLocal();
            oldCode = dict->getCodeLocal(input[i]);
            currentSize = 1;
        } else {
            oldCode = code;
            currentSize++;
        }
    }

    // Flush remaining string in current
    output.append(oldCode, symwidth);
    // Force statistics write for last character of file
    log(8 * currentSize, symwidth, dict->size(), 0, true);

    output.append(dict->getEOF(), symwidth);
}

void Compressor_Variable::_extract(BitSeq &input,
                                       std::vector<uint8_t> &output,
                                       Dictionary *dict)     {
    bool hitEOF = false;
    bool inserting = true;
    uint64_t decodePos = 0;
    DictionaryEntry lastEntry = {NOT_FOUND, std::vector<uint8_t>()};

    while (!hitEOF) {
        uint64_t code = input.getBits(decodePos, symwidth);

#ifdef DEBUG_EXTRACT
        fprintf(stderr, "[EX] Got code %lx [position %lx : %lx]\n",
                code, decodePos, decodePos + symwidth);
#endif
        decodePos += symwidth;

        DictionaryEntry entry = dict->getSymbols(code, maxSymwidth);

        switch (entry.type) {
        case DATA:
            if (lastEntry.type == DATA) {
                code = dict->insertLocalLastSymbol(entry.entry.back(),
                                                   maxSymwidth);
                if (code == DICT_NOINSERT) {
                    inserting = false;
                } else if (inserting && (code + 1) >> symwidth != 0) {
#ifdef DEBUG_EXTRACT
                    cerr << "[EX] Expanding after "
                         << std::hex << code << std::dec
                         << "(" << entry.entry.front() << ")" << std::endl;
#endif
                    symwidth++;
                    if (symwidth > maxSymwidth) {
                        symwidth--;
                    }
                }
            }
            for (auto it = entry.entry.rbegin();
                   it != entry.entry.rend(); ++it) {
                output.push_back(*it);
            }
            lastEntry = entry;
            break;

        case EOF_MARKER:
            hitEOF = true;
            break;

        case NOT_FOUND:
            if (lastEntry.type != DATA) {
                throw std::runtime_error ("Unknown code " + std::to_string(code)
                                                + " following control symbol");
            }

            lastEntry.entry.insert(lastEntry.entry.begin(),
                                   lastEntry.entry.back());

            for (auto it = lastEntry.entry.rbegin();
                   it != lastEntry.entry.rend(); ++it) {
                output.push_back(*it);
            }

            if (inserting) {
                code = dict->insert(lastEntry.entry, maxSymwidth);
            }

            if (code == DICT_NOINSERT) {
                inserting = false;
            } else if (inserting && (code + 1) >> symwidth != 0) {
#ifdef DEBUG_EXTRACT
                cerr << "[EX] Expanding after " << std::hex << code << std::dec
                          << "(" << lastEntry.entry.front() << ")" << std::endl;
#endif
                symwidth++;
                if (symwidth > maxSymwidth) {
                    symwidth--;
                }
            }
            break;

        default:
            cerr << "Got unexpected dictionary entry type\n";
        }
    }
}
