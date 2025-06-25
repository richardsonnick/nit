/**
 * Interface for interacting with the filesystem.
 */
#include <filesystem>

#include <types.h>
#include <Tree.h>
#include <variant>

#pragma once

namespace nit {

struct File {
    std::filesystem::path path;
    std::vector<uint8_t> blob;
};

struct FileMetadata {
    uint32_t ctime;
    uint32_t mtime;
    uint32_t deviceID;
    uint32_t inode;
    uint32_t fileMode;
    uint32_t userId;
    uint32_t groupId;
    uint32_t fileSize;
    uint16_t flags;
    std::string pathName;
};

class FileSystemAdaptorInterface {
    public:

    using Directory = std::vector<std::filesystem::path>;

    using DirectoryOrFile = std::variant<Directory, File>;

    virtual ~FileSystemAdaptorInterface() = default;
    virtual File fromPath(const std::filesystem::path& path) = 0;

    virtual bool isFile(const std::filesystem::path& path) = 0;

    /**
    * These creation type functions will prempitively create the directory structure needed
    * for the given path. i.e. this will create the necessary path recursively for the desired write path.
    */
    virtual void writeBlobToFile(const std::filesystem::path& path, const std::vector<uint8_t>& blob) = 0;
    virtual void createDirectory(const std::filesystem::path& path) = 0;

    virtual bool pathExists(const std::filesystem::path &path) = 0;

    /**
     * Returns a flat (non-recursive) list of the paths of the entries in `path`;
     **/
    virtual std::vector<std::filesystem::path> getEntries(const std::filesystem::path& path) = 0;

    virtual void addEntry(const std::filesystem::path& path, const DirectoryOrFile entry) = 0;
};

class FileSystemAdaptorImpl : public FileSystemAdaptorInterface {
    public:
    void writeBlobToFile(const std::filesystem::path& path, const std::vector<uint8_t>& blob) override;
    File fromPath(const std::filesystem::path& path) override;
    bool isFile(const std::filesystem::path& path) override; 
    void createDirectory(const std::filesystem::path& path) override;
    bool pathExists(const std::filesystem::path &path) override;
    std::vector<std::filesystem::path> getEntries(const std::filesystem::path& path) override;
    void addEntry(const std::filesystem::path& path, const DirectoryOrFile entry) override;
};

} //namespace nit