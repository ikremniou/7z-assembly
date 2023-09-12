#ifndef UTILS_H_
#define UTILS_H_

#include "./plugin/Export.h"

namespace utils {
HRESULT SetPropGUID(const GUID& guid, PROPVARIANT* value) noexcept(false);
HRESULT SetVariant(bool bool_prop, PROPVARIANT* value) noexcept;
HRESULT SetVariant(wchar_t* str_prop, PROPVARIANT* value) noexcept;
HRESULT SetVariant(unsigned int uint_prop, PROPVARIANT* value) noexcept;
}  // namespace utils

#endif  // UTILS_H_