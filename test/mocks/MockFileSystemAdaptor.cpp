#include <MockFileSystemAdaptor.h>

#include <hash-object.h>

namespace nit::test {
void MockFileSystemAdaptor::writeBlobToFile(const fs::path& path, const nit::Blob& blob) {
    FileBlob f{path, blob};
    fsMap[path].push_back(f);
}

nit::Blob MockFileSystemAdaptor::fromFile(const fs::path& path) {
    auto& vec = fsMap[path.string()];
    for (auto& obj : vec) {
        if (std::holds_alternative<FileBlob>(obj)) {
            FileBlob f = std::get<FileBlob>(obj);
            if (f.name == path) {
                return f.blob;
            }
        }
    }
   throw std::runtime_error("No FileBlob found for path: " + path.string());
}

void MockFileSystemAdaptor::createDirectory(const fs::path& path) {
    fsMap[path.string()]  = {};
}

} // namespace nit::test