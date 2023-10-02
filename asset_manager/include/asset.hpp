#pragma once

#include <cstddef>
#include <string>

struct AssetInfo
{
  std::string dataFileName;
  std::streampos offset;
  size_t length;
};
