#include <sstream>

#include <hash-object.h>
#include <Commit.h>

namespace nit {

std::string committerToString(const Committer& c) {
    return c.name + " <" + c.email + "> " + c.timestamp + " " + c.timezone;
}

/**
 * Returns a "blank" commit with only the treeHash.
 * Caller is responsible for filling out the rest of the commit
 * and updating it's hash before "committing".
 */
Commit Commit::fromTree(const Tree& tree) {
    auto treeHash = tree.getHash(); // TODO we should have a notion for a "stale" hash
    return Commit(treeHash);
}

std::string Commit::header(const Commit& c) {
    std::ostringstream header;
    header << "tree " << c.treeHash << "\n";
    if (!c.parentHash.empty()) {
        header << "parent " << c.parentHash << "\n";
    }
    header << "author " << c.author << "\n";
    header << "committer " << committerToString(c.committer) << "\n";
    header << "\n"; // blank line before message
    return header.str();
}

// Returns the serialization of the Commit's contents
Blob Commit::serialize() const {
    auto content = Commit::header(*this) + commitMessage;

    std::ostringstream headerStream;
    headerStream << "commit " << content.size() << '\0';
    std::string header = headerStream.str();

    Blob data;
    data.insert(data.end(), header.begin(), header.end());
    data.insert(data.end(), content.begin(), content.end());

    return data;
}

Commit Commit::deserialize(const Blob& data) {
    Commit commit;
    size_t pos = 0;
    const size_t dataSize = data.size();

    // Read until the first '\0' (skip the "commit <size>\0" header)
    while (pos < dataSize && data[pos] != '\0') {
        pos++;
    }
    if (pos == dataSize) throw std::runtime_error("Invalid commit serialization (missing header terminator)");
    pos++; // skip '\0'

    // Now parse lines until an empty line (message separator)
    while (pos < dataSize) {
        size_t lineEnd = pos;
        while (lineEnd < dataSize && data[lineEnd] != '\n') lineEnd++;
        if (lineEnd == pos) {
            // Empty line means message starts after this
            pos++; 
            break;
        }
        std::string line(reinterpret_cast<const char*>(&data[pos]), lineEnd - pos);

        if (line.starts_with("tree ")) {
            commit.treeHash = line.substr(5);
        } else if (line.starts_with("parent ")) {
            commit.parentHash = line.substr(7);
        } else if (line.starts_with("author ")) {
            commit.author = line.substr(7);
        } else if (line.starts_with("committer ")) {
            std::string committerLine = line.substr(10);
            std::istringstream ss(committerLine);
            ss >> commit.committer.name;

            // read until we hit a '<'
            std::string token;
            while (ss >> token) {
                if (token.starts_with("<")) {
                    commit.committer.email = token.substr(1, token.size() - 2); // strip < and >
                    break;
                } else {
                    commit.committer.name += " " + token;
                }
            }
            ss >> commit.committer.timestamp;
            ss >> commit.committer.timezone;
        }

        pos = lineEnd + 1;
    }

    // Read the commit message
    if (pos < dataSize) {
        commit.commitMessage = std::string(reinterpret_cast<const char*>(&data[pos]), dataSize - pos);
    }

    commit.updateHash();

    return commit;
}

void Commit::updateHash() {
    Blob serialization = serialize();
    hash = nit::hashObject(serialization);
}

void Commit::addParent(Commit& parent) {
    parent.updateHash(); // TODO rethink eager updates of hashes
    parentHash = parent.getHash();
    //Update this hash since we added the parent
    updateHash();
}

std::string Commit::getHash() const {
    return hash;
}

std::string Commit::getAuthor() const {
    return author;
}

std::string Commit::getParentHash() const {
    return parentHash;
}

std::string Commit::getTreeHash() const {
    return treeHash;
}

}