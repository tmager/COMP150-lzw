#include "dictionaryentry.hpp"

bool operator==(const DictionaryEntry &e1,
                const DictionaryEntry &e2) {
    return e1.type == e2.type && (e1.type != DATA || e1.entry == e2.entry);
}
