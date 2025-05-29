#include <FileSystemAdaptor.h>
#include <unordered_map>

namespace fs = std::filesystem;

class MockFileSystemAdaptor : public FileSystemAdaptorInterface {
public:
    // In-memory mapping of path string -> blob content
    std::unordered_map<std::string, nit::Blob> blobStore;
    // Simulated file system tree
    nit::Tree fsTree;

    void writeBlobToFile(const fs::path& path, const nit::Blob& blob) override;
    nit::Blob getBlobFromFile(const fs::path& path) override;
    nit::Tree getTreeFromPath(const fs::path& path) override;

    void createDirectory(const fs::path& path) override;
};