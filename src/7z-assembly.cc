#include "7z-assembly.h"
#include <iostream>
#include "./hashers/hashers.h"
#include "utils.h"

void say_hello() {
  std::cout << "Hello, from 7z-assembly!\n";
}

// {da3d52d8-9f83-40de-ab27-b1f355716887}
Z7_DEFINE_GUID(SzEncoderGuid, 0xda3d52d8, 0x9f83, 0x40de, 0xab, 0x27, 0xb1,
               0xf3, 0x55, 0x71, 0x68, 0x87);
// {d60c3998-2341-4dca-95b2-be58fa5a83ef}
Z7_DEFINE_GUID(SzDecoderGuid, 0xd60c3998, 0x2341, 0x4dca, 0x95, 0xb2, 0xbe,
               0x58, 0xfa, 0x5a, 0x83, 0xef);
// {1e3b7f26-3b1b-4257-a6fa-9c62d67c695e}
Z7_DEFINE_GUID(SzHandlerGuid, 0x1e3b7f26, 0x3b1b, 0x4257, 0xa6, 0xfa, 0x9c, 0x62, 0xd6, 0x7c, 0x69, 0x5e);


STDAPI_LIB CreateObject(const GUID* clsid, const GUID* iid, void** outObject) {
  return S_OK;
}

STDAPI_LIB GetHandlerProperty2(UInt32 formatIndex, PROPID propID,
                               PROPVARIANT* value) {
  switch (propID) {
    case NArchive::NHandlerPropID::kName:
      return utils::SetVariant(L"SZ", value);
    case NArchive::NHandlerPropID::kClassID:
      return utils::SetPropGUID(SzHandlerGuid, value);
    case NArchive::NHandlerPropID::kExtension:
      return utils::SetVariant(L"sz", value);
    case NArchive::NHandlerPropID::kAddExtension:
      return utils::SetVariant(L"", value);
    case NArchive::NHandlerPropID::kUpdate:
      return utils::SetVariant(false, value);
    case NArchive::NHandlerPropID::kFlags:
      return utils::SetVariant(0u, value);
    case NArchive::NHandlerPropID::kTimeFlags:
      return utils::SetVariant(0u, value);
    case NArchive::NHandlerPropID::kSignature:
      return utils::SetVariant(L"signature", value);
    case NArchive::NHandlerPropID::kMultiSignature:
        return E_FAIL;
    case NArchive::NHandlerPropID::kSignatureOffset:
        return S_OK;
    default:
        return E_FAIL;
  }
  return S_OK;
}

STDAPI_LIB GetHandlerProperty(PROPID propID, PROPVARIANT* value) {
  return GetHandlerProperty2(0, propID, value);
}

STDAPI_LIB GetNumberOfFormats(UINT32* numFormats) {
  *numFormats = 1;
  return S_OK;
}

STDAPI_LIB GetIsArc(UInt32 formatIndex, Func_IsArc* isArc) {
  return S_OK;
}

STDAPI_LIB GetNumberOfMethods(UInt32* numCodecs) {
  *numCodecs = 1;
  return S_OK;
}

STDAPI_LIB GetMethodProperty(UInt32 codecIndex, PROPID propID,
                             PROPVARIANT* value) {
  switch (propID) {
    case NMethodPropID::kEncoder:
      return utils::SetPropGUID(SzEncoderGuid, value);
    case NMethodPropID::kDecoder:
      return utils::SetPropGUID(SzDecoderGuid, value);
    case NMethodPropID::kIsFilter:
      return utils::SetVariant(false, value);
  }

  return S_OK;
}

STDAPI_LIB CreateDecoder(UInt32 index, const GUID* iid, void** outObject) {
  return S_OK;
}

STDAPI_LIB CreateEncoder(UInt32 index, const GUID* iid, void** outObject) {
  return S_OK;
}

STDAPI_LIB GetHashers(IHashers** hashers) {
  static hashers::HashersImpl real_hashers;
  *hashers = &real_hashers;
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
  switch (propID) {
    case NModulePropID::EEnum::kInterfaceType:
      value->vt = VT_UI4;
      value->uiVal = 0;
      return S_OK;
    case NModulePropID::EEnum::kVersion:
      value->vt = VT_UI4;
      value->uiVal = 1;
      return S_OK;
  }
  return S_OK;
}
