#include <MockFileSystemAdaptor.h>
#include <Utils.h>

#include <hash-object.h>

namespace nit::test {

void MockFileSystemAdaptor::writeBlobToFile(const std::filesystem::path &path,
                                            const std::vector<uint8_t> &blob) {
  nit::utils::ensurePathExists(this, path.parent_path());
  File fb {path, blob};
  addEntry(path.parent_path(), fb);
}

std::vector<uint8_t> MockFileSystemAdaptor::fromFile(const std::filesystem::path &path) {
  // if (blobMap.find(path) == blobMap.end()) {
  throw std::runtime_error("No FileBlob found for path: " + path.string());
  // }
  // return blobMap[path];
}

void MockFileSystemAdaptor::createDirectory(const std::filesystem::path &path) {
  if (!pathExists(path)) {
    fsMap[path] = {};
  }
}

bool MockFileSystemAdaptor::pathExists(const std::filesystem::path &path) {
  return fsMap.find(path) != fsMap.end();
}


std::vector<std::filesystem::path> MockFileSystemAdaptor::getEntries(const std::filesystem::path &path) {
  return {};
}

void MockFileSystemAdaptor::addEntry(const std::filesystem::path &path,
   const PathOrFile entry) {
  if (pathExists(path)) {
    fsMap[path].push_back(entry);
  }
}

} // namespace nit::test
