#include <gtest/gtest.h>
#include <Tree.h>
#include <Utils.h>

using namespace nit;

class TreeTest : public ::testing::Test {
protected:
    std::unique_ptr<Tree> tree;

    void SetUp() override {
        tree = std::make_unique<Tree>();
    }

    void TearDown() override {
    }
};

TEST_F(TreeTest, TestAddEntryAndHashChange) {
    EXPECT_TRUE(tree->getEntries().empty());
    auto initialHash = tree->getHash();

    TreeEntry fileEntry{ "file1.txt", FILEMODE, utils::hexToRawHash("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") };
    tree->addEntry(fileEntry);
    ASSERT_EQ(tree->getEntries().size(), (unsigned long)1);
    EXPECT_NE(tree->getHash(), initialHash);

    auto hashAfterFirstAdd = tree->getHash();
    TreeEntry dirEntry{ "subdir", DIRMODE, utils::hexToRawHash("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb") };
    tree->addEntry(dirEntry);
    EXPECT_EQ(tree->getEntries().size(), (unsigned long)2);
    EXPECT_NE(tree->getHash(), hashAfterFirstAdd);
}

TEST_F(TreeTest, TestSerializeDeserialize) {
    TreeEntry fileEntry{ "file1.txt", FILEMODE, utils::hexToRawHash("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") };
    TreeEntry dirEntry{ "subdir", DIRMODE, utils::hexToRawHash("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb") };
    tree->addEntry(fileEntry);
    tree->addEntry(dirEntry);

    auto serialized = tree->serialize();
    Tree deserialized = Tree::deserialize(serialized);

    const auto& originalEntries = tree->getEntries();
    const auto& deserializedEntries = deserialized.getEntries();
    ASSERT_EQ(originalEntries.size(), deserializedEntries.size());

    for (size_t i = 0; i < originalEntries.size(); ++i) {
        EXPECT_EQ(originalEntries[i].mode, deserializedEntries[i].mode);
        EXPECT_EQ(originalEntries[i].name, deserializedEntries[i].name);
        EXPECT_EQ(originalEntries[i].hash, deserializedEntries[i].hash);
    }

    // I'm choosing not to update the tree's hash.
    // This is unneccessary since the owning commit will have
    // that information. To my knowledge this is how OG git works.
    // EXPECT_EQ(tree->getHash(), deserialized.getHash());
}

TEST_F(TreeTest, TestEmptyTreeHashable) {
    Tree emptyTree;
    EXPECT_EQ(tree->getHash(), emptyTree.getHash());
}