#include <gtest/gtest.h>

#include <manager.hpp>
#include <packer.hpp>

TEST(AssetPackerTest, PackAssetsTest)
{
  AssetPacker assetPacker;
  assetPacker.AddAsset("test_data/DublikuntMux.png");
  assetPacker.AddAsset("test_data/icon.jpg");
  assetPacker.AddAsset("test_data/pig.png");

  EXPECT_NO_THROW(assetPacker.PackAssets());
}

TEST(AssetManagerTest, ReadAssetMapTest)
{
  AssetManager assetManager;

  EXPECT_NO_THROW(assetManager.ReadAssetMap("asset_map.bin"));

  EXPECT_THROW(assetManager.ReadAssetMap("asset_not_map.bin"), AssetManagerException);
}

TEST(AssetManagerTest, GetAssetTest)
{
  AssetManager assetManager;
  assetManager.ReadAssetMap("asset_map.bin");

  std::ifstream assetFile("test_data/pig.png", std::ios::binary);
  assetFile.seekg(0, std::ios::end);
  std::streampos assetSize = assetFile.tellg();
  assetFile.seekg(0, std::ios::beg);

  std::vector<char> assetData(assetSize);

  assetFile.read(assetData.data(), assetSize);
  assetFile.close();

  EXPECT_EQ(assetManager.GetAsset("test_data/pig.png"), assetData);

  EXPECT_THROW(assetManager.GetAsset("test_data/coolebaka.ls"), AssetManagerException);
}
