/**
 * Interface for interacting with the filesystem.
 */
#include <filesystem>

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

class FileSystemAdaptorImpl : public FileSystemAdaptorInterface {
    public:
    void writeBlobToFile(const fs::path& path, const nit::Blob& blob) override;
    nit::Blob getBlobFromFile(const fs::path& path) override;
    nit::Tree getTreeFromPath(const fs::path& path) override;
};