#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <cstdint>
#include <future>

#include "asset.hpp"

class AssetPackerException : public std::runtime_error
{
public:
  explicit AssetPackerException(const std::string &message) : std::runtime_error(message) {}
};

class AssetPacker
{
public:
  std::future<void> AddAsset(const std::string &assetPath);
  void PackAssets();

private:
  const uint64_t MAX_DATA_FILE_SIZE = 4 * 1024 * 1024 * 1024ULL;
  std::map<std::string, AssetInfo> assetMap;
  std::string currentDataFileName = "data_1.bin";
  std::ofstream currentDataFile{ currentDataFileName, std::ios::binary };
};
