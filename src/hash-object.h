#include <hash/sha1.h>
#include <FileSystemAdaptor.h>
#include <filesystem>

#pragma once

namespace nit {

static std::string hashBlob(const nit::Blob& blob) {
    std::string data(reinterpret_cast<const char*>(blob.data()), blob.size());
    std::istringstream iss(data);
    return hash::sha1::hash_stream(iss);
}

static std::filesystem::path hashToFSPath(const std::filesystem::path& objectStorePath, const std::string& hash) {
    const std::string prefix = hash.substr(0, 2);
    const std::string suffix = hash.substr(2);
    return objectStorePath / prefix / suffix;
}

}