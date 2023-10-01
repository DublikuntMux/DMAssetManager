#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "manager.hpp"

void AssetManager::ReadAssetMap(const std::string &filePath)
{
  std::ifstream binaryFile(filePath, std::ios::binary);
  if (!binaryFile.is_open()) { throw AssetMnagerException("Error: Unable to open asset map file: " + filePath); }

  while (!binaryFile.eof()) {
    size_t pathLength;
    binaryFile.read(reinterpret_cast<char *>(&pathLength), sizeof(pathLength));

    std::string assetPath(pathLength, '\0');
    binaryFile.read(&assetPath[0], pathLength);

    AssetInfo info;
    binaryFile.read(reinterpret_cast<char *>(&info), sizeof(AssetInfo));

    assetMap[assetPath] = info;
  }

  binaryFile.close();
}

std::vector<char> AssetManager::GetAsset(const std::string &assetPath)
{
  if (assetMap.find(assetPath) != assetMap.end()) {
    const auto &info = assetMap.at(assetPath);

    std::ifstream dataFile(info.dataFileName, std::ios::binary);
    if (!dataFile.is_open()) { throw AssetMnagerException("Error: Unable to open data file: " + info.dataFileName); }

    std::vector<char> assetData(info.length);
    dataFile.seekg(info.offset);
    dataFile.read(assetData.data(), info.length);
    dataFile.close();

    return assetData;
  } else {
    throw AssetMnagerException("Error: Asset not found: " + assetPath);
  }
}
