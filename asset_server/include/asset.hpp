#pragma once

#include <string>

struct AssetInfo
{
  std::string dataFileName;
  std::streampos offset;
  std::streamsize length;
};
