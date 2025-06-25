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