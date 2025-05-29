#include <hash-object.h>

#include <Tree.h>

namespace nit {


void Tree::addEntry(const TreeEntry& entry) {
    entries.push_back(entry);
    updateHash();
}

// Returns the serialization of the Tree's contents
std::vector<uint8_t> Tree::serialize() const {
    std::vector<uint8_t> data;
    for (const auto& entry : entries) {
        std::string line = entry.mode + " " + entry.name;
        data.insert(data.end(), line.begin(), line.end());
        data.push_back('\0');
        data.insert(data.end(), entry.hash.begin(), entry.hash.end());
    }
    return data;
}

void Tree::updateHash() {
    hash = nit::hashObject(serialize());
}

Tree Tree::deserialize(const std::vector<uint8_t>& data) {
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

const std::vector<TreeEntry>& Tree::getEntries() const {
    return entries;
}

const std::string& Tree::getHash() const {
    return hash;
}

} // namespace nit