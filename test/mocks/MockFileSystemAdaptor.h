#include <FileSystemAdaptor.h>
#include <types.h>

class MockFileSystemAdaptor : FileSystemAdaptorInterface {
    std::unordered_map<std::string, nit::Blob> filesytemTree; 

    void writeBlobToFile(const std::filesystem::path& path, const nit::Blob& blob) {
        filesytemTree[path.string()] = blob;
    }

    nit::Blob getBlobFromFile(const std::filesystem::path& path) {
        return filesytemTree[path.string()];
    }
};