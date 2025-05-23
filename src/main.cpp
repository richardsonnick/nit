#include <iostream>
#include "hash-object.h"

using namespace std;
int main(){
    std::string path = "foo/bar/test.txt";
    std::string r = hashObject(path);
    cout << r << endl;
    return 0;
}