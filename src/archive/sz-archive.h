#ifndef ARCHIVE_SZ_ARCHIVE_H_
#define ARCHIVE_SZ_ARCHIVE_H_

#include "../plugin/Export.h"

namespace archive {
// #region snippet
class SzInArchive : public CMyUnknownImp, public IInArchive {
 public:
  Z7_IFACES_IMP_UNK_1(IInArchive);
};
// #endregion snippet
}  // namespace archive

#endif  // ARCHIVE_SZ_ARCHIVE_H_