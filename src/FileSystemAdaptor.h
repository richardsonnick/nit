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
    virtual void writeBlobToFile(const fs::path& path, const std::vector<uint8_t>& blob) = 0;
    virtual std::vector<uint8_t> fromFile(const fs::path& path) = 0;
    virtual void createDirectory(const fs::path& path) = 0;
};

class FileSystemAdaptorImpl : public FileSystemAdaptorInterface {
    public:
    void writeBlobToFile(const fs::path& path, const std::vector<uint8_t>& blob) override;
    std::vector<uint8_t> fromFile(const fs::path& path) override;
    void createDirectory(const fs::path& path) override;
};