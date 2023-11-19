#ifndef ARCHIVE_SZE_ARCHIVE_H_
#define ARCHIVE_SZE_ARCHIVE_H_

#include <string>
#include <vector>
#include "../plugin/Export.h"

namespace archive {
class SzeInArchive : public CMyUnknownImp,
                     public IInArchive,
                     public IOutArchive {
  Z7_IFACES_IMP_UNK_2(IInArchive, IOutArchive);

 private:
  struct File {
    std::string path;
    std::vector<char> content;
  };

  void WriteFilesToOutStream(ISequentialOutStream* outStream);
  void UpdateItemsInMemItems(UInt32 numItems,
                             IArchiveUpdateCallback* updateCallback);

  UInt32 all_size_;
  std::vector<File> items_;
};
}  // namespace archive

#endif  // ARCHIVE_SZE_ARCHIVE_H_