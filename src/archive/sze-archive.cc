#include "sze-archive.h"
#include <iostream>
#include <tuple>
#include "../utils.h"
#include "sze-reader.h"

namespace archive {

HRESULT SzeInArchive::Open(IInStream* stream,
                           const UInt64* maxCheckStartPosition,
                           IArchiveOpenCallback* openCallback) noexcept {
  ArchiveReader archive_reader(stream);
  auto curr_it = archive_reader.begin();
  auto next = [](ArchiveReader::Iterator& it) -> ArchiveReader::Iterator& {
    return ++it;
  };

  // validate signature
  if (*curr_it != 'S' && *next(curr_it) != 'Z') {
    return S_FALSE;
  }

  while (curr_it != archive_reader.end()) {
    if (*curr_it == '{') {
      File file;

      // read item path
      while (*next(curr_it) != '|') {
        file.path.push_back(*curr_it);
      }

      // read item size
      std::string size_str;
      while (*next(curr_it) != '}') {
        size_str.push_back(*curr_it);
      }

      // read item content
      auto size = std::atoi(size_str.c_str());
      file.content.resize(size);
      for (auto i = 0; i < size; i++) {
        file.content[i] = *next(curr_it);
      }

      items_.push_back(std::move(file));
    }

    next(curr_it);
  }

  return S_OK;
}

HRESULT SzeInArchive::Close() noexcept {
  return S_OK;
}

HRESULT SzeInArchive::GetNumberOfItems(UInt32* numItems) noexcept {
  *numItems = static_cast<UInt32>(items_.size());
  return S_OK;
}

HRESULT SzeInArchive::GetProperty(UInt32 index, PROPID propID,
                                  PROPVARIANT* value) noexcept {
  switch (propID) {
    case kpidPath:
      return utils::SetVariant(items_[index].path.c_str(), value);
    case kpidIsDir:
      return utils::SetVariant(false, value);
    case kpidSize:
      return utils::SetVariant(items_[index].content.size(), value);
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
    stream->Write(items_[*indices].content.data(),
                  static_cast<UInt32>(items_[*indices].content.size()),
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
  *numProps = 1;
  return S_OK;
}

HRESULT SzeInArchive::GetPropertyInfo(UInt32 index, BSTR* name, PROPID* propID,
                                      VARTYPE* varType) noexcept {

  *name = SysAllocString(L"Size");
  *propID = kpidSize;
  *varType = VT_UI8;
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

HRESULT SzeInArchive::UpdateItems(
    ISequentialOutStream* outStream, UInt32 numItems,
    IArchiveUpdateCallback* updateCallback) noexcept {
  for (UInt32 i = 0; i < numItems; i++) {
    Int32 newData;
    Int32 newProps;
    UInt32 indexInArchive;
    HRESULT res = updateCallback->GetUpdateItemInfo(i, &newData, &newProps, &indexInArchive);
    if (newData == 0 && newProps == 0) {
        continue;
    }

    CMyComPtr<ISequentialInStream> in_stream;
    res = updateCallback->GetStream(i, &in_stream);
    if (FAILED(res)) {
        continue;
    }

    File file{};
    if (items_.size() <= i) {
        items_.push_back(file);
    }

    ArchiveReader reader(in_stream);
    if (newData) {
        for(byte b : reader) {
            items_[i].content.push_back(b);
        }
    }

    if (newProps) {
        PROPVARIANT variant_path;
        updateCallback->GetProperty(i, kpidPath, &variant_path);
        items_[i].path = std::wstring(variant_path.bstrVal);
    }

  }
  return S_OK;
}

HRESULT SzeInArchive::GetFileTimeType(UInt32* type) noexcept {
  *type = NFileTimeType::EEnum::kNotDefined;
  return S_OK;
}
}  // namespace archive