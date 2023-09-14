#include "hashers.h"

namespace hashers {
STDAPI HashersImpl::CreateHasher(UInt32 index, IHasher** hasher) noexcept {
  return E_NOTIMPL;
}

STDAPI HashersImpl::GetHasherProp(UInt32 codecIndex, PROPID propID,
                                   PROPVARIANT* value) noexcept {
 switch(propID) {
    case NMethodPropID::kID:
      return utils::SetVariant(1ull, value);
    case NMethodPropID::kName:
      return utils::SetVariant(L"NameFromHashers", value);
    default:
      return E_NOTIMPL;
 }
}

UInt32 HashersImpl::GetNumHashers() noexcept {
  return 1;
}
}  // namespace hashers