# nit
A git-like with zero third-party dependencies. 
  - All crypto done with my very own 🕺🏾[boogie](https://github.com/NickR23/boogie)🕺🏾!

## Features
The CLI is still a WIP so this currently doesn't do much beside the unit tests as I work out some primitives.
| Feature            | Status     |
|--------------------|------------|
| git-hash           | WIP        |

## Dependencies
- boogie: https://github.com/NickR23/boogie
  - Requires boogielib installed (follow install guide)

## Building

```bash
# Clone the repository
git clone https://github.com/NickR23/nit.git
cd nit

# Create a build directory
mkdir build && cd build

# Configure and build
# For prod use:
cmake .. 
# Or use the following to enable debug symbols:
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
make
# Places binary at nit/build/bin/nit

# Run tests
ctest

# Install (may need sudo)
make install 
```

## Resources
* git source: https://github.com/git/git/tree/master
* Git from the Bottom Up: https://jwiegley.github.io/git-from-the-bottom-up

