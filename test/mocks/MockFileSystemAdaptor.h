#include <FileSystemAdaptor.h>
#include <unordered_map>
#include <variant>

namespace fs = std::filesystem;

namespace nit::test {
class MockFileSystemAdaptor : public FileSystemAdaptorInterface {
public:
    struct FileBlob {
        fs::path name;
        std::vector<uint8_t> blob;
    };

    // Variant type to hold either a Blob or a string
    using FileOrPath = std::variant<FileBlob, fs::path>;

    // In-memory mapping of path string -> BlobOrString
    std::unordered_map<std::string, std::vector<FileOrPath>> fsMap;

    void writeBlobToFile(const fs::path& path, const std::vector<uint8_t>& blob) override;
    std::vector<uint8_t> fromFile(const fs::path& path) override;

    void createDirectory(const fs::path& path) override;
};

} // namespace nit::test