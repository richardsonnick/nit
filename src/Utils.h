#include <FileSystemAdaptor.h>
#include <filesystem>
#include <sstream>

#pragma once

namespace nit::utils {

inline constexpr uint8_t SHA1_HASH_SIZE = 20;

inline std::string rawHashToHex(const std::array<uint32_t, 5>& buf) {
    std::ostringstream oss;
    oss << std::hex;

    for (const auto& value : buf) {
        oss << std::setfill('0') << std::setw(8) << value;
    }

    return oss.str();
}

// TODO: I spent too long on this. There's gotta be a better (safer) way....
inline std::array<uint32_t, 5> hexToRawHash(const std::string& str) {
    // TODO: Will need to modify for diff hash algos.
    if (str.length() != 40) {
        throw new std::invalid_argument("Hex string must be exactly 40 characters long (for sha-1).");
    }

    std::array<uint32_t, 5> buf;

    for (size_t i = 0; i < 5; i++) {
        std::string hexChunk = str.substr(i * 8, 8);

        char* endPtr;
        unsigned long value = std::strtoul(hexChunk.c_str(), &endPtr, 16);

        if (*endPtr != '\0') {
            throw new std::invalid_argument("Invalid hex char string.");
        }

        buf[i] = static_cast<uint32_t>(value);
    }
    return buf;
}

/** 
 * Executes func on each intermediate path in `path`.
 */ 
template<typename Func>
void walkIntermediatePaths(const std::filesystem::path& path, Func&& func) {
    if (path.root_path() != path) {
        walkIntermediatePaths(path.parent_path(), func);
    }
    func(path);
}

// "Preemptively" ensures the given path exists.
inline void ensurePathExists(
  FileSystemAdaptorInterface* fsa, const std::filesystem::path& path) {
  nit::utils::walkIntermediatePaths(path, [&](const std::filesystem::path& path) {
    if (fsa->pathExists(path)) {
        return;
    }
    if (path.root_path() != path) {
        fsa->addEntry(path, {});
    } else {
        fsa->createDirectory(path);
    }
  });
}

/** 
 * Recursively executes func on each intermediate entry starting from `root`. Works bottom up.
 */ 
template<typename Func>
void walkIntermediateEntries(
        FileSystemAdaptorInterface* fsa,
        const std::filesystem::path& root,
        Func&& func) {
    for (auto& entry : fsa->getEntries(root)) {
        walkIntermediateEntries(fsa, entry, func);
    }
    func(root);
}


} // namespace nit::utils