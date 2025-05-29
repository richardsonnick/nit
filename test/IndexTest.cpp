#include <gtest/gtest.h>
#include <Index.h>
#include <MockFileSystemAdaptor.h>

using namespace nit;

class IndexTest : public ::testing::Test {
protected:
    Index* index;

    void SetUp() override {
        std::shared_ptr<FileSystemAdaptorInterface> fsa = std::make_shared<MockFileSystemAdaptor>();
        index = new Index(fsa, "/home/snoopy/code/doghouse");
    }

    void TearDown() override {
        delete index;
    }
};

TEST_F(IndexTest, TestSanity) {
    FAIL();
}