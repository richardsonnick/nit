/**
 * Interface for interacting with the filesystem.
 */
#include <vector>

class FileSystemAdaptorInterface {
    virtual std::vector<char> getObject(const std::string& path) = 0;
    virtual void putObject(std::vector<char> object) = 0;
};