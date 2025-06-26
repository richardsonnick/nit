#include <types.h>
#include <Tree.h>

#pragma once

namespace nit {

struct Committer {
    std::string name;
    std::string email;
    std::string timestamp;
    std::string timezone;
};

std::string committerToString(const Committer& c);

/** 
 * Commits represent metadata for Trees.
 * They basically tell us how we got to this state of the repo
 */
class Commit {
public:
    Commit() = default;
    Commit(std::array<uint32_t, 5> treeHash) : treeHash(treeHash) {} ;
    Commit(std::array<uint32_t, 5> treeHash, std::array<uint32_t, 5> parentHash, std::string author,
        Committer committer, std::string commitMessage, std::array<uint32_t, 5> hash) :
            treeHash(treeHash), parentHash(parentHash), author(author), committer(committer),
            commitMessage(commitMessage), hash(hash) {};
    Commit(std::array<uint32_t, 5> treeHash, std::array<uint32_t, 5> parentHash, std::string author,
        Committer committer, std::string commitMessage) :
            treeHash(treeHash), parentHash(parentHash), author(author), committer(committer),
            commitMessage(commitMessage) {
                updateHash();
            };

    bool operator==(const Commit& other) const {
        auto committerEqual = [](Committer c, Committer o) {
            return c.name == o.name &&
                c.email == o.email &&
                c.timestamp == o.timestamp &&
                c.timezone == o.timezone;
        };
        return treeHash == other.treeHash && 
                parentHash == other.parentHash && 
                author == other.author &&
                committerEqual(this->committer, other.committer) &&
                commitMessage == other.commitMessage &&
                hash == other.hash;
    }

    /**
     * Returns a "blank" commit with only the treeHash.
     * Caller is responsible for filling out the rest of the commit
     * and updating it's hash before "committing".
     */
    static Commit fromTree(const Tree& tree); 
    static std::string header(const Commit& c); 

    // Returns the serialization of the Commit's contents
    std::vector<uint8_t> serialize() const;
    static Commit deserialize(const std::vector<uint8_t>& data);

    void updateHash();
    void addParent(Commit& parent);

    std::array<uint32_t, 5> getHash() const;
    std::string getAuthor() const;
    std::array<uint32_t, 5> getParentHash() const;
    std::array<uint32_t, 5> getTreeHash() const;

private:
    std::array<uint32_t, 5> treeHash;
    std::array<uint32_t, 5> parentHash; // TODO For merges this could be one or more. I'm not supporting that yet.
    std::string author;
    Committer committer;
    std::string commitMessage;
    std::array<uint32_t, 5> hash;
};

}