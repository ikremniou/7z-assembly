#ifndef ARCHIVE_SZE_ARCHIVE_H_
#define ARCHIVE_SZE_ARCHIVE_H_

#include <vector>
#include <string>
#include "../plugin/Export.h"

namespace archive {
class SzeInArchive : public CMyUnknownImp, public IInArchive, public IOutArchive {
  Z7_IFACES_IMP_UNK_2(IInArchive, IOutArchive);

private:
  struct File {
    std::wstring path;
    std::vector<char> content;
  };

  UInt32 all_size_;
  std::vector<File> items_;
};
}  // namespace archive

#endif  // ARCHIVE_SZE_ARCHIVE_H_