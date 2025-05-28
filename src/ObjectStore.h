#include <hash/sha1.h>
#include <FileSystemAdaptor.h>
#include <filesystem>
#include <hash-object.h>

namespace nit {

class ObjectStore {
    public:

    ObjectStore(const std::filesystem::path& objectStorePath,  std::shared_ptr<FileSystemAdaptorInterface> fs) : objectStorePath(objectStorePath), fs(fs) {};

    std::filesystem::path putObject(const nit::Blob& blob) {
        const std::string hash = hashBlob(blob);
        const std::filesystem::path path = hashToFSPath(objectStorePath, hash);
        fs->writeBlobToFile(path, blob);
        return path;
    }

    private:
    const std::filesystem::path& objectStorePath;
    const std::shared_ptr<FileSystemAdaptorInterface> fs;
};


}