#include <vector>
#include <fstream>

#include <FileSystemAdaptor.h>

void FileSystemAdaptorImpl::writeBlobToFile(const fs::path& path, const nit::Blob& blob) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open file for write: " + path.string());
    }
    out.write(reinterpret_cast<const char*>(blob.data()), blob.size());
}

nit::Blob FileSystemAdaptorImpl::getBlobFromFile(const fs::path& path) {
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

nit::Tree FileSystemAdaptorImpl::getTreeFromPath(const fs::path& path) {
    throw std::runtime_error("Not implemented");
    return {};
} 

void FileSystemAdaptorImpl::createDirectory(const fs::path& path) {
    throw std::runtime_error("Not implemented");
} 