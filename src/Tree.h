#include <vector>
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
    Tree(const std::string& name, const std::string& mode)
        : name(name), mode(mode) {
            updateHash();
        }

    bool isDir() const {
        return mode == DIRMODE;
    }

    // Returns the serialization of the Tree's contents
    Blob serialize() const {
        Blob data;
        
        // TODO does git sort the entries somehow???

        for (const auto& child : children) {
            // Surprisingly git does not hash the blob data along with the object headers :o
            std::string header = child.mode + " " + child.name;
            data.insert(data.end(), header.begin(), header.end());
            data.push_back('\0');

            data.insert(data.end(), child.hash.begin(), child.hash.end());
        }

        return data;
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

private:
    std::string name;
    std::string mode; // Modes: "100644" (normal file), "40000" (directory)
    std::string hash;

    std::vector<Tree> children;
};

}