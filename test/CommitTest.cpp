#include <gtest/gtest.h>
#include <Commit.h>
#include <types.h>

using namespace nit;

TEST(CommitTests, TestSerializeDeserialize) {
    Committer committer = {
        "Snoopy",
        "snoopy@yahoomail.com",
        "1748472312",
        "UTC"
    };
    Commit c("da39a3ee5e6b4b0d3255bfef95601890afd80709", "4015b57a143aec5156fd1444a017a32137a3fd0f",
        "NickR23", committer, "Add CommitTest.cpp");
    Blob serial = c.serialize();
    Commit gotCommit = Commit::deserialize(serial);
    EXPECT_EQ(gotCommit, c);

    committer = {
        "Snoopy",
        "snoopy@yahoomail.com",
        "1748472612",
        "UTC"
    };
    Commit parent("0c93927457b40b6e8fbf27ced7ef418d8ebe8479", "",
        "NickR23", committer, "Add CommitTest.cpp");
    c.addParent(parent);
    serial = c.serialize();
    gotCommit = Commit::deserialize(serial);
    EXPECT_EQ(gotCommit, c);
}