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
        auto relativePath = std::filesystem::relative(path, baseRepoPath);
        if (relativePath.string().starts_with(".nit")) {
            return;
        }

        TreeEntry entry;
        if (fsa->isFile(path)) {
            IndexEntry indexEntry = fromPath(path);
            entries.push_back(indexEntry);
            mode = FILEMODE;
            // TODO we compute the hash twice for the raw and string versions. Let's fix this.
            std::string hexHash = hashObject(fsa->fromPath(path).blob);
            entry = {
                relativePath, 
                mode,
                hexHash
            };
        } else {
            mode = DIRMODE;
            treeMap[path].updateHash();
            entry = {
                relativePath, 
                mode,
                treeMap[path].getHash()
            };
            if (treeMap.find(path) == treeMap.end()) {
                treeMap[path] = Tree();
            }
        }
        if (path == baseRepoPath) {
            rootTree = treeMap[path];
        } else {
        treeMap[path.parent_path()].addEntry(entry);
        }
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

const std::vector<IndexEntry>& Index::getEntries() const {
    return entries;
}

} // namespace nit
