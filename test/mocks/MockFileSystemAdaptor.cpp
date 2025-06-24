#include <MockFileSystemAdaptor.h>
#include <Utils.h>

#include <hash-object.h>

namespace nit::test {

void MockFileSystemAdaptor::writeBlobToFile(const std::filesystem::path &path,
                                            const std::vector<uint8_t> &blob) {
  nit::utils::ensurePathExists(this, path.parent_path());
  File fb {path, blob};
  addEntry(fb.path, fb);
}

FileSystemAdaptorImpl::File MockFileSystemAdaptor::fromPath(const std::filesystem::path &path) {
  if (fsMap.find(path) == fsMap.end()) {
    throw std::runtime_error("No File found for path: " + path.string());
  }
  auto& entry = fsMap[path];
  if (!std::holds_alternative<File>(entry)) {
    throw std::runtime_error("Entry at path: " + path.string() + " is not a file.");
  }
  return std::get<File>(entry);
}

void MockFileSystemAdaptor::createDirectory(const std::filesystem::path &path) {
  if (!pathExists(path)) {
    fsMap[path] = {};
  }
}

bool MockFileSystemAdaptor::pathExists(const std::filesystem::path &path) {
  return fsMap.find(path) != fsMap.end();
}

bool MockFileSystemAdaptor::isFile(const std::filesystem::path& path) {
  auto& entry = fsMap[path];
  if (!std::holds_alternative<File>(entry)) {
    return false;
  }
  return true;
}

std::vector<std::filesystem::path> MockFileSystemAdaptor::getEntries(const std::filesystem::path &path) {
  auto& entry = fsMap[path];
  if (std::holds_alternative<Directory>(entry)) {
    return std::get<Directory>(entry);
  }
  return {};
}

void MockFileSystemAdaptor::addEntry(const std::filesystem::path &path,
   const DirectoryOrFile entry) {
  if (pathExists(path.parent_path())) {
    auto& parentEntry = fsMap[path.parent_path()];
    if (std::holds_alternative<Directory>(parentEntry)) {
      Directory& parentDirectory = std::get<Directory>(parentEntry);
      parentDirectory.push_back(path);
      fsMap[path] = entry;
    }
  }
}

} // namespace nit::test
