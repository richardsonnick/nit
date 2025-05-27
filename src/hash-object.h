#include <hash/sha1.h>

static std::string hash_object(const std::string& path) {
    return hash::sha1::hash_file(path);
}