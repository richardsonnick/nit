#include <gtest/gtest.h>
#include <filesystem>

#include <hash-object.h>
#include <MockFileSystemAdaptor.h>

TEST(ObjectHashTests, TestHashBlob) {
    const std::string current_dir = std::filesystem::path(__FILE__).parent_path().string();
    const std::string bee_movie_path = current_dir + "/assets/bee_movie.txt";
    FileSystemAdaptorImpl fs;
    nit::Blob blob = fs.getBlobFromFile(bee_movie_path);
    auto r = nit::hashObject(blob);
    EXPECT_EQ(r, "93ae3d6436613af8a6957db81e1701fbc50de7a8");
}

TEST(ObjectHashTests, TestHashToFSPath) {
    const std::filesystem::path objectStorePath("/Users/snoopy/code/doghouse/.nit/objects");
    const std::string hash = "da39a3ee5e6b4b0d3255bfef95601890afd80709";
    const std::filesystem::path expected = objectStorePath / hash.substr(0, 2) / hash.substr(2);
    const std::filesystem::path gotPath = nit::hashToFSPath(objectStorePath, hash);
    EXPECT_EQ(gotPath, expected);
}
