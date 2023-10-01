#pragma once
#include <map>
#include <stdexcept>
#include <vector>

#include "asset.hpp"

class AssetMnagerException : public std::runtime_error
{
public:
  AssetMnagerException(const std::string &message) : std::runtime_error(message) {}
};

class AssetManager
{
public:
  void ReadAssetMap(const std::string &filePath);
  std::vector<char> GetAsset(const std::string &assetPath);

private:
  std::map<std::string, AssetInfo> assetMap;
};
