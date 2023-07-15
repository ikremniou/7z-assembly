#include <iostream>
#include "7z-assembly.h"

void say_hello(){
    std::cout << "Hello, from 7z-assemblsssy!\n";
}

STDAPI_LIB CreateObject(const GUID *clsid, const GUID *iid, void **outObject)
{
    return S_OK;
}
