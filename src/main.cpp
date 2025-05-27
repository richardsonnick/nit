#include <iostream>

#include <types.h>
#include <hash-object.h>

using namespace std;
int main(){
    std::string objData = "hello";
    std::vector<uint8_t> obj(objData.begin(), objData.end());
    std::string r = nit::hashBlob(obj);
    cout << r << endl;
    return 0;
}