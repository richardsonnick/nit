#include <Index.h>
#include <hash-object.h>
#include <Utils.h>
#include <unordered_map>

namespace nit {

std::vector<uint8_t> Index::serialize() {
    return {};
}

Index deserialize(std::vector<uint8_t> blob) {
    return {};
}

/**
 * Recursively finds files in `baseRepoPath` and adds them to the index.
 */
void Index::addTrees() {
    std::unordered_map<std::filesystem::path, Tree> treeMap;

    nit::utils::walkIntermediateEntries(fsa.get(), baseRepoPath, [&](const std::filesystem::path& path){
        std::string mode;
        std::string hash;
        auto relativePath = std::filesystem::relative(path, baseRepoPath);
        TreeEntry entry;
        if (fsa->isFile(path)) {
            mode = FILEMODE;
            hash = hashObject(fsa->fromPath(path).blob);
            entry = {
                relativePath, 
                mode,
                hash
            };
        } else {
            treeMap[path] = Tree();
            mode = DIRMODE;
            entry = {
                relativePath, 
                mode,
                ""
            };
            treeMap[path].updateHash();
        }
        treeMap[path.parent_path()].addEntry(entry);
    });

    indexTrees.reserve(treeMap.size());
    for (auto& [path, tree] : treeMap) {
        indexTrees.push_back(tree);
    }
}

/**
 * This commit is "blank". It is on the caller
 * to fill this commit out before "committing".
 */
Commit Index::fromIndexTree() const {
    throw new std::runtime_error("Not implemented.");
    // return Commit::fromTree(indexTree);
}

const std::filesystem::path& Index::getRepoPath() const {
    return baseRepoPath;
}

const std::vector<Tree>& Index::getTrees() const {
    return indexTrees;
}

} // namespace nit
