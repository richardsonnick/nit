#include <gtest/gtest.h>
#include <Index.h>
#include <MockFileSystemAdaptor.h>
#include <TestUtils.h>

using namespace nit;
namespace fs = std::filesystem;

class IndexTest : public ::testing::Test {
protected:
    std::unique_ptr<Index> index;
    std::shared_ptr<nit::test::MockFileSystemAdaptor> fsa; // Use MockFileSystemAdaptor directly

    void SetUp() override {
        fsa = std::make_shared<nit::test::MockFileSystemAdaptor>();
        index = std::make_unique<Index>(fsa, "/home/snoopy/code/doghouse");
        nit::test::createInitialRepo(fsa);
        auto baseRepoDir = std::get<FileSystemAdaptorInterface::Directory>(fsa->fsMap["/home/snoopy/code/doghouse"].first);
        ASSERT_EQ(baseRepoDir.size(), (size_t) 3);
    }

    void TearDown() override {
    }
};

TEST_F(IndexTest, TestAddTrees) {
    EXPECT_TRUE(index->getTrees().empty());
    index->addTrees();
    auto trees = index->getTrees();
    EXPECT_EQ(trees.size(), (unsigned long)3);
    auto entries = index->getEntries();
    EXPECT_EQ(entries.size(), (unsigned long)4); // Entries should reflect number of ~Files~ not ~Directories~ (this is contained in Trees)
}

TEST_F(IndexTest, TestSerializeDeserialize) {
    index->addTrees();
    std::vector<uint8_t> serialization = index->serialize();

    Index gotIndex = Index::deserialize(serialization);
    ASSERT_EQ(index->getEntries().size(), gotIndex.getEntries().size());
    ASSERT_TRUE(index->getRootTree() != nullptr);
    ASSERT_TRUE(gotIndex.getRootTree() != nullptr);
    EXPECT_EQ(index->getRootTree()->getHash(), gotIndex.getRootTree()->getHash());

    auto entries = index->getEntries();
    auto gotEntries = gotIndex.getEntries();
    auto compareEntry = [&](int i) {
        EXPECT_EQ(gotEntries[i].hash, entries[i].hash);
        EXPECT_EQ(gotEntries[i].metadata.ctime, entries[i].metadata.ctime);
        EXPECT_EQ(gotEntries[i].metadata.mtime, entries[i].metadata.mtime);
        EXPECT_EQ(gotEntries[i].metadata.deviceID, entries[i].metadata.deviceID);
        EXPECT_EQ(gotEntries[i].metadata.inode, entries[i].metadata.inode);
        EXPECT_EQ(gotEntries[i].metadata.fileMode, entries[i].metadata.fileMode);
        EXPECT_EQ(gotEntries[i].metadata.userId, entries[i].metadata.userId);
        EXPECT_EQ(gotEntries[i].metadata.groupId, entries[i].metadata.groupId);
        EXPECT_EQ(gotEntries[i].metadata.fileSize, entries[i].metadata.fileSize);
        EXPECT_EQ(gotEntries[i].metadata.flags, entries[i].metadata.flags);
        EXPECT_EQ(gotEntries[i].metadata.pathName, entries[i].metadata.pathName);
    };
    for (size_t i = 0; i < entries.size(); i++) {
        compareEntry(i);
    }
}