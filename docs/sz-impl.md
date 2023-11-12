# Implementation of the SZ Archive

The implementation of the SZ sample archive can be found in [sz-archive.h](https://github.com/ikremniou/7z-assembly/blob/master/src/archive/sz-archive.h) and [sz-archive.cc](https://github.com/ikremniou/7z-assembly/blob/master/src/archive/sz-archive.cc). First we define the `SzInArchive` that will be created by the [CreateObject](./plugin-api-def.md#createobject-const-guid-clsid-const-guid-iid-void-outobject) function. Now, let's implement SZ sample archive. 

### Definition of the SzInArchive

```C++
class SzInArchive : public CMyUnknownImp, public IInArchive {
public:
  Z7_IFACES_IMP_UNK_1(IInArchive);
};
```
We implement the following interfaces:
1. The `IInArchive` interface.
2. The `IUnknown` interface.

I used the macro `Z7_IFACES_IMP_UNK_1` to create a declarations for `IUnknown` and `IInArchive` interfaces, and inherited from `CMyUnknownImpl` to provide definitions for `IUnknown`.


### Definition of the virtual files.

```C++
struct File {
  const wchar_t* path;
  bool is_dir;
  const char* content;
};

std::array<File, 5> files = {
  {
    {L"sample.txt", false, "sample"},
    {L"sample2.txt", false, "sample2"},
    {L"someDir", true, nullptr},
    {L"someDir/sample3.txt", false, "sample3"},
    {L"child.sz", false, "any"},
  }
};
```

### `IInArchive::Open` - opens archive and validates signature.

```C++
HRESULT SzInArchive::Open(IInStream* stream,
                          const UInt64* maxCheckStartPosition,
                          IArchiveOpenCallback* openCallback) noexcept {
  char buffer[8];
  UInt32 processed = 0;
  stream->Read(buffer, sizeof(buffer), &processed);
  if (buffer[0] != 'S' && buffer[0] != 'Z') {
    return S_FALSE;
  }
  return S_OK;
}
```

### `IInArchive::GetNumberOfItems` - returns number of files in archive.

```C++
HRESULT SzInArchive::GetNumberOfItems(UInt32* numItems) noexcept {
  *numItems = static_cast<UInt32>(std::size(files));
  return S_OK;
}
```

### `IInArchive::GetProperty` - returns the file properties.

```C++
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
        return S_OK;
    }
  }
}
```

All of the above properties are queried from by the plugin host with current setup. Most of them are defaulted or ignored by returning `S_OK` and not touching the `value` variant.

### `IInArchive::Extract` - extracts files.

```C++
HRESULT SzInArchive::Extract(
    const UInt32* indices, UInt32 numItems, Int32 testMode,
    IArchiveExtractCallback* extractCallback) noexcept {
  CMyComPtr<ISequentialOutStream> outStream;
  while (numItems-- > 0) {
    RINOK(extractCallback->GetStream(*indices, &outStream, 0));
    UInt32 size_processed;
    const char* content = files[*indices].content;
    if (content) {
      outStream->Write(content, static_cast<UInt32>(std::strlen(content)),
                       &size_processed);
    }
    indices = indices + 1;
  }

  return S_OK;
}
```


### `IInArchive::GetArchiveProperty` - returns the properties of the archive file.

```C++
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
      return utils::SetVariant(false, value);
    case kpidReadOnly:
      return utils::SetVariant(true, value);

    case kpidMainSubfile:
      return utils::SetVariant(0u, value);

    default:
      return E_NOTIMPL;
  }
}
```
All of the above properties are queried by the plugin host with current setup. Pay attention that we mark archive as **readonly**.

### `IInArchive::GetNumberOfProperties` - returns 0.

```C++
HRESULT SzInArchive::GetNumberOfProperties(UInt32* numProps) noexcept {
  *numProps = 0;
  return S_OK;
}
```
We return 0 to indicate that we do not display properties of the files **inside** of the archive.

::: tip
Rest of the `IInArchive` methods can safely return `S_OK` right away.
:::