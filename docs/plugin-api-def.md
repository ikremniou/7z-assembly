# Plugin Interface

Once again, let's look at the definition of the `Archive2.def` file from [7-zip sources](./beginning.md#getting-along-with-the-7-zip-source-code).

These are the methods that can be implemented by the [Dynamic Plugins](./plugin-system.md#dynamic-plugins). Some of them are not mandatory to implement. It strongly depends on the functionality of your dynamic plugin. Below you can find a description of each function:

## GetModuleProp

```C++
HRESULT GetModuleProp(
    PROPID propID,
    PROPVARIANT* value
)
```

This function is called first to define if the DLL is compatible with the current version of 7-Zip and to get the version of the module.

By default, 7-zip will expect that the module will return `0` for `kInterfaceType` on Windows and Linux (starting from `23.01`). The main idea is to have a match between the plugin host destruction model and the module. If destination models do not match, the plugin is considered not compatible with the plugin host.

::: info
If the implementation is omitted, the version of the plugin will be `0`, and the plugin will be considered compatible with the plugin host.
:::

`[in] propID`

The ID of the property to retrieve. Can be one of the following values:
   - `kInterfaceType(0)`* - if VT_UI4 `0` is returned, then the `IUnknown` inside of your module should not use the virtual destructor, otherwise it does (VT_UI4 `1` returned).
   - *`kVersion(1)`* - the module version as VT_UI4. Used internally by the plugin host. 

`[out] value`

The property variant value to return.

## GetNumberOfMethods
```C++
HRESULT GetNumberOfMethods(
    UInt32* numCodecs
)
```

This function is called by the plugin host to get the number of supported codecs. Coders are not used when working with archives in File Manager, hence for FM plugins, we can omit implementation.

::: info
If the method is not implemented, the default codec index will be set to `0`.
:::

`[out] numCodecs`

The number of codecs to return.

## GetMethodProperty
```C++
HRESULT GetMethodProperty(
    UInt32 codecIndex
    PROPID propID
    PROPVARIANT* value
)
```

This function returns the metadata about a specific archive coder.

::: info
Invoked only if [GetNumberOfMethods](#getnumberofmethods) is implemented and returns a value greater than 0.
:::

`[in] codecIndex`

The index of the codec. This is used to specify the index of the archiver coder. The top index is resolved by [GetNumberOfMethods](#getnumberofmethods).

`[in] propID`

The ID of the property. See props defined in `ProgID.h` for details. Can be one of the following values:
   - *`kID`* - expects the *VT_UI8* as the ID of the archive coder. It is used internally by the plugin host for tasks like checking the version of the "interface". Values like `kName` won't be queried if `kID` is not supported.
   - *`kName`* - expects the *VT_BSTR* as the name of the archiver coder. For example, the name is displayed in the `Options` dialog in File Manager.
   - *`kDecoder`* - expects the binary GUID of the decoder as *VT_BSTR*.
   - *`kEncoder`* - expects the binary GUID of the encoder as *VT_BSTR*.
   - *`kPackStreams`* - the *VT_UI4* number of pack streams (threads) that can be utilized to pack the data.
   - *`kUnpackStreams`* - the *VT_UI4* number of unpack streams (threads) that can be used to unpack the data.
   - *`kDescription`* - the description of the archive coder as *VT_BSTR*.
   - *`kDecoderIsAssigned`* - Whether the decoder is assigned.
   - *`kEncoderIsAssigned`* - Whether the encoder is assigned.
   - *`kDigestSize`* - TBA
   - *`kIsFilter`* - signals the plugin host that the plugin supports filtering data streams. It can be used to pre-process data before compression. For example, if you have a `080204050201` sequence where you know that 0s are always odd, you can filter them using a special `-F` parameter and compress only even numbers. The same filter must be used to restore the original data.

`[out] value`

Returns the method property value.

## GetNumberOfFormats

```C++
HRESULT GetNumberOfFormats(
    UINT32 *numFormats
)
```

It is considered that different formats will have different implementations. However, it is possible to have only one implementation for multiple formats, breaking the single responsibility principle. Later on, the plugin host will refer to the formats using their index (starting from `0`).

`[in] numFormats`

The number of archiver formats supported by the plugin.

## GetHandlerProperty2
```C++
HRESULT GetHandlerProperty2(
    UInt32 formatIndex,
    PROPID propID,
    PROPVARIANT* value
)
```

This function returns the metadata about the archive format(handler).

`[in] formatIndex`

The index of the archiver format. 

`[in] propID`

Represents the ID of the property to retrieve. See props defined in `ProgID.h` for details. It can be one of the following values:
  - *`kName`* - expects the name of the Handler as VT_BSTR
  - *`kClassID`* - expects the class id of Handler as VT_BSTR binary GUID
  - *`kExtension`* - expect the extension as VT_BSTR, that will be associated with the Handler.
  - *`kAddExtension`* - allows to specify additional extensions supported by the Handler, separated by space symbol(` `)
  - *`kUpdate`* - return variant bool `true` if it is possible to update the archive, otherwise `false`;
  - *`kKeepName`* - TBA
  - *`kSignature`* - if the archive has a [signature](https://en.wikipedia.org/wiki/List_of_file_signatures), it will be verified by the plugin host before invoking the plugin's logic. The binary VT_BSTR is expected in return. 
  - *`kkMultiSignature`* - if the archive has multiple signatures, it expects VT_BSTR here. The first byte should be the length of the sub-signature, then the signature with the specified length follows, and so on.
  - *`kSignatureOffset`* - The offset of the file signature in bytes.
  - *`kAltStreams`* - assign if alternative streams are supported.
  - *`kNtSecure`* - TBA
  - *`kFlags`* - Flags that define the capabilities that are implemented by the handler. **TBA**
  - *`kTimeFlags`* - TBA

`[out] value`

Represents the property value to set.

## CreateObject
```C++
HRESULT CreateObject(
    const GUID *clsid,
    const GUID *iid,
    void **outObject
)
```

Creates plugin objects(formats, handlers, coders, etc.) using their class id and interface id.

::: warning IMPORTANT
You should `AddRef` objects before you return them to the plugin host.
:::

`[in] clsid`

The object class id that is returned by the [GetHandlerProperty2](#gethandlerproperty2).

`[in] iid`

The id of the interface of the object to create. For example, `IID_IInArchive` - the interface used to **read** archives. 

`[out] outObject`

The COM-way to return the reference to the created object. Assign the object reference to `*outObject`.


## GetHashers
```C++
HRESULT GetHashers(
    IHashers** hashers
)
```

This function returns the object behind the `IHashers` interface. This interface is used to enumerate `IHasher` instances.

::: info
If none of your formats or coders supports hashing, there is no reason to implement it.
:::

`[out] hashers`

The plugin should set this argument with the object implementing `IHashers` - the collection of hashers available in the plugin. It will be queried on demand.

## SetLargePageMode

```C++
HRESULT SetLargePageMode()
```

Called to notify if [Large Page Mode](https://learn.microsoft.com/en-us/windows/win32/memory/large-page-support) is enabled. It can be enabled by the CLI option or a special WinAPI call from the plugin host.

##SetCaseSensitive

```C++
HRESULT SetCaseSensitive(
    Int32 caseSensitive
)
```

Adjusts the [Case Sensitivity](https://learn.microsoft.com/en-us/windows/wsl/case-sensitivity). It looks like it can only be set from the CLI option. The method will be ignored if the CLI option is not set.

`[in] caseSensitive` 

The 'bool' argument specifies whether case sensitivity is enabled. If it equals `0`, then it is `false`; otherwise, it is `true`.

## SetCodecs
```C++
HRESULT SetCodecs(
    ICompressCodecsInfo* compressCodecsInfo
)
```

The plugin host calls this method to set compression codecs for the plugin. This allows using built-in [In-Place](./plugin-system.md#in-place-plugins) compression algorithms.

`[in] compressCodecsInfo`

The interface to the compression codecs implemented by 7-zip.

## GetIsArc
```C++
typedef UInt32(WINAPI* Func_IsArc)(const Byte* p, size_t size);

HRESULT GetIsArc(
    UInt32 formatIndex,
    Func_IsArc *isArc
)
```

`[in] formatIndex`

The format index of the archive to assign `isArc` function.

`[out] isArc` 

The function that will be called to check if the archive is supported by the plugin. The function will accept a raw sequence of bytes. The return type must be `true` or `false`.
