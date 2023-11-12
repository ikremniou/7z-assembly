# Understanding the 7-zip plugin system
## Before we kick off

In this documentation we will examine the **23.01** version of the 7-Zip plugin system used in File Manager. It may not work with the older versions. So if you would like develop the plugins for the older versions you will not find the solution in this repository.

Once we have all of the declarations in place we can create a mock definitions for the required API. In scope of my research I used the CMake install command to install the plugin `.dll` to the `Formats` directory on the 7-zip root. At the same time I used VSCode launch configuration to launch the the **7-Zip File Manager** to debug and test the sample plugins:
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

## In-place Plugins
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
```
It is evident that macro `REGISTER_ARC_IO` is used to register the implementation that supports `gzip` formats like `gz`, `gzip`, `tgz`, `tpz`, and `apk`. It's worth mentioning that there are multiple `REGISTER_ARC_*` macros that are designed to tune how `Handlers` are registered. We won't go into the details of what all these parameters mean because we won't be creating a `Handler` and registering it inside the 7-Zip source code. Instead, we'll use a separate dynamic library to register and implement `Handlers`. It is also possible to add a handler into the 7-Zip source code, but that would require compiling 7-Zip binaries and shipping a custom version of 7-Zip, which is not acceptable.

In the end, the `REGISTER_ARC_*` macro expands to the definition of the struct and the static variables. These static variables are later aggregated into a collection by invoking the constructor of a static class during the initialization phase. When the 7-Zip File Manager tries to open a file, it searches for the matching handler for that file within the internal collection and uses it to open the archive. For all of this to work, the `REGISTER_ARC_*` macros must be defined in a unique namespace.

## Dynamic Plugins
The dynamic plugins are DLLs that export functions defined in the `Archive2.def` file. It's worth noting that not all of the defined functions are required to be implemented. Usually, 7-Zip will fallback to default values for most of the callbacks. 
- The `Archive2.def` function can be located in [Initial Section](./beginning.md#initial-setup). 
- The function documentation can be found in [Plugin Interface](./plugin-api-def.md).

::: info
In order for dynamic plugins to work, they must be placed inside the `Coders` or `Formats` folders, at the root of the 7-Zip installation, for example `C:\Program Files\7-Zip\Formats\7z-assembly.dll`.
:::