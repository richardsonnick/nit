#include <filesystem>

#include <FileSystemAdaptor.h>

namespace nit {

class ObjectStore {
    public:
    ObjectStore(std::filesystem::path objectStorePath,  std::shared_ptr<FileSystemAdaptorInterface> fs) : objectStorePath(objectStorePath), fs(fs) {};

    std::filesystem::path putObject(const std::vector<uint8_t>& blob);
    std::vector<uint8_t> getObject(const std::string hash);

    private:
    const std::filesystem::path objectStorePath;
    const std::shared_ptr<FileSystemAdaptorInterface> fs;
};


}