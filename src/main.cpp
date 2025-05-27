#include <iostream>
#include "hash-object.h"

using namespace std;
int main(){
    std::string path = "../test/assets/bee_movie.txt";
    std::string r = hash_object(path);
    cout << r << endl;
    return 0;
}