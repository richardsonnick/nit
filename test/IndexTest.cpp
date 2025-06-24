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
    }

    void TearDown() override {
    }
};

TEST_F(IndexTest, TestAddTrees) {
    EXPECT_TRUE(index->getTrees().empty());
    index->addTrees();
    EXPECT_EQ(index->getTrees().size(), (unsigned long)3);
}