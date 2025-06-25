#include <vector>
#include <fstream>

#include <FileSystemAdaptor.h>

namespace nit {

void FileSystemAdaptorImpl::writeBlobToFile(const std::filesystem::path& path, const std::vector<uint8_t>& blob) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open file for write: " + path.string());
    }
    out.write(reinterpret_cast<const char*>(blob.data()), blob.size());
}

File FileSystemAdaptorImpl::fromPath(const std::filesystem::path& path) {
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

    return File{path, buffer};
}

FileMetadata FileSystemAdaptorImpl::metadataFromPath(const std::filesystem::path& path) {
  throw new std::runtime_error("Not implemented.");
}

bool FileSystemAdaptorImpl::isFile(const std::filesystem::path& path) {
    throw std::runtime_error("Not implemented");
}

void FileSystemAdaptorImpl::createDirectory(const std::filesystem::path& path) {
    throw std::runtime_error("Not implemented");
} 

bool FileSystemAdaptorImpl::pathExists(const std::filesystem::path &path) {
    throw std::runtime_error("Not implemented");
}

std::vector<std::filesystem::path> FileSystemAdaptorImpl::getEntries(const std::filesystem::path& path) {
  throw std::runtime_error("Not implemented");
  return {};
}

void FileSystemAdaptorImpl::addEntry(const std::filesystem::path& path, const DirectoryOrFile entry) {
  throw std::runtime_error("Not implemented");
}

void FileSystemAdaptorImpl::setMetadata(const std::filesystem::path& path, const FileMetadata& metadata)  {
  throw std::runtime_error("Not implemented");
}

} // namespace nit