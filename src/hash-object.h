#include <filesystem>
#include <types.h>

#pragma once

namespace nit {
std::pair<std::string, std::string> decomposeHash(const std::string hash);
std::string hashObject(const nit::Blob& obj);
std::filesystem::path hashToFSPath(const std::filesystem::path& objectStorePath, const std::string& hash);
}