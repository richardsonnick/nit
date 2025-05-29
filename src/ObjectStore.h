#include <hash/sha1.h>
#include <FileSystemAdaptor.h>
#include <filesystem>
#include <hash-object.h>

namespace nit {

class ObjectStore {
    public:
    ObjectStore(const std::filesystem::path& objectStorePath,  std::shared_ptr<FileSystemAdaptorInterface> fs) : objectStorePath(objectStorePath), fs(fs) {};

    std::filesystem::path putObject(const nit::Blob& blob) {
        const std::string hash = hashObject(blob);
        const std::filesystem::path path = hashToFSPath(objectStorePath, hash);
        fs->writeBlobToFile(path, blob);
        return path;
    }

    Blob getObject(const std::string hash) {
        auto [prefix, suffix] = decomposeHash(hash);
        const std::filesystem::path path = objectStorePath / prefix / suffix;
        return fs->getBlobFromFile(path);
    }

    private:
    const std::filesystem::path& objectStorePath;
    const std::shared_ptr<FileSystemAdaptorInterface> fs;
};


}