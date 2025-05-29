#include <filesystem>

#include <FileSystemAdaptor.h>
#include <Tree.h>
#include <Commit.h>

namespace fs = std::filesystem;

#pragma once

namespace nit {
/**
 * Represents the index (staging area) for nit objects.
 */
class Index {
public:
    Index(const std::shared_ptr<FileSystemAdaptorInterface> fsa, const fs::path& baseRepoPath) :  fsa(fsa), baseRepoPath(baseRepoPath) {}

    /**
     * Recursively finds files in `baseRepoPath` and adds them to the index.
     */
    void addTree() {
        indexTree = fsa->getTreeFromPath(baseRepoPath);
    }

    /**
     * This commit is "blank". It is on the caller
     * to fill this commit out before "committing".
     */
    Commit fromIndexTree() const {
        return Commit::fromTree(indexTree);
    }

    const fs::path& getRepoPath() const {
        return baseRepoPath;
    }

    const Tree& getTree() const {
        return indexTree;
    }

private:
    const std::shared_ptr<FileSystemAdaptorInterface> fsa;
    fs::path baseRepoPath; // The parent directory that contains `.nit`.
    Tree indexTree;
};

} // namespace nit