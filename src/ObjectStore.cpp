#include <hash-object.h>

#include <ObjectStore.h>

namespace nit {

std::filesystem::path ObjectStore::putObject(const nit::Blob& blob) {
    const std::string hash = hashObject(blob);
    const std::filesystem::path path = hashToFSPath(objectStorePath, hash);
    fs->writeBlobToFile(path, blob);
    return path;
}

Blob ObjectStore::getObject(const std::string hash) {
    auto [prefix, suffix] = decomposeHash(hash);
    const std::filesystem::path path = objectStorePath / prefix / suffix;
    return fs->fromFile(path);
}

} // namespace nit