#pragma once

#include <vector>

#ifdef ENABLE_ENCODER
std::vector<char> compress(const std::vector<char> &input);
#endif

#ifdef ENABLE_DECODER
std::vector<char> decompress(const std::vector<char> &input);
#endif