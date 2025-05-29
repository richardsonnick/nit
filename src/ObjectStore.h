#include <filesystem>

#include <FileSystemAdaptor.h>

namespace nit {

class ObjectStore {
    public:
    ObjectStore(const std::filesystem::path& objectStorePath,  std::shared_ptr<FileSystemAdaptorInterface> fs) : objectStorePath(objectStorePath), fs(fs) {};

    std::filesystem::path putObject(const nit::Blob& blob);
    Blob getObject(const std::string hash);

    private:
    const std::filesystem::path& objectStorePath;
    const std::shared_ptr<FileSystemAdaptorInterface> fs;
};


}