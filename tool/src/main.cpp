#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <manager.hpp>
#include <packer.hpp>

#include "color.hpp"
#include "file_helper.hpp"

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << YELLOW << "Dublikunt Max Asset Packer tool" << RESET << std::endl;
    std::cerr << "Usage: " << argv[0] << " <mode> [arguments]" << std::endl;
    std::cerr << "Mode can be: " << std::endl;
    std::cerr << "p - create assets file." << std::endl;
    std::cerr << "u - unpack assets." << std::endl;
    return 1;
  }

  char mode = argv[1][0];
  if ((mode == 'p' || mode == 'u') && argc > 2) {
    if (mode == 'p') {
      std::vector<std::string> files;
      for (int i = 2; i < argc; ++i) {
        std::string path(argv[i]);
        if (std::filesystem::is_directory(path)) {
          std::vector<std::string> dirFiles = getFilesFromDirectory(path);
          files.insert(files.end(), dirFiles.begin(), dirFiles.end());
        } else if (std::filesystem::is_regular_file(path)) {
          files.push_back(path);
        }
      }
      AssetPacker assetPacker;
      std::cout << GREEN << "Pack mode: Processing files..." << RESET << std::endl;
      std::vector<std::future<void>> futures;

      for (const auto &file : files) {
        std::cout << "Processing file: " << file << std::endl;
        futures.push_back(assetPacker.AddAsset(file));
      }
      for(auto &fut : futures) {
        fut.get();
      }
      assetPacker.PackAssets();
    } else if (mode == 'u' && argc == 3) {
      std::string filePath(argv[2]);
      if (std::filesystem::is_regular_file(filePath)) {
        AssetManager assetManager;
        std::cout << GREEN << "Unpack mode: Processing file: " << filePath << RESET << std::endl;
        assetManager.ReadAssetMap(filePath);

        for (const auto &pair : assetManager.GetAssetMap()) {
          const std::string &assetPath = pair.first;
          const AssetInfo &assetInfo = pair.second;

          std::ifstream dataFile(assetInfo.dataFileName, std::ios::binary);
          if (!dataFile) { throw AssetManagerException("Failed to open asset data file: " + assetInfo.dataFileName); }

          dataFile.seekg(assetInfo.offset);

          std::vector<char> data(assetInfo.length);
          dataFile.read(data.data(), assetInfo.length);

          std::ofstream outputFile("./" + assetPath, std::ios::binary);
          if (!outputFile) { throw AssetManagerException("Failed to open output file: ./" + assetPath); }
          outputFile.write(data.data(), data.size());
        }
      } else {
        std::cerr << RED << "Error: Invalid file path." << RESET << std::endl;
        return 1;
      }
    }
  } else {
    std::cerr << RED << "Error: Invalid mode or arguments." << RESET << std::endl;
    return 1;
  }

  return 0;
}
