#include "7z-assembly.h"
#include <iostream>

void say_hello() {
  std::cout << "Hello, from 7z-assembly!\n";
}

STDAPI_LIB CreateObject(const GUID* clsid, const GUID* iid, void** outObject) {
  return S_OK;
}

STDAPI_LIB GetHandlerProperty(PROPID propID, PROPVARIANT* value) {
  return S_OK;
}

STDAPI_LIB GetNumberOfFormats(UINT32* numFormats) {
  return S_OK;
}

STDAPI_LIB GetIsArc(UInt32 formatIndex, Func_IsArc* isArc) {
  return S_OK;
}

STDAPI_LIB GetNumberOfMethods(UInt32* numCodecs) {
  return S_OK;
}

STDAPI_LIB GetMethodProperty(UInt32 codecIndex, PROPID propID,
                             PROPVARIANT* value) {
  return S_OK;
}

STDAPI_LIB CreateDecoder(UInt32 index, const GUID* iid, void** outObject) {
  return S_OK;
}

STDAPI_LIB CreateEncoder(UInt32 index, const GUID* iid, void** outObject) {
  return S_OK;
}

STDAPI_LIB GetHashers(IHashers** hashers) {
  return S_OK;
}

STDAPI_LIB SetCodecs(ICompressCodecsInfo* compressCodecsInfo) {
  return S_OK;
}

STDAPI_LIB SetLargePageMode() {
  return S_OK;
}

STDAPI_LIB SetCaseSensitive(Int32 caseSensitive) {
  return S_OK;
}

STDAPI_LIB GetModuleProp(PROPID propID, PROPVARIANT* value) {
  return S_OK;
}
