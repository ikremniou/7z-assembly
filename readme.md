# How to create a 7-Zip(7z) archiver plugins
This repository is a light documentation of the 7-zip plugin system. It includes two examples of the archiver plugins:

Criteria | The SZ | The SZE
-------- | ------ | -------
Name | Sample zip archiver | Sample zip archiver extended
Format | Returns two entries. First one is a text file named `sample.txt` with a text `sample`. Seconds is a `child.sz` archive, that creates endless recursion. | Files are encoded into the "archive" using text format: `{fileName}.{fileExtension}-{fileContent}|{fileName}.{fileExtension}-{fileContent}.....`
Extensions | .sz | .sze, .szex

All of the code can be found in the `.src` folder.

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

## Understanding the 7-zip plugin system
### Introduction

In this documentation we will examine the most recent version of the 7-Zip plugin system. It may not work with the older versions. So if you would like develop the plugins for the older versions you will not fine the solution in this repository.

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
The plugins infrastructure plays important role in 7-Zip. Looking through the source code we can see that 7z.dll by itself is a plugin that is loaded by the File Manager or command line interface to provide implementation for supported archive formats. The formats entry points are called `Handlers` and area located under the `CPP/7zip/Archive` folder. For example, here is peace of code from end of the `GzHandler.cpp`:
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

At the end the macro `REGISTER_ARC_*` is expanded to the definition of the struct and the static variables to later on aggregate all of the handlers into the collection (by invoking the constructor of the static class on initialization phase). Once 7-Zip File Manager tries to open a file it will search for the matching handler for this file inside of the internal collection and then will use it to open the archive. For all of this to work the `REGISTER_ARC_*` must be defined into the unique namespace.

### Dynamic Plugins
The dynamic plugins are the DLLs that export functions defined in the `Archine2.def`. Looking ahead, not all of the function defined are required to be implemented. Usually 7-Zip will fallback to the default values for the most of the callbacks. The following functions are defined in the `Archine2.def`:

#### `GetModuleProp(PROPID propID, PROPVARIANT* value)`
> This function is optional and can be omitted by the plugin implementer.

This function is called first to define if dll is compatible with the current version of the 7-Zip and to get the version of the module. In has two `PROPID` values:
1. `progID = kInterfaceType = 0`, which expects in return `0` or `1` of unsigned 4 bytes int `VT_UI4` type. 
   - `0` means that the `IUnknown` inside of your module should not use the virtual destructor. 
   - `1` means that `IUnknown` inside of your module is using the virtual destructor.
   By default the 7-zip will expect that the module will return `0` on Windows and Linux(starting from `v23`). The main idea is to have match between the plugin host destruction model and the module. If destination models does not match the plugin is considered not compatible with the plugin host.
2. `progID = kVersion = 1`, which expects in return the version of the module as `VT_UI4`. The usage of this number inside of the plugin host is not clearly defined.

The implementation can be omitted. In this case the version of the plugin will be `0`, and the plugin will be considered compatible with the plugin host.

#### `GetNumberOfMethods(UInt32* numCodecs)`
> This function is optional and can be omitted by the plugin implementer.

This function is called by the plugin host to get the number supported archive formats. It is considered that different formats will have different implementations, however, it is possible to have only one implementation for multiple formats, breaking the single responsibility principle. In our case we should return `2` as we have two archive types `sz` and `sze`. The later on the plugin host will refer to the formats using its number. For example, to get information, metadata about first archive format (`sz`) it will send its index(`1`) to the functions that will follow.

I will use the 7-Zip terminology and can refer to the "archive format" as `Coder` later on in this documentation.

The implementation can be omitted. By default the number of methods(coders) is set to `1`.