#include "utils.h"

namespace utils {
HRESULT SetPropGUID(const GUID& guid, PROPVARIANT* value) noexcept(false) {
  if ((value->bstrVal =
           ::SysAllocStringByteLen((const char*)&guid, sizeof(guid))) != NULL)
    value->vt = VT_BSTR;
  return S_OK;
}

HRESULT SetVariant(bool bool_prop, PROPVARIANT* value) noexcept {
  value->vt = VT_BOOL;
  value->boolVal = bool_prop ? VARIANT_TRUE : VARIANT_FALSE;
  return S_OK;
}

HRESULT SetVariant(const char* binary, unsigned size,
                   PROPVARIANT* value) noexcept {
  value->vt = VT_BSTR;
  value->bstrVal = ::SysAllocStringByteLen(binary, size);
  return S_OK;
}

HRESULT SetVariant(const wchar_t* str_prop, PROPVARIANT* value) noexcept {
  value->vt = VT_BSTR;
  value->bstrVal = SysAllocString(str_prop);
  return S_OK;
}

HRESULT SetVariant(unsigned int uint_prop, PROPVARIANT* value) noexcept {
  value->vt = VT_UI4;
  value->uintVal = uint_prop;
  return S_OK;
}

HRESULT SetVariant(ULONGLONG ull_value, PROPVARIANT* value) noexcept {
  value->vt = VT_UI8;
  value->uhVal.QuadPart = ull_value;
  return S_OK;
}

HRESULT SetMockTime(PROPVARIANT* value) noexcept {
  FILETIME time{0x1F6FF480, 0x01D9E7E8};
  value->vt = VT_FILETIME;
  value->filetime = time;
  return S_OK;
}

}  // namespace utils