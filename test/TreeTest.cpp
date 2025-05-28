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
    // Initial state
    EXPECT_EQ(root->getName(), "root");
    EXPECT_EQ(root->getMode(), DIRMODE);
    EXPECT_TRUE(root->getChildren().empty());

    // Add a file child
    auto initHash = root->getHash();
    Tree l2f1("L2F1", FILEMODE);
    root->addChild(l2f1);
    ASSERT_EQ(root->getChildren().size(), static_cast<size_t>(1));
    EXPECT_EQ(root->getChildren()[0], l2f1);
    EXPECT_NE(root->getHash(), initHash);

    // Add a directory child with its own children
    initHash = root->getHash();
    Tree l2d1("L2D1", DIRMODE);
    l2d1.addChild(Tree("l3f1", FILEMODE));
    l2d1.addChild(Tree("l3f2", FILEMODE));
    l2d1.addChild(Tree("l3f3", DIRMODE));
    root->addChild(l2d1);

    EXPECT_NE(root->getHash(), initHash);
    EXPECT_EQ(root->getChildren().size(), static_cast<size_t>(2));
    EXPECT_EQ(l2d1.getChildren().size(), static_cast<size_t>(3));

    // Check names and modes of children
    EXPECT_EQ(root->getChildren()[0].getName(), "L2F1");
    EXPECT_EQ(root->getChildren()[0].getMode(), FILEMODE);
    EXPECT_EQ(root->getChildren()[1].getName(), "L2D1");
    EXPECT_EQ(root->getChildren()[1].getMode(), DIRMODE);

    // Check grandchildren
    const auto& l2d1Children = root->getChildren()[1].getChildren();
    ASSERT_EQ(l2d1Children.size(), 3u);
    EXPECT_EQ(l2d1Children[0].getName(), "l3f1");
    EXPECT_EQ(l2d1Children[0].getMode(), FILEMODE);
    EXPECT_EQ(l2d1Children[2].getMode(), DIRMODE);

    // Test that hashes change when children are added
    auto hashBefore = root->getHash();
    root->addChild(Tree("L2F2", FILEMODE));
    EXPECT_NE(root->getHash(), hashBefore);


}

TEST_F(TreeTest, TestEquality) {
    Tree copy = *root;
    EXPECT_EQ(copy, *root);
}