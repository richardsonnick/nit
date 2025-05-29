#include <MockFileSystemAdaptor.h>

#include <hash-object.h>

void MockFileSystemAdaptor::writeBlobToFile(const fs::path& path, const nit::Blob& blob) {
    blobStore[path.string()] = blob;

    // Determine mode based on path (file for now)
    std::string mode = nit::FILEMODE;

    // Compute hash of blob as file content
    std::string hash = nit::hashObject(blob);

    // Add to fsTree as an entry
    nit::TreeEntry entry{path.string(), mode, hash};
    fsTree.addEntry(entry);
}

nit::Blob MockFileSystemAdaptor::getBlobFromFile(const fs::path& path) {
    auto it = blobStore.find(path.string());
    if (it == blobStore.end()) {
        throw std::runtime_error("File not found: " + path.string());
    }
    return it->second;
}

nit::Tree MockFileSystemAdaptor::getTreeFromPath(const fs::path& path) {
    // Since this is a flat Tree with entries keyed by full path strings,
    // we can collect matching entries under a given path
    nit::Tree subTree;

    std::string pathPrefix = path.string();
    if (!pathPrefix.empty() && pathPrefix.back() != '/') {
        pathPrefix += '/';
    }

    for (const auto& entry : fsTree.getEntries()) {
        if (entry.name.compare(0, pathPrefix.size(), pathPrefix) == 0) {
            subTree.addEntry(entry);
        }
    }

    if (subTree.getEntries().empty()) {
        throw std::runtime_error("Path not found: " + path.string());
    }

    return subTree;
}

void MockFileSystemAdaptor::createDirectory(const fs::path& path) {
    nit::TreeEntry entry{path.string(), nit::DIRMODE, ""};
    fsTree.addEntry(entry);
}