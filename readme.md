# How to create a 7-Zip(7z) archiver plugins
This repository is a light documentation of the 7-zip plugin system. We will develop 2 archiver handlers to extend the 7-zip File Manager. Below you can see some of the properties of the archiver:

Criteria | The SZ | The SZE
-------- | ------ | -------
Name | Sample zip archiver | Sample zip archiver extended
Format | Returns two entries. First one is a text file named `sample.txt` with a text `sample`. Seconds is a `child.sz` archive, that creates endless recursion. | Files are encoded into the "archive" using text format: `{fileName}.{fileExtension}-{fileContent}|{fileName}.{fileExtension}-{fileContent}.....`
Extensions | .sz | .sze, .szex

The SZ format will be based on the simples example possible. For SZE we will experiment with the 7-zip flags and archiver parameters and interfaces. All of the code can be found in the `.src` folder.

## Initial setup

To kick off with the plugin creation for 7-Zip you have two option available.
1. *(Easy)* You can copy the content of the `./src/7z-assembly.h` file to you repository and copy the contents of the `./src/plugin/` folder to your `plugin` folder in your repository. Please be aware that `7z-assembly.h` files has declarations only. Do not forget to implement the required declarations. Continue from the next [section](#understanding-the-7-zip-plugin-system).
2. *(Hard)* You can follow the [instructions](#getting-along-with-the-7-zip-source-code) to build up the initial infrastructure by yourself.

### Getting along with the 7-Zip source code

First we need to download the 7-Zip source code. It is located on the [SourceForge](https://sourceforge.net/projects/sevenzip/). Go to the `Files` -> `7-Zip` -> `Version` -> `***-src.7z` and download the archive. After download and extracting the source code we can search for the definition files and find `CPP/7zip/Archive/Archive.def` and `CPP/7zip/Archive/Archive2.def`. It appears that there are multiple versions of the plugins available. We will use the second one as it seems to be specially designed for the new plugins. Here is the content of the `Archive2.def`:

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

We will search for all of the methods defined in the definition file and will copy function signatures to the `7z-assembly.h` file. After copying the definitions we must add the missing includes. I will place this files in the `./src/plugins/` directory to organize everything in one place. In addition to that I will create in `Export.h` inside of the plugins directory to have just one `#include` in the `7z-assembly.h` file. When I copy files from the 7-zip source code archive I update the `#include`'s to reference local files neglecting the source code directory structure. Here is the list of the definitions I have assembled from the 7-zip source code archive:

```cpp
STDAPI_LIB CreateObject(const GUID *clsid, const GUID *iid, void **outObject);
STDAPI_LIB GetHandlerProperty(PROPID propID, PROPVARIANT *value);
STDAPI_LIB GetNumberOfFormats(UINT32 *numFormats);
STDAPI_LIB GetHandlerProperty2(UInt32 formatIndex, PROPID propID, PROPVARIANT *value);
STDAPI_LIB GetIsArc(UInt32 formatIndex, Func_IsArc *isArc);
STDAPI_LIB GetNumberOfMethods(UInt32 *numCodecs);
STDAPI_LIB GetMethodProperty(UInt32 codecIndex, PROPID propID, PROPVARIANT *value);
STDAPI_LIB CreateDecoder(UInt32 index, const GUID *iid, void **outObject);
STDAPI_LIB CreateEncoder(UInt32 index, const GUID *iid, void **outObject);
STDAPI_LIB GetHashers(IHashers **hashers);
STDAPI_LIB SetCodecs(ICompressCodecsInfo *compressCodecsInfo)
STDAPI_LIB SetLargePageMode();
STDAPI_LIB SetCaseSensitive(Int32 caseSensitive)
STDAPI_LIB GetModuleProp(PROPID propID, PROPVARIANT *value);
```

I have defined the `STDAPI_LIB` as `as` to make them exported from the library. I am using the cmake `GENERATE_EXPORT_HEADER` function to define the `LIB_EXPORT` macro.

### How to compile 7-zip components

During my work on this documentation I compiled the File Manager to be able to see call stack and to debug the plugin system. This guide will allow you to compile any components you want to explore.

1. Navigate to the `CPP` folder and find `Build.mak`. This is base make file that is referenced by the other components. Open it.
2. Find `CFLAGS = $(CFLAGS) -nologo` ... line. Add `/Zi` compiler flag at the end (It instructs compiler to create a symbols file).
4. Find `LFLAGS = $(LFLAGS) -nologo -OPT:REF -OPT:ICF -INCREMENTAL:NO` and add `/DEBUG` is linker flag.
5. Find `CFLAGS_O1`, `CFLAGS_O2`, and disable optimization flags `-Ox` with `-Od`
6. Go to any component you want to compile. For example `CPP\7zip\UI\FileManager\`, open Visual Studio console there and type `nmake`. Should work without parameters.
7. Build process will create `x64` folder. Open it and find required component. Copy it to installed 7-zip version. Done.

## Understanding the 7-zip plugin system
### Introduction

In this documentation we will examine the **23.01** version of the 7-Zip plugin system used in File Manager. It may not work with the older versions. So if you would like develop the plugins for the older versions you will not find the solution in this repository.

Once we have all of the declarations in place we can create a mock definitions for the required API. In scope of my research I used the CMake install command to install the plugin `.dll` to the `Formats` directory on the 7-zip root. At the same time I used the VSCode launch configuration to launch the the **7-Zip File Manager** to debug and test the sample plugins:
```json
{
    "name": "Debug 7zFM",
    "type": "cppvsdbg",
    "request": "launch",
    "program": "C:\\Program Files\\7-Zip\\7zFM.exe",
    "cwd": "${fileDirname}",
    "console": "internalConsole"
}
```

This way you can debug the plugin system of the 7-Zip and also test the sample plugins developed in this repository.

### In-place Plugins
The plugins infrastructure plays important role in 7-Zip. Looking through the source code we can see that 7z.dll by itself is a plugin that is loaded by the File Manager or command line interface to provide implementation for supported archive formats. The formats entry points are called `Handlers` and are located under the `CPP/7zip/Archive` folder. For example, this peace of code is from end of the `GzHandler.cpp`:
```cpp
REGISTER_ARC_IO(
  "gzip", "gz gzip tgz tpz apk", "* * .tar .tar .tar", 0xEF,
  k_Signature, 0,
    NArcInfoFlags::kKeepName
  | NArcInfoFlags::kMTime
  | NArcInfoFlags::kMTime_Default
  , TIME_PREC_TO_ARC_FLAGS_MASK (NFileTimeType::kUnix)
  | TIME_PREC_TO_ARC_FLAGS_TIME_DEFAULT (NFileTimeType::kUnix)
  , IsArc_Gz)
}}
```

It is evident that macro `REGISTER_ARC_IO` is used to register the implementation that supports `gzip` formats like  `gz`, `gzip`, `tgz`, `tpz` and `apk`. It worth to mention that there are multiple `REGISTER_ARC_*` that are designed to tune how `Handlers` are registered. We will not go into the details on what all of these parameters mean, because we will not create a `Handler` and register it inside of the 7-Zip source code, but use the separate dynamic library to register and implement `Handlers`. At the same time it is possible to add handler into the 7-Zip source code. However, we will need to compile 7-zip binaries and ship a custom version of the 7-Zip, which is not acceptable.

At the end, the macro `REGISTER_ARC_*` is expanded to the definition of the struct and the static variables to later on aggregate all of the handlers into the collection (by invoking the constructor of the static class on initialization phase). Once 7-Zip File Manager tries to open a file it will search for the matching handler for this file inside of the internal collection and then will use it to open the archive. For all of this to work the `REGISTER_ARC_*` must be defined into the unique namespace.

### Dynamic Plugins
The dynamic plugins are the DLLs that export functions defined in the `Archine2.def`. Looking ahead, not all of the function defined are required to be implemented. Usually 7-Zip will fallback to the default values for the most of the callbacks. The following functions are defined in the `Archine2.def`:

#### `GetModuleProp(PROPID propID, PROPVARIANT* value)*`

1. `propID` - the ID of the property to retrieve. Can be one of the following values:
   - *`kInterfaceType(0)`* - if VT_UI4 `0` is returned than the `IUnknown` inside of your module should not use the virtual destructor, otherwise it does (VT_UI4 `1` returned).
   - *`kVersion(1)`* - the module version as VT_UI4. Used used internally by the plugin host. 
2. `value` - the property variant value to return.

This function is called first to define if dll is compatible with the current version of the 7-Zip and to get the version of the module.

By default the 7-zip will expect that the module will return `0` for `kInterfaceType` on Windows and Linux(starting from `23.01`). The main idea is to have match between the plugin host destruction model and the module. If destination models does not match the plugin is considered not compatible with the plugin host.

> The implementation can be omitted. In this case the version of the plugin will be `0`, and the plugin will be considered compatible with the plugin host.

#### `GetNumberOfMethods(UInt32* numCodecs)`

1. `numCodecs` - the number of codecs to return.

This function is called by the plugin host to get the number of supported codecs. Coders are not used when working with archives in File Manager, hance for FM plugins we can omit implementation.

> The implementation can be omitted. Default index will be set to 0.

#### `GetMethodProperty(UInt32 codecIndex, PROPID propID, PROPVARIANT* value)`

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

> Invoked only if `GetNumberOfMethods` is implemented and returned value that is greater then 0.

#### `GetNumberOfFormats(UINT32 *numFormats)`

1. `numFormats` - the number of archiver formats supported by the plugin.

It is considered that different formats will have different implementations, however, it is possible to have only one implementation for multiple formats, breaking the single responsibility principle. Later on the plugin host will refer to the formats using their index(starting from `0`).

#### `GetHandlerProperty2(UInt32 formatIndex, PROPID propID, PROPVARIANT* value)`

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

#### `CreateObject(const GUID *clsid, const GUID *iid, void **outObject)`

1. `clsid` - the format(handler) class id that is returned by the [GetHandlerProperty](#gethandlerproperty2uint32-formatindex-propid-propid-propvariant-value).
2. `iid` - the id of the interface of the object to create. For example, `IID_IInArchive` - the interface used to **read** the archives. 
3. `outObject` - the COM-way to return the reference to the created object. Assign object reference to `*outObject`.

> [!IMPORTANT]
> You should `AddRef` objects before you return them to the plugin host.

#### `GetHashers(IHashers** hashers)`

1. `hashers` - the plugin should set this argument with the object implementing `IHashers` - the collection of hashers available in plugin. If will be queried on demand.

This function returns the object behind the `IHashers` interface. This interface is used to enumerate `IHasher` instances. The following 3 functions must be defined in order to implement `IHashers`:
1. `HashersImpl::CreateHasher(UInt32 index, IHasher** hasher) noexcept` - creates hasher with specified `index`.
2. `HashersImpl::GetHasherProp(UInt32 codecIndex, PROPID propID, PROPVARIANT* value) noexcept` - gets metadata about the Hasher using its index(`codecIndex`) and metadata property id(`propID`).
3. `HashersImpl::GetNumHashers() noexcept` - returns the number of Hashers supported by the plugin.

> The methods implementation is optional. If none of your formats or coders supports hashing there is no reason to implement it.

#### `SetLargePageMode()`
Called to notify if [Large Page Mode](https://learn.microsoft.com/en-us/windows/win32/memory/large-page-support) is enabled. Can be enabled by the CLI option, or special WinAPI call from plugin host.

> This method is optional to implement.

#### `SetCaseSensitive(Int32 caseSensitive)`

1. `caseSensitive` - the 'bool' argument specifies whether case sensitivity is enabled. If it equals `0` then it is `false`, otherwise it is `true`.

Adjusts the [Case Sensitivity](https://learn.microsoft.com/en-us/windows/wsl/case-sensitivity). It looks like can only be set from the CLI option. Method will be ignored if CLI option is not set.

> This method is optional to implement

#### `SetCodecs(ICompressCodecsInfo* compressCodecsInfo)`

1. `compressCodecsInfo` - the interface to the compression codecs implemented by 7-zip.

Plugin host calls this method to set compression codecs for the plugin. This allows using build-in [In-Place](#in-place-plugins) compression algorithms.

> The method is optional to implement

#### `CreateDecoder(UInt32 index, const GUID* iid, void** outObject)`

TBA

> The method is optional if you developing plugin for File Manager

#### `CreateEncoder(UInt32 index, const GUID* iid, void** outObject);`

TBA

> The method is optional if you developing plugin for File Manager