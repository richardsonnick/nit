#include <Index.h>

namespace fs = std::filesystem;

namespace nit {

/**
 * Recursively finds files in `baseRepoPath` and adds them to the index.
 */
void Index::addTree() {
    throw std::runtime_error("Not implemented");
}

/**
 * This commit is "blank". It is on the caller
 * to fill this commit out before "committing".
 */
Commit Index::fromIndexTree() const {
    return Commit::fromTree(indexTree);
}

const fs::path& Index::getRepoPath() const {
    return baseRepoPath;
}

const Tree& Index::getTree() const {
    return indexTree;
}

} // namespace nit