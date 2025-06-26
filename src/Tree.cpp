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

        // TODO: This routine of converting from 8bit to 32bit uints
        //  is very common in this codebase. I should find a way to make
        //  this more ergonomic.
        // Convert to uint32_t hash
        for (const auto& hashWord : entry.hash) {
            data.push_back(static_cast<uint8_t>(hashWord & 0xFF));
            data.push_back(static_cast<uint8_t>((hashWord >> 8) & 0xFF));
            data.push_back(static_cast<uint8_t>((hashWord >> 16) & 0xFF));
            data.push_back(static_cast<uint8_t>((hashWord >> 24) & 0xFF));
        }
    }
    return data;
}

void Tree::updateHash() {
    hash = nit::hashObjectRaw(serialize());
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

        std::array<uint32_t, 5> hash;
        for (int i = 0; i < 5; i++) {
            if (pos + 4 > dataSize) {
                throw new std::runtime_error("Insufficient data for hash.");
            }
            hash[i] = static_cast<uint32_t>(data[pos]) |
                (static_cast<uint32_t>(data[pos + 1]) << 8) |
                (static_cast<uint32_t>(data[pos + 2]) << 16) |
                (static_cast<uint32_t>(data[pos + 3]) << 24);
            pos += 4;
        }

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

const std::array<uint32_t, 5>& Tree::getHash() const {
    return hash;
}

} // namespace nit