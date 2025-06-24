#include <gtest/gtest.h>
#include <filesystem>

#include <hash-object.h>
#include <MockFileSystemAdaptor.h>
#include <ObjectStore.h>

using namespace nit;

TEST(ObjectHashTests, TestPutObject) {
    std::shared_ptr<FileSystemAdaptorInterface> mockFs = std::make_shared<nit::test::MockFileSystemAdaptor>();
    const std::filesystem::path objectStorePath("/Users/snoopy/code/doghouse/.nit/objects");
    auto objectStore = ObjectStore(objectStorePath, mockFs);

    const std::string hash = "2aae6c35c94fcfb415dbe95f408b9ce91ee846ed";
    const std::filesystem::path expectedObjectPath = objectStorePath / "2a" / "ae6c35c94fcfb415dbe95f408b9ce91ee846ed";

    const std::string data = "hello world";
    const std::vector<uint8_t> blob(data.begin(), data.end());
    auto gotPath = objectStore.putObject(blob);
    EXPECT_EQ(gotPath, expectedObjectPath);

    auto gotBlob = mockFs->fromPath(gotPath).blob;
    EXPECT_EQ(blob, gotBlob);
}

TEST(ObjectHashTests, TestGetObject) {
    std::shared_ptr<FileSystemAdaptorInterface> mockFs = std::make_shared<nit::test::MockFileSystemAdaptor>();
    const std::filesystem::path objectStorePath("/Users/snoopy/code/doghouse/.nit/objects");
    auto objectStore = ObjectStore(objectStorePath, mockFs);

    const std::string hash = "2aae6c35c94fcfb415dbe95f408b9ce91ee846ed";
    const std::filesystem::path objectPath = objectStorePath / "2a" / "ae6c35c94fcfb415dbe95f408b9ce91ee846ed";
    const std::string data = "hello world";
    const std::vector<uint8_t> blob(data.begin(), data.end());
    mockFs->writeBlobToFile(objectPath, blob);

    auto gotBlob = objectStore.getObject(hash);
    EXPECT_EQ(gotBlob, blob);
}