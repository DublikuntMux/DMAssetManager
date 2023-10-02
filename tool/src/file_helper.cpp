#include <filesystem>
#include <string>
#include <vector>

#include "file_helper.hpp"

std::vector<std::string> getFilesFromDirectory(const std::string &path)
{
  std::vector<std::string> files;
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (std::filesystem::is_regular_file(entry)) { files.push_back(entry.path().string()); }
  }
  return files;
}