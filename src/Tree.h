#include <vector>


namespace nit {
/** 
 * Trees represent a snapshot of a tree of directories.
 */
class Tree {
public:
    Tree() = default;
    Tree(const std::string& name, const std::string& mode, const std::string& hash)
        : name(name), mode(mode), hash(hash) {}
private:
    std::string name;
    std::string mode; // Modes: "100644" (normal file), "40000" (directory)
    std::string hash;

    std::vector<std::unique_ptr<Tree>> children;
};

}