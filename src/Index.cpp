#include <Index.h>
#include <hash-object.h>
#include <unordered_map>
#include <algorithm>

namespace nit {

void appendSerializedEntry(std::vector<uint8_t>& serialization, const IndexEntry& entry) {
    auto pushback32 = [&serialization](const uint32_t data){
        serialization.push_back((uint8_t) (data >> 24));
        serialization.push_back((uint8_t) (data >> 16));
        serialization.push_back((uint8_t) (data >> 8));
        serialization.push_back((uint8_t) (data & 0xFF));
    };

    size_t entryStart = serialization.size();
    FileMetadata metadata = entry.metadata;
    pushback32(entry.metadata.ctime);
    pushback32(entry.metadata.mtime);
    pushback32(entry.metadata.deviceID);
    pushback32(entry.metadata.inode);
    pushback32(entry.metadata.fileMode);
    pushback32(entry.metadata.userId);
    pushback32(entry.metadata.groupId);
    pushback32(entry.metadata.fileSize);
    for (auto hashChunk : entry.hash) {
        pushback32(hashChunk);
    }
    serialization.push_back((uint8_t) (entry.flags >> 8));
    serialization.push_back((uint8_t) (entry.flags & 0xFF));

    for (char c : entry.metadata.pathName) {
        serialization.push_back(static_cast<uint8_t>(c));
    }
    serialization.push_back(0); // pathname null terminator

    size_t entryLen = serialization.size() - entryStart;
    size_t paddingLen = (8 - (entryLen % 8)) % 8;
    for (size_t i = 0; i < paddingLen; i++) {
        serialization.push_back(0);
    }

}

void Index::sortEntries() {
    std::sort(entries.begin(), entries.end(), [](const IndexEntry& a, const IndexEntry& b){
        return a.metadata.pathName < b.metadata.pathName;
    });
}

std::vector<uint8_t> Index::serialize() {
    sortEntries();
    std::vector<uint8_t> serialization;
    // bit pack header
    uint8_t dircache = 0x00; // TODO make this frfr
    uint8_t version = 0x01; // Using my own versioning. git's current index version is 0x02 (i think)
    uint8_t numEntries = entries.size();
    uint16_t header = ((uint16_t) dircache << 8) | ((uint16_t) version << 4) | ((uint16_t) numEntries);
    serialization.push_back((uint8_t) (header >> 8));
    serialization.push_back((uint8_t) (header & 0xFF));

    for (IndexEntry& entry : entries) {
        appendSerializedEntry(serialization, entry);
    }

    return serialization;
}

IndexEntry deserializeEntry(const std::vector<uint8_t>& blob, size_t& cursor) {
    IndexEntry entry{};
    size_t entryStart = cursor;

    auto read32 = [&blob, &cursor] () -> uint32_t {
        uint32_t value = ((uint32_t)blob[cursor] << 24) |
                          ((uint32_t)blob[cursor+1] << 16) |
                          ((uint32_t)blob[cursor+2] << 8)  |
                          ((uint32_t)blob[cursor+3]);
        cursor += 4;
        return value;
    };

    entry.metadata.ctime = read32();
    entry.metadata.mtime = read32();
    entry.metadata.deviceID = read32();
    entry.metadata.inode = read32();
    entry.metadata.fileMode = read32();
    entry.metadata.userId = read32();
    entry.metadata.groupId = read32();
    entry.metadata.fileSize = read32();

    // Read sha1 hash
    for (size_t i = 0; i < utils::SHA1_HASH_SIZE / 4; i++) {
        entry.hash[i] = read32();
    }

    entry.flags = ((uint16_t)blob[cursor] << 8) | ((uint16_t)blob[cursor+1]);
    cursor += 2;

    std::string pathname;
    while (cursor < blob.size() && blob[cursor] != 0) {
        pathname += static_cast<char>(blob[cursor++]);
    }
    cursor++; // Skip null terminator
    entry.metadata.pathName = pathname;


    size_t entryLen = cursor - entryStart;
    size_t paddingLen = (8 - (entryLen % 8)) % 8;
    cursor += paddingLen;

    return entry;
}

Index Index::deserialize(std::vector<uint8_t> blob) {
    Index index{};
    size_t cursor = 0;
    uint16_t header = (((uint16_t) blob[cursor++]) << 8) | ((uint16_t) blob[cursor++]);
    uint8_t dircache = (header >> 8) & 0xFF;
    uint8_t indexVersion = (header >> 4) & 0x0F;
    uint8_t entriesSize = ((uint8_t) header & 0x0F);
    index.entries.reserve(entriesSize);

    for (size_t i = 0; i < entriesSize; i++) {
        IndexEntry entry = deserializeEntry(blob, cursor);
        index.entries.push_back(entry);
    }

    return index;
}

IndexEntry Index::fromPath(const std::filesystem::path& path) {
    IndexEntry entry{};
    // TODO we do this twice for each entry bc the Tree repr uses hex
    //  while Index uses raw bytes.
    entry.hash = hashObjectRaw(fsa->fromPath(path).blob);
    entry.metadata = fsa->metadataFromPath(path);
    return entry;
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
