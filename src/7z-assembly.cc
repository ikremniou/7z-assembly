#include "7z-assembly.h"
#include <iostream>
#include "./archive/sz-archive.h"
#include "./archive/sze-archive.h"
#include "utils.h"

// {1e3b7f26-3b1b-4257-a6fa-9c62d67c695e}
Z7_DEFINE_GUID(SzHandlerGuid, 0x1e3b7f26, 0x3b1b, 0x4257, 0xa6, 0xfa, 0x9c,
               0x62, 0xd6, 0x7c, 0x69, 0x5e);
// {78931804-7ba3-4730-86ea-c537b0910adf}
Z7_DEFINE_GUID(SzeHandlerGuid, 0x78931804, 0x7ba3, 0x4730, 0x86, 0xea, 0xc5,
               0x37, 0xb0, 0x91, 0x0a, 0xdf);

// #region archive_handlers
struct ArchiveHandler {
  const wchar_t* name;
  GUID guid;
  const wchar_t* extension;
  const wchar_t* add_extension;
  UInt32 flags;
  char signature[2];
  bool enable_update;
  void* (*in_factory)();
};

const ArchiveHandler handlers[] = {
    {L"SZ",
     SzHandlerGuid,
     L"sz",
     L"",
     NArcInfoFlags::kByExtOnlyOpen,
     {0x53, 0x5A},
     false,
     []() -> void* {
       return new archive::SzInArchive();
     }},
    {L"SZE",
     SzeHandlerGuid,
     L"sze",
     L"",
     NArcInfoFlags::kByExtOnlyOpen,
     {0x53, 0x45},
     true,
     []() -> void* {
       return new archive::SzeInArchive();
     }},
};
// #endregion archive_handlers

// #region create_object
STDAPI_LIB CreateObject(const GUID* clsid, const GUID* iid, void** outObject) {
  for (int i = 0; i < std::size(handlers) && *outObject == nullptr; i++) {
    if (handlers[i].guid == *clsid && *iid == IID_IInArchive) {
      *outObject = handlers[i].in_factory();
    }
  }

  static_cast<IUnknown*>(*outObject)->AddRef();
  return S_OK;
}
// #endregion create_object

// #region get_handler_property
STDAPI_LIB GetHandlerProperty2(UInt32 formatIndex, PROPID propID,
                               PROPVARIANT* value) {
  switch (propID) {
    case NArchive::NHandlerPropID::kName:
      return utils::SetVariant(handlers[formatIndex].name, value);
    case NArchive::NHandlerPropID::kClassID:
      return utils::SetPropGUID(handlers[formatIndex].guid, value);
    case NArchive::NHandlerPropID::kExtension:
      return utils::SetVariant(handlers[formatIndex].extension, value);
    case NArchive::NHandlerPropID::kAddExtension:
      return utils::SetVariant(handlers[formatIndex].add_extension, value);
    case NArchive::NHandlerPropID::kFlags:
      return utils::SetVariant(handlers[formatIndex].flags, value);
    case NArchive::NHandlerPropID::kUpdate:
      return utils::SetVariant(handlers[formatIndex].enable_update, value);
    case NArchive::NHandlerPropID::kTimeFlags:
      return S_OK;
    case NArchive::NHandlerPropID::kSignature:
      return utils::SetVariant(handlers[formatIndex].signature, 2, value);
    case NArchive::NHandlerPropID::kMultiSignature:
      return S_OK;
    case NArchive::NHandlerPropID::kSignatureOffset:
      return utils::SetVariant(0u, value);
    default:
      return E_FAIL;
  }
}
// #endregion get_handler_property

// #region rest
STDAPI_LIB GetHandlerProperty(PROPID propID, PROPVARIANT* value) {
  return GetHandlerProperty2(0, propID, value);
}

STDAPI_LIB GetNumberOfFormats(UINT32* numFormats) {
  *numFormats = static_cast<UInt32>(std::size(handlers));
  return S_OK;
}
// #endregion rest
