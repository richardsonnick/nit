#include <filesystem>
#include <types.h>

#pragma once

namespace nit {
std::pair<std::string, std::string> decomposeHash(const std::string hash);
std::array<uint32_t, 5> hashObjectRaw(const std::vector<uint8_t>& obj);
std::string hashObject(const std::vector<uint8_t>& obj);
std::filesystem::path hashToFSPath(const std::filesystem::path& objectStorePath, const std::string& hash);
}