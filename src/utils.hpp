#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "flags.hpp"

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

std::string toString(std::vector<uint8_t> vect);
std::string toStringReverse(std::vector<uint8_t> vect);

#endif /* __UTILS_HPP__ */
