# Plugin API

Once again let's look at the definition of the `Archive2.def` file from 7-zip sources:
```def
EXPORTS
  CreateObject PRIVATE
  GetHandlerProperty PRIVATE
  GetNumberOfFormats PRIVATE
  GetHandlerProperty2 PRIVATE
  GetIsArc PRIVATE
  GetNumberOfMethods PRIVATE
  GetMethodProperty PRIVATE
  CreateDecoder PRIVATE
  CreateEncoder PRIVATE
  GetHashers PRIVATE
  SetCodecs PRIVATE
  SetLargePageMode PRIVATE
  SetCaseSensitive PRIVATE
  GetModuleProp PRIVATE
```

This are the methods that can be implemented by the dynamic plugin. Some of them are not mandatory to implement. It strongly depends on the functionality of your dynamic plugin.

::: info
Methods marked with `*` are optional to implement.
:::

### `GetModuleProp(PROPID propID, PROPVARIANT* value)`*

1. `propID` - the ID of the property to retrieve. Can be one of the following values:
   - *`kInterfaceType(0)`* - if VT_UI4 `0` is returned than the `IUnknown` inside of your module should not use the virtual destructor, otherwise it does (VT_UI4 `1` returned).
   - *`kVersion(1)`* - the module version as VT_UI4. Used used internally by the plugin host. 
2. `value` - the property variant value to return.

This function is called first to define if dll is compatible with the current version of the 7-Zip and to get the version of the module.

By default the 7-zip will expect that the module will return `0` for `kInterfaceType` on Windows and Linux(starting from `23.01`). The main idea is to have match between the plugin host destruction model and the module. If destination models does not match the plugin is considered not compatible with the plugin host.

::: info
If implementation is omitted the version of the plugin will be `0`, and the plugin will be considered compatible with the plugin host.
:::

### `GetNumberOfMethods(UInt32* numCodecs)`*

1. `numCodecs` - the number of codecs to return.

This function is called by the plugin host to get the number of supported codecs. Coders are not used when working with archives in File Manager, hance for FM plugins we can omit implementation.

::: info
If method is not implemented, the default codec index will be set to `0`.
:::
### `GetMethodProperty(UInt32 codecIndex, PROPID propID, PROPVARIANT* value)`*

