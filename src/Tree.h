#include <vector>

#pragma once

namespace nit {

std::string hashObject(const nit::Blob& obj);

constexpr std::string DIRMODE = "40000";
constexpr std::string FILEMODE = "100644";


struct TreeEntry {
    std::string name;
    std::string mode;  // DIRMODE or FILEMODE. TODO maybe make this an enum?
    std::string hash; // blob hash for files and tree hash for directories
};

/** 
 * Trees represent a snapshot of a tree of directories.
 */
class Tree {
public:
    Tree() = default;

    void addEntry(const TreeEntry& entry) {
        entries.push_back(entry);
        updateHash();
    }

    // Returns the serialization of the Tree's contents
    Blob serialize() const {
        Blob data;
        for (const auto& entry : entries) {
            std::string line = entry.mode + " " + entry.name;
            data.insert(data.end(), line.begin(), line.end());
            data.push_back('\0');
            data.insert(data.end(), entry.hash.begin(), entry.hash.end());
        }
        return data;
    }

    void updateHash() {
        hash = nit::hashObject(serialize());
    }

    static Tree deserialize(const Blob& data) {
        Tree tree;
        size_t pos = 0, dataSize = data.size();
        while (pos < dataSize) {
            size_t spacePos = pos;
            while (data[spacePos] != ' ') spacePos++;
            std::string name(data.begin() + pos, data.begin() + spacePos);
            pos = spacePos + 1;

            size_t nullPos = pos;
            while (data[nullPos] != '\0') nullPos++;
            std::string mode(data.begin() + pos, data.begin() + nullPos);
            pos = nullPos + 1;

            std::string hash(data.begin() + pos, data.begin() + pos + 40);
            pos += 40;

            tree.entries.push_back({mode, name, hash});
        }

        // I'm choosing not to update the tree's hash.
        // This is unneccessary since the owning commit will have
        // that information. To my knowledge this is how OG git works.
        // tree.updateHash();
        return tree;
    }

    const std::vector<TreeEntry>& getEntries() const {
        return entries;
    }

    const std::string& getHash() const {
        return hash;
    }

private:
    std::vector<TreeEntry> entries;
    std::string hash;
};

}