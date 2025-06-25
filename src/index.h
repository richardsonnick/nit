#include <filesystem>

#include <FileSystemAdaptor.h>
#include <Tree.h>
#include <Commit.h>
#include <Utils.h>

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
    Index(const std::shared_ptr<FileSystemAdaptorInterface> fsa, const std::filesystem::path& baseRepoPath) :  fsa(fsa), baseRepoPath(baseRepoPath) {}

    // TODO: returns the deserialized Index from path.
    static Index fromIndexObject(std::shared_ptr<FileSystemAdaptorInterface> fsa, const std::filesystem::path& path) {
        File file = fsa->fromPath(path);
        return deserialize(file.blob);
    }


    // TODO: https://mincong.io/2018/04/28/git-index/
    std::vector<uint8_t> serialize();
    static Index deserialize(std::vector<uint8_t>);

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

    const std::filesystem::path& getRepoPath() const;
    const std::vector<IndexEntry>& getEntries() const;
    const std::vector<Tree>& getTrees() const;

private:
    const std::shared_ptr<FileSystemAdaptorInterface> fsa;
    std::filesystem::path baseRepoPath; // The parent directory that contains `.nit`.
    Tree rootTree;
    std::vector<Tree> indexTrees;
    std::vector<IndexEntry> entries;

    IndexEntry fromPath(const std::filesystem::path& path);
    void sortEntries();
};

} // namespace nit