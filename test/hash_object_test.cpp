#include <gtest/gtest.h>
#include <hash-object.h>
#include <filesystem>

TEST(SHA1Tests, HashFile) {
    const std::string current_dir = std::filesystem::path(__FILE__).parent_path().string();
    const std::string bee_movie_path = current_dir + "/assets/bee_movie.txt";
    FileSystemAdaptorImpl fs;
    nit::Blob blob = fs.getBlobFromFile(bee_movie_path);
    auto r = nit::hashBlob(blob);
    EXPECT_EQ(r, "93ae3d6436613af8a6957db81e1701fbc50de7a8");
}