#include "sz-archive.h"
#include <unordered_map>
#include "../utils.h"

namespace archive {

struct File {
  const wchar_t* path;
  bool is_dir;
};

File files[2] = {
    {L"somefile.txt", false},
    {L"somefile.sz", false},
};

HRESULT SzInArchive::Open(IInStream* stream,
                          const UInt64* maxCheckStartPosition,
                          IArchiveOpenCallback* openCallback) noexcept {
  return S_OK;
}

HRESULT SzInArchive::Close() noexcept {
  return S_OK;
}

HRESULT SzInArchive::GetNumberOfItems(UInt32* numItems) noexcept {
  *numItems = static_cast<UInt32>(std::size(files));
  return S_OK;
}

HRESULT SzInArchive::GetProperty(UInt32 index, PROPID propID,
                                 PROPVARIANT* value) noexcept {
  switch (propID) {
    {
      case kpidPath:
        return utils::SetVariant(files[index].path, value);
      case kpidIsDir:
        return utils::SetVariant(files[index].is_dir, value);
      case kpidNumSubFiles:
      case kpidMTime:
      case kpidIsAltStream:
      case kpidEncrypted:
      case kpidAttrib:
      case kpidNumSubDirs:
      case kpidSize:
      case kpidPackSize:
      case kpidCRC:
      case kpidHardLink:
      case kpidPosition:
      case kpidSymLink:
      case kpidPosixAttrib:
      case kpidATime:
      case kpidCTime:
      case kpidIsAnti:
        return S_OK;
      default:
        return E_NOTIMPL;
    }
  }
}

HRESULT SzInArchive::Extract(
    const UInt32* indices, UInt32 numItems, Int32 testMode,
    IArchiveExtractCallback* extractCallback) noexcept {
  CMyComPtr<ISequentialOutStream> outStream;
  for (UInt32 i = 0; i < numItems; i++) {
    RINOK(extractCallback->GetStream(*indices, &outStream, 0));
    UInt32 size_processed;
    outStream->Write("12345678", 8, &size_processed);
  }

  return S_OK;
}

HRESULT SzInArchive::GetArchiveProperty(PROPID propID,
                                        PROPVARIANT* value) noexcept {
  switch (propID) {
    case kpidWarningFlags:
    case kpidWarning:
    case kpidErrorFlags:
    case kpidError:
    case kpidOffset:
    case kpidPhySize:
      return S_OK;

    case kpidIsTree:
    case kpidIsDeleted:
    case kpidIsAltStream:
    case kpidIsAux:
    case kpidINode:
    case kpidReadOnly:
      return utils::SetVariant(true, value);

    case kpidMainSubfile:
      return utils::SetVariant(0u, value);

    default:
      return E_NOTIMPL;
  }
}

HRESULT SzInArchive::GetNumberOfProperties(UInt32* numProps) noexcept {
  *numProps = 0;
  return S_OK;
}

HRESULT SzInArchive::GetPropertyInfo(UInt32 index, BSTR* name, PROPID* propID,
                                     VARTYPE* varType) noexcept {
  return E_NOTIMPL;
}

HRESULT SzInArchive::GetNumberOfArchiveProperties(UInt32* numProps) noexcept {
  return E_NOTIMPL;
}

HRESULT SzInArchive::GetArchivePropertyInfo(UInt32 index, BSTR* name,
                                            PROPID* propID,
                                            VARTYPE* varType) noexcept {
  return E_NOTIMPL;
}

}  // namespace archive
