#ifndef LIB_ASSEMBLY_H_
#define LIB_ASSEMBLY_H_

#include "./plugin/Export.h"

#define STDAPI_LIB EXTERN_C EXPORTED HRESULT STDAPICALLTYPE
typedef UInt32 (WINAPI *Func_IsArc)(const Byte *p, size_t size);

STDAPI_LIB CreateObject(const GUID *clsid, const GUID *iid, void **outObject);
STDAPI_LIB GetHandlerProperty(PROPID propID, PROPVARIANT *value);
STDAPI_LIB GetNumberOfFormats(UINT32 *numFormats);
STDAPI_LIB GetHandlerProperty2(UInt32 formatIndex, PROPID propID, PROPVARIANT *value);
STDAPI_LIB GetIsArc(UInt32 formatIndex, Func_IsArc *isArc);
STDAPI_LIB GetNumberOfMethods(UInt32 *numCodecs);
STDAPI_LIB GetMethodProperty(UInt32 codecIndex, PROPID propID, PROPVARIANT *value);
STDAPI_LIB CreateDecoder(UInt32 index, const GUID *iid, void **outObject);
STDAPI_LIB CreateEncoder(UInt32 index, const GUID *iid, void **outObject);
STDAPI_LIB GetHashers(IHashers **hashers);
STDAPI_LIB SetCodecs(ICompressCodecsInfo *compressCodecsInfo);
STDAPI_LIB SetLargePageMode();
STDAPI_LIB SetCaseSensitive(Int32 caseSensitive);
STDAPI_LIB GetModuleProp(PROPID propID, PROPVARIANT *value);

void say_hello();

#endif // LIB_ASSEMBLY_H_
