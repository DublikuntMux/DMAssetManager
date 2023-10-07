# DM Aseet Manager

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)

## Info

DM Asset Manager is a high-performance C++ library for managing digital assets such as textures, sounds, and models. It enables you to read and manage assets from specific file paths, providing built-in error handling for issues like inaccessible file paths or missing assets.

## Usage

### CMake Configuration

To use this project, you need to enable specific features in CMake:

* To enable asset packing, set `DMASSET_ENABLE_ENCODER` in CMake.
* To enable asset loading, set `DMASSET_ENABLE_DECODER` in CMake.

### Packing Assets

To create assets, instantiate an `AssetPacker` object, add assets to it, and then pack them.

```c++
#include <packer.hpp>

AssetPacker assetPacker;
assetPacker.AddAsset("assets/my_texture.png");

assetPacker.PackAssets();
```

### Using Assets

Create an `AssetManager` object and utilize it to load assets.

```c++
#include <manager.hpp>

AssetManager manager;
manager.ReadAssetMap("asset_map.bin");
std::vector<char> assetData = manager.GetAsset("assets/my_texture.png");
```

## Contributions

Contributions to DM Asset Manager are welcome! To contribute:

1. Fork this repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them.
4. Push your changes to your fork.
5. Submit a pull request.
