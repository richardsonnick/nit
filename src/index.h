#include <filesystem>

#include <FileSystemAdaptor.h>
#include <Tree.h>
#include <Commit.h>
#include <Utils.h>
#include <optional>
#include <ObjectStore.h>

#pragma once

namespace nit {

struct IndexEntry {
    FileMetadata metadata;
    std::array<uint32_t, utils::SHA1_HASH_SIZE / 4> hash; // 5 * 4 bytes = 20 bytes (SHA-1)
    uint16_t flags;
};

/**
 * Represents the index (staging area) for nit objects.
 */
class Index {
public:
    Index() = default;
    Index(const std::shared_ptr<FileSystemAdaptorInterface> fsa, 
        const std::shared_ptr<ObjectStore> objectStore,
        const std::filesystem::path& baseRepoPath) :  fsa(fsa), objectStore(objectStore), baseRepoPath(baseRepoPath) {}

    // // TODO: returns the deserialized Index from path.
    // static Index fromIndexObject(std::shared_ptr<FileSystemAdaptorInterface> fsa, const std::filesystem::path& path) {
    //     File file = fsa->fromPath(path);
    //     return deserialize(file.blob);
    // }


    // TODO: https://mincong.io/2018/04/28/git-index/
    std::vector<uint8_t> serialize();
    // TODO: I should find a way to remove the objectStore dependency here...
    //  Currently it is needed to retrieve the "rootTree" for the index.
    static Index deserialize(std::vector<uint8_t> blob, std::shared_ptr<ObjectStore> objectStore);

    /**
     * Recursively finds files in `baseRepoPath` and adds them to the index.
     */
    // TODO this should take a path and only add the recursive entries to the index.
    void addTrees();

    /**
     * This commit is "blank". It is on the caller
     * to fill this commit out before "committing".
     */
    Commit fromIndexTree() const;

    void setObjectStore(std::shared_ptr<ObjectStore> objStore);
    const std::filesystem::path& getRepoPath() const;
    const std::vector<IndexEntry>& getEntries() const;
    const std::unique_ptr<Tree>& getRootTree() const;

private:
    const std::shared_ptr<FileSystemAdaptorInterface> fsa;
    std::shared_ptr<ObjectStore> objectStore;
    std::filesystem::path baseRepoPath; // The parent directory that contains `.nit`.
    std::unique_ptr<Tree> rootTree;
    std::vector<IndexEntry> entries;

    std::optional<Tree> findTree(const std::array<uint32_t, 5>& hash);
    IndexEntry fromPath(const std::filesystem::path& path);
    void sortEntries();
};

} // namespace nit