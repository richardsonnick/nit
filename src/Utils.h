#include <FileSystemAdaptor.h>
#include <filesystem>

#pragma once

namespace nit::utils {

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
void ensurePathExists(
  FileSystemAdaptorInterface* fsa, const std::filesystem::path& path) {
  nit::utils::walkIntermediatePaths(path, [&](const std::filesystem::path& path) {
    fsa->createDirectory(path);
    if (path.root_path() != path) {
        fsa->addEntry(path.parent_path(), path);
    }
  });
}



} // namespace nit::utils