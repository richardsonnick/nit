#include <vector>
#include <algorithm>
#include <filesystem>

#include <hash-object.h>
#include <types.h>

#pragma once

namespace fs = std::filesystem;

namespace nit {

std::string hashObject(const nit::Blob& obj);

constexpr std::string DIRMODE = "40000";
constexpr std::string FILEMODE = "100644";

/** 
 * Trees represent a snapshot of a tree of directories.
 */
class Tree {
public:
    Tree() = default;
    Tree(const std::string& name, const std::string& mode, const std::string& hash)
        : name(name), mode(mode), hash(hash) {};
    Tree(const std::string& name, const std::string& mode)
        : name(name), mode(mode) {
            updateHash();
        }

    bool operator==(const Tree& other) const {
        return name == other.name && mode == other.mode && hash == other.hash;
    }

    bool isDir() const {
        return mode == DIRMODE;
    }

    static std::string header(Tree t) {
        return t.name + " " + t.mode;
    }

    static void sortChildren(std::vector<Tree>& children) {
        std::sort(children.begin(), children.end(), [](const Tree& a, const Tree& b) {
            return a.getName() < b.getName();
        });
    }

    // Returns the serialization of the Tree's contents
    Blob serialize() {
        //TODO think about how this eagerly sorts children feels inefficient.
        sortChildren(children);

        Blob data;
        auto addSerial = [&data](Tree t) {
            // Surprisingly git does not hash the blob data along with the object headers :o
            auto header = Tree::header(t);
            data.insert(data.end(), header.begin(), header.end());
            data.push_back('\0');
            data.insert(data.end(), t.hash.begin(), t.hash.end());
        };

        /**
         * Trees in git do not serialize their name and mode!!!!
         * The name and mode is only serialized by the parent tree.
         * Also only the children are serialized!
         * Weird!
         */
        for (const auto& child : children) {
            addSerial(child);
        }

        return data;
    }

    static Tree deserialize(const Blob& data) {
        Tree tree;
        size_t pos = 0;
        const size_t dataSize = data.size();

        while (pos < dataSize) {
            // Parse mode (until space)
            size_t modeEnd = pos;
            while (modeEnd < dataSize && data[modeEnd] != ' ') {
                modeEnd++;
            }
            if (modeEnd == dataSize) throw std::runtime_error("Invalid tree serialization (mode)");
            std::string mode(reinterpret_cast<const char*>(&data[pos]), modeEnd - pos);
            pos = modeEnd + 1;

            // Parse name (until \0)
            size_t nameEnd = pos;
            while (nameEnd < dataSize && data[nameEnd] != '\0') {
                nameEnd++;
            }
            if (nameEnd == dataSize) throw std::runtime_error("Invalid tree serialization (name)");
            std::string name(reinterpret_cast<const char*>(&data[pos]), nameEnd - pos);
            pos = nameEnd + 1;

            // Read 40-char (sha1 len) hash
            if (pos + 40 > dataSize) throw std::runtime_error("Invalid tree serialization (hash)");
            std::string hash(reinterpret_cast<const char*>(&data[pos]), 40);
            pos += 40;

            // Add to entries
            tree.children.push_back(Tree(mode, name, hash));
        }

        sortChildren(tree.children);
        return tree;
    }

    void updateHash() {
        Blob serialization = serialize();
        hash = nit::hashObject(serialization);
    }

    /**
     * Adds child to tree and recomputes the hash.
     */
    void addChild(const Tree& childTree) {
        children.push_back(childTree);
        updateHash();
    }

    std::string getHash() const {
        return hash;
    }

    std::string getName() const {
        return name;
    }

    std::string getMode() const {
        return mode;
    }

    std::vector<Tree> getChildren() const {
        return children;
    }

private:
    std::string name;
    std::string mode; // Modes: "100644" (normal file), "40000" (directory)
    std::string hash;

    std::vector<Tree> children;
};

}