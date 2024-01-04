#include <fstream>
#include <map>
#include <vector>

#include <brotli/decode.h>

#include "manager.hpp"

std::vector<char> decompress(const std::vector<char> &input)
{
  std::vector<char> output;

  BrotliDecoderState *state = BrotliDecoderCreateInstance(nullptr, nullptr, nullptr);

  size_t available_in = input.size();
  const auto *next_in = reinterpret_cast<const uint8_t *>(input.data());

  size_t estimated_output_size = input.size() * 10;
  output.resize(estimated_output_size);

  size_t available_out = output.size();
  auto *next_out = reinterpret_cast<uint8_t *>(output.data());

  BrotliDecoderResult result =
    BrotliDecoderDecompressStream(state, &available_in, &next_in, &available_out, &next_out, nullptr);

  if (result == BROTLI_DECODER_RESULT_SUCCESS) {
    output.resize(output.size() - available_out);
  } else {
    output.clear();
    throw AssetManagerException("Error: Brotli error.");
  }

  BrotliDecoderDestroyInstance(state);

  return output;
}

void AssetManager::ReadAssetMap(const std::string &filePath)
{
  std::ifstream binaryFile(filePath, std::ios::binary);
  if (!binaryFile.is_open()) { throw AssetManagerException("Error: Unable to open asset map file: " + filePath); }

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
    if (!dataFile.is_open()) { throw AssetManagerException("Error: Unable to open data file: " + info.dataFileName); }

    std::vector<char> assetData(info.length);
    dataFile.seekg(info.offset);
    dataFile.read(assetData.data(), info.length);
    dataFile.close();

    std::vector<char> decompressed = decompress(assetData);
    return decompressed;
  } else {
    throw AssetManagerException("Error: Asset not found: " + assetPath);
  }
}

std::map<std::string, AssetInfo> AssetManager::GetAssetMap() { return this->assetMap; }
