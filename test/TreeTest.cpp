#include <gtest/gtest.h>
#include <Tree.h>

using namespace nit;

class TreeTest : public ::testing::Test {
protected:
    Tree* root;

    void SetUp() override {
        root = new Tree("root", DIRMODE);
    }

    void TearDown() override {
        delete root;
    }
};

TEST_F(TreeTest, TestAddChild) {
    auto initHash = root->getHash();
    auto l2f1 = Tree("L2F1", FILEMODE);
    root->addChild(l2f1);
    ASSERT_EQ(root->getChildren().size(), static_cast<size_t>(1));
    EXPECT_EQ(root->getChildren()[0], l2f1);
    EXPECT_NE(root->getHash(), initHash);

    initHash = root->getHash();
    auto l2d1 = Tree("L2D1", DIRMODE);
    l2d1.addChild(Tree("l3f1", FILEMODE));
    l2d1.addChild(Tree("l3f2", FILEMODE));
    l2d1.addChild(Tree("l3f3", DIRMODE));
    root->addChild(l2d1);
    EXPECT_NE(root->getHash(), initHash);
    EXPECT_EQ(root->getChildren().size(), static_cast<size_t>(2));
    EXPECT_EQ(l2d1.getChildren().size(), static_cast<size_t>(3));
}