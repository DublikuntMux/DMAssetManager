#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <brotli/encode.h>

#include "packer.hpp"

std::vector<char> compress(const std::vector<char> &input)
{
  std::vector<char> output;
  size_t output_size = BrotliEncoderMaxCompressedSize(input.size());
  output.resize(output_size);

  BrotliEncoderState *state = BrotliEncoderCreateInstance(nullptr, nullptr, nullptr);
  BrotliEncoderSetParameter(state, BROTLI_PARAM_QUALITY, 10);

  size_t available_in = input.size();
  const auto *next_in = reinterpret_cast<const uint8_t *>(input.data());
  size_t available_out = output.size();
  auto *next_out = reinterpret_cast<uint8_t *>(output.data());

  BrotliEncoderCompressStream(
    state, BROTLI_OPERATION_PROCESS, &available_in, &next_in, &available_out, &next_out, nullptr);

  size_t remaining_out = available_out;
  BrotliEncoderCompressStream(
    state, BROTLI_OPERATION_FINISH, &available_in, &next_in, &remaining_out, &next_out, nullptr);

  output.resize(output.size() - remaining_out);
  BrotliEncoderDestroyInstance(state);

  return output;
}

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

std::future<void> AssetPacker::AddAsset(const std::string &assetPath)
{
  return std::async(std::launch::async, [this, assetPath] {
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
  });
}
