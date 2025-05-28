/**
 * Interface for interacting with the filesystem.
 */
#include <vector>
#include <filesystem>
#include <fstream>

#include <types.h>

#pragma once

class FileSystemAdaptorInterface {
    public:
    virtual ~FileSystemAdaptorInterface() = default;
    virtual void writeBlobToFile(const std::filesystem::path& path, const nit::Blob& blob) = 0;
    virtual nit::Blob getBlobFromFile(const std::filesystem::path& path) = 0;
};

class FileSystemAdaptorImpl : public FileSystemAdaptorInterface {
    public:
    void writeBlobToFile(const std::filesystem::path& path, const nit::Blob& blob) {
        std::ofstream out(path, std::ios::binary);
        if (!out) {
            throw std::runtime_error("Failed to open file for write: " + path.string());
        }
        out.write(reinterpret_cast<const char*>(blob.data()), blob.size());
    }

    nit::Blob getBlobFromFile(const std::filesystem::path& path) {
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
};