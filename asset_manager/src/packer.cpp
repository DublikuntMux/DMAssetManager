#include <fstream>
#include <iostream>
#include <string>

#include "compress.hpp"
#include "packer.hpp"

#ifdef ENABLE_ENCODER
void AssetPacker::PackAssets()
{
  try {
    std::ofstream binaryFile("asset_map.bin", std::ios::binary);
    if (!binaryFile.is_open()) { throw AssetPackerException("Error: Unable to create binary asset map file."); }

    for (const auto &entry : assetMap) {

      size_t pathLength = entry.first.size();
      binaryFile.write(reinterpret_cast<const char *>(&pathLength), sizeof(pathLength));

      binaryFile.write(entry.first.c_str(), pathLength);

      binaryFile.write(reinterpret_cast<const char *>(&entry.second), sizeof(AssetInfo));
    }

    binaryFile.close();
  } catch (const std::exception &e) {
    throw AssetPackerException(e.what());
  }
}

void AssetPacker::AddAsset(const std::string &assetPath)
{
  std::ifstream assetFile(assetPath, std::ios::binary);
  if (!assetFile.is_open()) { throw AssetPackerException("Error: Unable to open asset file: " + assetPath); }

  assetFile.seekg(0, std::ios::end);
  std::streamsize assetSize = assetFile.tellg();
  assetFile.seekg(0, std::ios::beg);

  std::vector<char> assetData(assetSize);
  assetFile.read(assetData.data(), assetSize);
  assetFile.close();

  std::vector<char> compressData = compress(assetData);

  if (currentDataFile.tellp() + static_cast<std::streamsize>(compressData.size()) > MAX_DATA_FILE_SIZE) {
    currentDataFile.close();
    currentDataFileName = "data_" + std::to_string(assetMap.size() + 1) + ".bin";
    currentDataFile.open(currentDataFileName, std::ios::binary);

    if (!currentDataFile.is_open()) {
      throw AssetPackerException("Error: Unable to create data file: " + currentDataFileName);
    }
  }

  std::streampos offset = currentDataFile.tellp();
  currentDataFile.write(compressData.data(), compressData.size());

  assetMap[assetPath] = { currentDataFileName, offset, (compressData.size()) };
}
#endif
