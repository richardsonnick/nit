/**
 * This is an interface into the filesystem backed object store for nit.
 * This will contain objects indexed in the following fashion:
 *   where id == sha1(object data) and len(id) == 20 bytes
 *   .nit/objects/id[0..1]/id[2..20]
 */
#include<vector>

class FileSystemAdaptor;

namespace store {
    void addObject(FileSystemAdaptor fsa, std::vector<char>& data);
};