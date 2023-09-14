#ifndef HASHERS_HASHERS_H_
#define HASHERS_HASHERS_H_

#include "../plugin/Export.h"
#include "../utils.h"

namespace hashers {
class HashersImpl : public IHashers, public CMyUnknownImp {
  Z7_IFACES_IMP_UNK_1(IHashers)
};
}  // namespace hashers

#endif  // HASHERS_HASHERS_H_