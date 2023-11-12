# Initial setup

To create plugins for 7-Zip, you have two options:

1. *(Easy)* Download the `plugin_headers_vXX.XX` folder from the releases section on GitHub under `Plugin API vXX.X`. Extract this folder into your project and implement the required declarations in the `7z-assembly.h` files. Refer to the [API](./plugin-api-def.md) section for documentation on the declarations implementation.

2. *(Hard)* Build the initial infrastructure yourself by following these steps:
   - Download the 7-Zip source code from [SourceForge](https://sourceforge.net/projects/sevenzip/). Go to `Files` -> `7-Zip` -> `Version` -> `***-src.7z` and extract the archive.
   - Look for the definition files `CPP/7zip/Archive/Archive.def` and `CPP/7zip/Archive/Archive2.def` in the extracted source code. We recommend using `Archive2.def` as it is specifically designed for new plugins.
   - Continue step by step with this documentation to build understanding of the plugins.

Here is the content of `Archive2.def`:

```txt
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

We will search for all of the methods defined in the definition file and will copy function signatures to the `7z-assembly.h` file. After copying the definitions, we must add the missing includes. I will place these files in the `./src/plugins/` directory to organize everything in one place. In addition to that, I will create `Export.h` inside the plugins directory to have just one `#include` in the `7z-assembly.h` file. When I copy files from the 7-zip source code archive, I update the `#include`s to reference local files neglecting the source code directory structure. Here is the list of the definitions I have assembled from the 7-zip source code archive:

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

I have defined the `STDAPI_LIB` as `is` to make them exported from the library. I am using the cmake `GENERATE_EXPORT_HEADER` function to define the `LIB_EXPORT` macro.