1. *`codecIndex`* - is used to specify the index of the archiver coder. Top index is resolved by [`GetNumberOfMethods`](#getnumberofmethodsuint32-numcodecs).
2. *`propID`* - is the one of the following values:
   - *`kID`*(0) - expect the *VT_UI8* as id of the archive coder(It used internally by the plugin host like checking the version of the "interface" where values like `kName` wont be queried if `kID` is not supported)
   - *`kName`*(1) - expect the *VT_BSTR* as name of the archiver coder. For example, name is displayed in the `Options` dialog in File Manager.
   - *`kDecoder`*(2) - expects the binary GUID of the decoder as *VT_BSTR*.
   - *`kEncoder`*(3) - expects the binary GUID of the encoder as *VT_BSTR*.
   - *`kPackStreams`*(4) - the *VT_UI4* number of pack streams(threads) that can be utilized to pack the data.
   - *`kUnpackStreams`*(5) - the *VT_UI4* number of unpack streams(threads) that can be used to unpack the data.
   - *`kDescription`*(6) - the description of the archive coder as *VT_BSTR*.
   - *`kDecoderIsAssigned`*(7) - TBA
   - *`kEncoderIsAssigned`*(8) - TBA
   - *`kDigestSize`*(9) - TBA
   - *`kIsFilter`*(10) - signals the plugin host that plugin supports filtering data streams. It can be used to pre-process data before compression. For example, you have `080204050201` sequence where you know that 0-s are always odd. You can filter them using special `-F` parameter and compress only even numbers. Same filter must be used to restore the original data.

3. Third parameter is used to report back the metadata about archive coder.

This function returns the metadata about specific archive coder.

::: info
Invoked only if `GetNumberOfMethods` is implemented and returned value that is greater then 0.
:::
### `GetNumberOfFormats(UINT32 *numFormats)`

1. `numFormats` - the number of archiver formats supported by the plugin.

It is considered that different formats will have different implementations, however, it is possible to have only one implementation for multiple formats, breaking the single responsibility principle. Later on the plugin host will refer to the formats using their index(starting from `0`).

### `GetHandlerProperty2(UInt32 formatIndex, PROPID propID, PROPVARIANT* value)`

1. `formatIndex` - the index of the format 
1. `propID` - represents the ID of the property to retrieve. It can be one of the following values:
    - *`kName`* - expects the name of the Handler as VT_BSTR
    - *`kClassID`* - expects the class id of Handler as VT_BSTR binary GUID
    - *`kExtension`* - expect the extension as VT_BSTR, that will be associated with the Handler.
    - *`kAddExtension`* - allows to specify additional extensions supported by the Handler, separated by space symbol(` `)
    - *`kUpdate`* - return variant bool `true` if it is possible to update the archive, otherwise `false`;
    - *`kKeepName`* - TBA
    - *`kSignature`* - if archive has a [signature](https://en.wikipedia.org/wiki/List_of_file_signatures), it will be verified by the plugin host before invoking plugins logic. The binary VT_BSTR is expected in return. 
    - *`kkMultiSignature`* - if archive has multiple signatures it expects VT_BSTR here. First byte should be the length of the sub-signature, then signature with specified length follows, and so on.
    - *`kSignatureOffset`* - The offset of the file signature in bytes.
    - *`kAltStreams`* - TBA
    - *`kNtSecure`* - TBA
    - *`kFlags`* - Flags that define the capabilities that are implemented by the handler. **TBA**
    - *`kTimeFlags`* - TBA
2. `value` - represents the property value to set.

This function returns the metadata about archive format(handler).

### `CreateObject(const GUID *clsid, const GUID *iid, void **outObject)`

1. `clsid` - the format(handler) class id that is returned by the [GetHandlerProperty](#gethandlerproperty2uint32-formatindex-propid-propid-propvariant-value).
2. `iid` - the id of the interface of the object to create. For example, `IID_IInArchive` - the interface used to **read** the archives. 
3. `outObject` - the COM-way to return the reference to the created object. Assign object reference to `*outObject`.

::: warning IMPORTANT
You should `AddRef` objects before you return them to the plugin host.
:::

### `GetHashers(IHashers** hashers)`*

1. `hashers` - the plugin should set this argument with the object implementing `IHashers` - the collection of hashers available in plugin. If will be queried on demand.

This function returns the object behind the `IHashers` interface. This interface is used to enumerate `IHasher` instances. The following 3 functions must be defined in order to implement `IHashers`:
1. `HashersImpl::CreateHasher(UInt32 index, IHasher** hasher) noexcept` - creates hasher with specified `index`.
2. `HashersImpl::GetHasherProp(UInt32 codecIndex, PROPID propID, PROPVARIANT* value) noexcept` - gets metadata about the Hasher using its index(`codecIndex`) and metadata property id(`propID`).
3. `HashersImpl::GetNumHashers() noexcept` - returns the number of Hashers supported by the plugin.

::: info
If none of your formats or coders supports hashing there is no reason to implement it.
:::
### `SetLargePageMode()`*
Called to notify if [Large Page Mode](https://learn.microsoft.com/en-us/windows/win32/memory/large-page-support) is enabled. Can be enabled by the CLI option, or special WinAPI call from plugin host.

### `SetCaseSensitive(Int32 caseSensitive)`*

1. `caseSensitive` - the 'bool' argument specifies whether case sensitivity is enabled. If it equals `0` then it is `false`, otherwise it is `true`.

Adjusts the [Case Sensitivity](https://learn.microsoft.com/en-us/windows/wsl/case-sensitivity). It looks like can only be set from the CLI option. Method will be ignored if CLI option is not set.

### `SetCodecs(ICompressCodecsInfo* compressCodecsInfo)`*

1. `compressCodecsInfo` - the interface to the compression codecs implemented by 7-zip.

Plugin host calls this method to set compression codecs for the plugin. This allows using build-in [In-Place](#in-place-plugins) compression algorithms.

### `CreateDecoder(UInt32 index, const GUID* iid, void** outObject)`*
::: info
The method is optional if you developing plugin for File Manager
:::

### `CreateEncoder(UInt32 index, const GUID* iid, void** outObject);`*
::: info
The method is optional if you developing plugin for File Manager
:::