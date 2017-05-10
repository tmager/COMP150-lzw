#include "utils.hpp"

using std::string;
using std::vector;

static string sanitizeChar(uint8_t c) {
    if (c == ' ') { return "_"; }
    if (isprint(c)) { return string(1,c); }

    switch (c) {
    case '\n':
        return "[\\n]";
    case '\r':
        return "[\\r]";
    case '\t':
        return "[\\t]";
    default:
        return "[" + std::to_string(c) + "]";
    }
}

string toString(vector<uint8_t> vect) {
    string s;
    for (auto v : vect) {
        s += sanitizeChar(v);
    }
    return s;
}


string toStringReverse(vector<uint8_t> vect) {
    string s;
    for (auto it = vect.rbegin(); it != vect.rend(); it++) {
        s += sanitizeChar(*it);
    }
    return s;
}
