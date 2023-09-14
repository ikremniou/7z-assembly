#include "sz-archive.h"

namespace archive {
HRESULT SzInArchive::Open(IInStream* stream,
                          const UInt64* maxCheckStartPosition,
                          IArchiveOpenCallback* openCallback) noexcept {
  UInt64 mock_total_files = 200;
  UInt64 mock_total_bytes = 1000;
  openCallback->SetTotal(&mock_total_files, &mock_total_bytes);
  UInt64 mock_completed_bytes = 0;
  UInt64 mock_completed_files = 0;
  while (mock_completed_files < mock_total_files) {
    mock_completed_files++;
    mock_completed_bytes += mock_total_bytes / mock_total_files;
    openCallback->SetCompleted(&mock_completed_files, &mock_completed_bytes);
    Sleep(10);
  }
  return S_OK;
}

HRESULT SzInArchive::Close() noexcept {
  return E_NOTIMPL;
}

HRESULT SzInArchive::GetNumberOfItems(UInt32* numItems) noexcept {
  return E_NOTIMPL;
}

HRESULT SzInArchive::GetProperty(UInt32 index, PROPID propID,
                                 PROPVARIANT* value) noexcept {
  return E_NOTIMPL;
}

HRESULT SzInArchive::Extract(
    const UInt32* indices, UInt32 numItems, Int32 testMode,
    IArchiveExtractCallback* extractCallback) noexcept {
  return E_NOTIMPL;
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
    default:
        return E_NOTIMPL;
  }
}

HRESULT SzInArchive::GetNumberOfProperties(UInt32* numProps) noexcept {
  return E_NOTIMPL;
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
