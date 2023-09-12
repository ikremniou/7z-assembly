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

HRESULT SetVariant(wchar_t* str_prop, PROPVARIANT* value) noexcept {
  value->vt = VT_BSTR;
  value->bstrVal = SysAllocString(str_prop);
  return S_OK;
}

HRESULT SetVariant(unsigned int uint_prop, PROPVARIANT* value) noexcept {
  value->vt = VT_UI4;
  value->uintVal = uint_prop;
  return S_OK;
}

}  // namespace utils