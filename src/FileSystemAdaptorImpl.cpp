#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

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
  struct stat statbuf;
  if (stat(path.c_str(), &statbuf) != 0) {
    throw new std::runtime_error("Failed to stat file: " + path.string());
  }

  nit::FileMetadata meta;
  meta.ctime = static_cast<uint32_t>(statbuf.st_ctime);
  meta.mtime = static_cast<uint32_t>(statbuf.st_mtime);
  meta.deviceID = static_cast<uint32_t>(statbuf.st_dev);
  meta.inode = static_cast<uint32_t>(statbuf.st_ino);
  meta.fileMode = static_cast<uint32_t>(statbuf.st_mode);
  meta.userId = static_cast<uint32_t>(statbuf.st_uid);
  meta.groupId = static_cast<uint32_t>(statbuf.st_gid);
  meta.fileSize = static_cast<uint32_t>(statbuf.st_size);
  //meta.flags = 0; // Not sure if i thought this one out...
  meta.pathName = path.filename().string();
}

bool FileSystemAdaptorImpl::isFile(const std::filesystem::path& path) {
  return std::filesystem::is_regular_file(path);
}

void FileSystemAdaptorImpl::createDirectory(const std::filesystem::path& path) {
  std::error_code ec;
  std::filesystem::create_directories(path, ec);
  if (ec) {
    throw new std::runtime_error("Failed to create directory: " + path.string() + " (" + ec.message() + ")");
  }
} 

bool FileSystemAdaptorImpl::pathExists(const std::filesystem::path &path) {
  return std::filesystem::exists(path);
}

std::vector<std::filesystem::path> FileSystemAdaptorImpl::getEntries(const std::filesystem::path& path) {
  std::vector<std::filesystem::path> entries;
  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    entries.push_back(entry.path());
  }
  return entries;
}

void FileSystemAdaptorImpl::addEntry(const std::filesystem::path& path, const DirectoryOrFile entry) {
  if (std::holds_alternative<Directory>(entry)) {
    if (!std::filesystem::exists(path)) {
      createDirectory(path);
    }
  } else if (std::holds_alternative<File>(entry)) {
    const File& file = std::get<File>(entry);
    writeBlobToFile(path, file.blob);
  } else {
    throw new std::runtime_error("Unknown entry type in DirectoryOrFile variant.");
  }
}

void FileSystemAdaptorImpl::setMetadata(const std::filesystem::path& path, const FileMetadata& metadata)  {
  throw new std::runtime_error("setMetadata is intended for testing purposes. ");
}

} // namespace nit