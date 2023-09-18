#include "sz-archive.h"
#include "../utils.h"

namespace archive {
HRESULT SzInArchive::Open(IInStream* stream,
                          const UInt64* maxCheckStartPosition,
                          IArchiveOpenCallback* openCallback) noexcept {
  return S_OK;
}

HRESULT SzInArchive::Close() noexcept {
  return S_OK;
}

HRESULT SzInArchive::GetNumberOfItems(UInt32* numItems) noexcept {
  *numItems = 1;
  return S_OK;
}

HRESULT SzInArchive::GetProperty(UInt32 index, PROPID propID,
                                 PROPVARIANT* value) noexcept {
  switch (propID) {
    {
      case kpidPath:
        return utils::SetVariant(L"somefile.txt", value);
      case kpidIsDir:
        return utils::SetVariant(false, value);
      case kpidNumSubFiles:
        return utils::SetVariant(1u, value);
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
  RINOK(extractCallback->GetStream(*indices, &outStream, 0));
  UInt32 size_processed;
  outStream->Write("12345678", 8, &size_processed);
  return S_OK;
}

HRESULT SzInArchive::GetArchiveProperty(PROPID propID,
                                        PROPVARIANT* value) noexcept {
  switch (propID) {
    // Open properties
    case kpidWarningFlags:
    case kpidWarning:
    case kpidErrorFlags:
    case kpidError:
    case kpidOffset:
    case kpidPhySize:
      return S_OK;

    case kpidIsTree:
      return utils::SetVariant(false, value);
    case kpidIsDeleted:
      return utils::SetVariant(false, value);
    case kpidIsAltStream:
      return utils::SetVariant(false, value);
    case kpidIsAux:
      return utils::SetVariant(false, value);
    case kpidINode:
      return utils::SetVariant(false, value);
    case kpidReadOnly:
      return utils::SetVariant(false, value);

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
