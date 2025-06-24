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
    Index(const std::shared_ptr<FileSystemAdaptorInterface> fsa, const std::filesystem::path& baseRepoPath) :  fsa(fsa), baseRepoPath(baseRepoPath) {}

    /**
     * Recursively finds files in `baseRepoPath` and adds them to the index.
     */
    void addTree();

    /**
     * This commit is "blank". It is on the caller
     * to fill this commit out before "committing".
     */
    Commit fromIndexTree() const;

    const std::filesystem::path& getRepoPath() const;
    const Tree& getTree() const;

private:
    const std::shared_ptr<FileSystemAdaptorInterface> fsa;
    std::filesystem::path baseRepoPath; // The parent directory that contains `.nit`.
    Tree indexTree;
};

} // namespace nit