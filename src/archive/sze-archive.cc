#include "sze-archive.h"
#include <tuple>
#include "../utils.h"

namespace archive {
HRESULT SzeInArchive::Open(IInStream* stream,
                           const UInt64* maxCheckStartPosition,
                           IArchiveOpenCallback* openCallback) noexcept {

  char buffer[255];
  UInt32 current_pos = 1;
  UInt32 extracted = 0;
  auto reader = [&current_pos, &extracted, buffer,
                 stream]() mutable -> std::pair<bool, char> {
    if (current_pos > extracted) {
      current_pos = 0;
      stream->Read(buffer, sizeof(buffer), &extracted);
      if (extracted == 0) {
        return {true, 0};
      }
    }

    return {false, buffer[current_pos++]};
  };

  // validating signature
  auto it = reader();
  if (it.second != 'S') {
    return S_FALSE;
  }

  it = reader();
  if (it.second != 'E') {
    return S_FALSE;
  }

  it = reader();
  while (!it.first) {
    if (it.second == '{') {
      File file;

      it = reader();
      while (it.second != '|') {
        file.path.push_back(it.second);
        it = reader();
      }

      it = reader();
      std::string size_str;
      while (it.second != '}') {
        size_str.push_back(it.second);
        it = reader();
      }

      it = reader();
      auto size = std::atoi(size_str.c_str());
      file.content.resize(size);
      for (auto i = 0; i < size; i++) {
        file.content[i] = it.second;
        it = reader();
      }

      files_.push_back(std::move(file));
    } else {
      it = reader();
    }
  }

  return S_OK;
}

HRESULT SzeInArchive::Close() noexcept {
  return S_OK;
}

HRESULT SzeInArchive::GetNumberOfItems(UInt32* numItems) noexcept {
  *numItems = static_cast<UInt32>(files_.size());
  return S_OK;
}

HRESULT SzeInArchive::GetProperty(UInt32 index, PROPID propID,
                                  PROPVARIANT* value) noexcept {
  switch (propID) {
    case kpidPath:
      return utils::SetVariant(files_[index].path.c_str(), value);
    case kpidIsDir:
      return utils::SetVariant(false, value);
  }
  return S_OK;
}

HRESULT SzeInArchive::Extract(
    const UInt32* indices, UInt32 numItems, Int32 testMode,
    IArchiveExtractCallback* extractCallback) noexcept {
  if (testMode) {
    return S_OK;
  }

  CMyComPtr<ISequentialOutStream> stream;
  while (numItems-- > 0) {
    extractCallback->GetStream(*indices, &stream, 0);

    UInt32 processed;
    stream->Write(files_[*indices].content.data(),
                  static_cast<UInt32>(files_[*indices].content.size()),
                  &processed);
    indices = indices + 1;
  }
  return S_OK;
}

HRESULT SzeInArchive::GetArchiveProperty(PROPID propID,
                                         PROPVARIANT* value) noexcept {
  return S_OK;
}

HRESULT SzeInArchive::GetNumberOfProperties(UInt32* numProps) noexcept {
  *numProps = 0;
  return S_OK;
}

HRESULT SzeInArchive::GetPropertyInfo(UInt32 index, BSTR* name, PROPID* propID,
                                      VARTYPE* varType) noexcept {
  return S_OK;
}

HRESULT SzeInArchive::GetNumberOfArchiveProperties(UInt32* numProps) noexcept {
  return S_OK;
}

HRESULT SzeInArchive::GetArchivePropertyInfo(UInt32 index, BSTR* name,
                                             PROPID* propID,
                                             VARTYPE* varType) noexcept {
  return S_OK;
}
}  // namespace archive