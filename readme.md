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

In this documentation we will examine the most recent version of the 7-Zip plugin system. It may not work with the older versions. So if you would like develop the plugins for the older versions you will not fine the solution in this repository.

Once we have all of the declarations in place we can create a mock definitions for the required API. In scope of my research I used the CMake install command to install the plugin `.dll` to the `Formats` directory on the 7-zip root. At the same time I used the VSCode launch configuration to launch the the **7-Zip File Manager** to debug and test the sample plugins.