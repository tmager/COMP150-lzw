#include "utils.hpp"

using std::string;
using std::vector;

string toString(vector<uint8_t> vect) {
    string s;
    for (auto v : vect) {
        s += v;
    }
    return s;
}
