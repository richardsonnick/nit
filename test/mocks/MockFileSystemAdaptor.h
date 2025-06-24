#include <FileSystemAdaptor.h>
#include <unordered_map>
#include <filesystem>

namespace nit::test {
class MockFileSystemAdaptor : public FileSystemAdaptorInterface {
public:
    // In-memory mapping of path string -> child Path or Blob
    std::unordered_map<std::filesystem::path, std::vector<PathOrFile>> fsMap;

    void writeBlobToFile(const std::filesystem::path& path, const std::vector<uint8_t>& blob) override;
    std::vector<uint8_t> fromFile(const std::filesystem::path& path) override;
    void createDirectory(const std::filesystem::path& path) override;
    bool pathExists(const std::filesystem::path &path) override;
    std::vector<std::filesystem::path> getEntries(const std::filesystem::path& path) override;
    void addEntry(const std::filesystem::path& path, const PathOrFile entry) override;
};

} // namespace nit::test
