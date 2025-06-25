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
    std::filesystem::path objectsPath = nitPath / "objects";
    std::filesystem::path includePath = repoPath / "include";
    std::filesystem::path mainCppPath = srcPath / "main.cpp";
    std::filesystem::path utilsCppPath = srcPath / "utils.cpp";
    std::filesystem::path headerPath = includePath / "header.h";
    std::filesystem::path utilsHeaderPath = includePath / "utilsHeader.h";

    fsa->addEntry(repoPath, {});
    fsa->setMetadata(repoPath, {
        .ctime = 1640995100,
        .mtime = 1640995090,
        .deviceID = 2049,
        .inode = 123436,
        .fileMode = 0040755,
        .userId = 1000,
        .groupId = 1000,
        .fileSize = 8096,
        .flags = 0,
        .pathName = repoPath.string()
    });
    fsa->addEntry(srcPath, {});
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
    fsa->addEntry(includePath, {});
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
        .pathName = includePath.string()
    });
    fsa->addEntry(nitPath, {});
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
        .pathName = nitPath.string()
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

    std::string utilsCppContentStr = R"(#include <iostream>
int ShoutItOut() {
std::cout << "Scream and SHOUT!!!" << std::endl;
return 0;
})";
    std::vector<uint8_t> utilsCppContent(utilsCppContentStr.begin(), utilsCppContentStr.end());
    std::string utilsHeaderContentStr = R"(#pragma once
void utility();
)";
    std::vector<uint8_t> utilsHeaderContent(utilsHeaderContentStr.begin(), utilsHeaderContentStr.end());

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

    fsa->writeBlobToFile(utilsCppPath, utilsCppContent);
    fsa->setMetadata(utilsCppPath, {
        .ctime = 1640995200,
        .mtime = 1640995300,
        .deviceID = 2049,
        .inode = 789012,
        .fileMode = 0100644,
        .userId = 1000,
        .groupId = 1000,
        .fileSize = 2048,
        .flags = 0,
        .pathName = utilsCppPath.string()
    });
    fsa->writeBlobToFile(utilsHeaderPath, utilsHeaderContent);
    fsa->setMetadata(utilsHeaderPath, {
        .ctime = 1640995400,
        .mtime = 1640995500,
        .deviceID = 2049,
        .inode = 345678,
        .fileMode = 0100755,
        .userId = 1000,
        .groupId = 1000,
        .fileSize = 1024,
        .flags = 0,
        .pathName = utilsHeaderPath.string()
    });

    return repoPath;
}

}