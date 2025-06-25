#include <filesystem>

#include <FileSystemAdaptor.h>
#include <Tree.h>
#include <Commit.h>

#pragma once

namespace nit {
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
    const std::vector<Tree>& getTrees() const;

private:
    const std::shared_ptr<FileSystemAdaptorInterface> fsa;
    std::filesystem::path baseRepoPath; // The parent directory that contains `.nit`.
    std::vector<Tree> indexTrees;
};

} // namespace nit