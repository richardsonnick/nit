/**
 * Interface for interacting with the filesystem.
 */
#include <vector>
#include <filesystem>
#include <fstream>

#include <types.h>
#include <Tree.h>

#pragma once

namespace fs = std::filesystem;

class FileSystemAdaptorInterface {
    public:
    virtual ~FileSystemAdaptorInterface() = default;
    virtual void writeBlobToFile(const fs::path& path, const nit::Blob& blob) = 0;
    virtual nit::Blob getBlobFromFile(const fs::path& path) = 0;

    /**
     * Recursively builds a Tree from path.
     */
    virtual nit::Tree getTreeFromPath(const fs::path& path) = 0;
};

class FSNode {
    std::string path;
    bool isDirectory;
    std::shared_ptr<FSNode> parent;
    std::vector<std::shared_ptr<FSNode>> children;
};

class FileSystemAdaptorImpl : public FileSystemAdaptorInterface {
    public:
    void writeBlobToFile(const fs::path& path, const nit::Blob& blob) {
        std::ofstream out(path, std::ios::binary);
        if (!out) {
            throw std::runtime_error("Failed to open file for write: " + path.string());
        }
        out.write(reinterpret_cast<const char*>(blob.data()), blob.size());
    }

    nit::Blob getBlobFromFile(const fs::path& path) {
       std::ifstream in(path, std::ios::binary);
       if (!in) {
            throw std::runtime_error("Failed to open file for read: " + path.string());
       }

       in.seekg(0, std::ios::end);
       std::streamsize size = in.tellg(); // Gets the file size in bytes
       in.seekg(0, std::ios::beg);

       std::vector<uint8_t> buffer(size);
       if(!in.read(reinterpret_cast<char*>(buffer.data()), size)) {
            throw std::runtime_error("Failed to read file: " + path.string());
       }

       return buffer;
    }

    nit::Tree getTreeFromPath(const fs::path& path) {
        throw std::runtime_error("Not implemented");
        return {};
    } 
};