/**
 * Interface for interacting with the filesystem.
 */
#include <filesystem>

#include <types.h>
#include <Tree.h>
#include <variant>

#pragma once

class FileSystemAdaptorInterface {
    public:

    struct File {
        std::filesystem::path name;
        std::vector<uint8_t> blob;
    };

    using PathOrFile = std::variant<std::filesystem::path, File>;

    static bool isFileBlob(const PathOrFile& pathOrBlob) {
        return std::holds_alternative<File>(pathOrBlob);
    }

    virtual ~FileSystemAdaptorInterface() = default;
    virtual std::vector<uint8_t> fromFile(const std::filesystem::path& path) = 0;

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

    virtual void addEntry(const std::filesystem::path& path, const PathOrFile entry) = 0;
};

class FileSystemAdaptorImpl : public FileSystemAdaptorInterface {
    public:
    void writeBlobToFile(const std::filesystem::path& path, const std::vector<uint8_t>& blob) override;
    std::vector<uint8_t> fromFile(const std::filesystem::path& path) override;
    void createDirectory(const std::filesystem::path& path) override;
    bool pathExists(const std::filesystem::path &path) override;
    std::vector<std::filesystem::path> getEntries(const std::filesystem::path& path) override;
    void addEntry(const std::filesystem::path& path, const PathOrFile entry) override;
};