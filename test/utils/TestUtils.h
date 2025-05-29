#include <filesystem>
#include <FileSystemAdaptor.h>

namespace nit::test {

/**
 * Setup a "dummy" repo using `fsa`.
 * Returns the base repo path.
 */
std::string createInitialRepo(std::shared_ptr<FileSystemAdaptorInterface> fsa) {
    // Setup a initial test project repo
    fs::path repoPath = "/home/snoopy/code/doghouse";
    fs::path srcPath = repoPath / "src";
    fs::path nitPath = repoPath / ".nit";
    fs::path includePath = repoPath / "include";
    fs::path mainCppPath = srcPath / "main.cpp";
    fs::path headerPath = includePath / "header.h";

    fsa->createDirectory(srcPath);
    fsa->createDirectory(includePath);
    fsa->createDirectory(nitPath);

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
    fsa->writeBlobToFile(headerPath, headerContent);

    return nitPath;
}

}