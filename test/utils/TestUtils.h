#include <filesystem>
#include <FileSystemAdaptor.h>

namespace nit::test {

/**
 * Setup a "dummy" repo using `fsa`.
 * Returns the base repo path.
 */
std::string createInitialRepo(std::shared_ptr<FileSystemAdaptorInterface> fsa) {
    // Setup a initial test project repo
    std::filesystem::path repoPath = "/home/snoopy/code/doghouse";
    std::filesystem::path srcPath = repoPath / "src";
    std::filesystem::path nitPath = repoPath / ".nit";
    std::filesystem::path includePath = repoPath / "include";
    std::filesystem::path mainCppPath = srcPath / "main.cpp";
    std::filesystem::path headerPath = includePath / "header.h";

    fsa->createDirectory(srcPath);
    fsa->setMetadata(srcPath, {
        .ctime = 1640995200,
        .mtime = 1640995200,
        .deviceID = 2049,
        .inode = 123456,
        .fileMode = 0040755,
        .userId = 1000,
        .groupId = 1000,
        .fileSize = 4096,
        .flags = 0,
        .pathName = srcPath.string()
    });
    fsa->createDirectory(includePath);
    fsa->setMetadata(includePath, {
        .ctime = 1640995211,
        .mtime = 1640995400,
        .deviceID = 2049,
        .inode = 123457,
        .fileMode = 0040755,
        .userId = 1000,
        .groupId = 1000,
        .fileSize = 2048,
        .flags = 0,
        .pathName = srcPath.string()
    });
    fsa->createDirectory(nitPath);
    fsa->setMetadata(nitPath, {
        .ctime = 1640995811,
        .mtime = 1640997400,
        .deviceID = 2049,
        .inode = 123458,
        .fileMode = 0040755,
        .userId = 1000,
        .groupId = 1000,
        .fileSize = 4096,
        .flags = 0,
        .pathName = srcPath.string()
    });

    std::string mainCppContentStr = R"(#include <iostream>
int main() {
std::cout << "Hello World!" << std::endl;
return 0;
})";
    std::vector<uint8_t> mainCppContent(mainCppContentStr.begin(), mainCppContentStr.end());

    std::string headerContentStr = R"(#pragma once
void function();
)";
    std::vector<uint8_t> headerContent(headerContentStr.begin(), headerContentStr.end());

    fsa->writeBlobToFile(mainCppPath, mainCppContent);
    fsa->setMetadata(mainCppPath, {
        .ctime = 1640995200,
        .mtime = 1640995300,
        .deviceID = 2049,
        .inode = 789012,
        .fileMode = 0100644,
        .userId = 1000,
        .groupId = 1000,
        .fileSize = 2048,
        .flags = 0,
        .pathName = mainCppPath.string()
    });
    fsa->writeBlobToFile(headerPath, headerContent);
    fsa->setMetadata(headerPath, {
        .ctime = 1640995400,
        .mtime = 1640995500,
        .deviceID = 2049,
        .inode = 345678,
        .fileMode = 0100755,
        .userId = 1000,
        .groupId = 1000,
        .fileSize = 1024,
        .flags = 0,
        .pathName = headerPath.string()
    });

    return repoPath;
}

}