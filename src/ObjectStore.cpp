#include <hash-object.h>

#include <ObjectStore.h>

namespace nit {

std::filesystem::path ObjectStore::putObject(const std::vector<uint8_t>& blob) {
    const std::string hash = hashObject(blob);
    const std::filesystem::path path = hashToFSPath(objectStorePath, hash);
    fs->writeBlobToFile(path, blob);
    return path;
}

std::vector<uint8_t> ObjectStore::getObject(const std::string hash) {
    auto [prefix, suffix] = decomposeHash(hash);
    const std::filesystem::path path = objectStorePath / prefix / suffix;
    return fs->fromPath(path).blob;
}

} // namespace nit