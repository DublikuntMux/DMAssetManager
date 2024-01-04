#pragma once

#include <map>
#include <stdexcept>
#include <vector>

#include "asset.hpp"

class AssetManagerException : public std::runtime_error
{
public:
  explicit AssetManagerException(const std::string &message) : std::runtime_error(message) {}
};

class AssetManager
{
public:
  void ReadAssetMap(const std::string &filePath);
  std::vector<char> GetAsset(const std::string &assetPath);

  std::map<std::string, AssetInfo> GetAssetMap();

private:
  std::map<std::string, AssetInfo> assetMap;
};
