/**
 * Interface for interacting with the filesystem.
 */
#include <vector>
#include <filesystem>

#include <types.h>

class FileSystemAdaptorInterface {
    virtual nit::Blob writeFile(const std::filesystem::path& path) = 0;
    virtual void putObject(nit::Blob object) = 0;
};

class FileSystemAdaptorImpl : FileSystemAdaptorInterface {
    nit::Blob writeFile(const std::filesystem::path& path) {
        return {};
    }

    void putObject(nit::Blob object) {
    }
};