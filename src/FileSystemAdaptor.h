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
    virtual nit::Blob fromFile(const fs::path& path) = 0;
    virtual void createDirectory(const fs::path& path) = 0;
};

class FileSystemAdaptorImpl : public FileSystemAdaptorInterface {
    public:
    void writeBlobToFile(const fs::path& path, const nit::Blob& blob) override;
    nit::Blob fromFile(const fs::path& path) override;
    void createDirectory(const fs::path& path) override;
};