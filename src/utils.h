#ifndef UTILS_H_
#define UTILS_H_

#include "./plugin/Export.h"

namespace utils {
HRESULT SetPropGUID(const GUID& guid, PROPVARIANT* value) noexcept(false);
HRESULT SetVariant(bool bool_prop, PROPVARIANT* value) noexcept;
HRESULT SetVariant(const char* binary, unsigned size,
                   PROPVARIANT* value) noexcept;
HRESULT SetVariant(const wchar_t* str_prop, PROPVARIANT* value) noexcept;
HRESULT SetVariant(unsigned int uint_prop, PROPVARIANT* value) noexcept;
HRESULT SetVariant(ULONGLONG ull_value, PROPVARIANT* value) noexcept;
HRESULT SetMockTime(PROPVARIANT* value) noexcept;
}  // namespace utils

#endif  // UTILS_H_