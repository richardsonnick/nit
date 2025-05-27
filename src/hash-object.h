#include <hash/sha1.h>

static std::string hash_object(std::string& path) {
    std::string s = "abc";
    return hash::sha1::hash_string(s);
}