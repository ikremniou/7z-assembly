# Understanding the 7-zip plugin system
## Before we kick off

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

It is evident that macro `REGISTER_ARC_IO` is used to register the implementation that supports `gzip` formats like  `gz`, `gzip`, `tgz`, `tpz` and `apk`. It worth to mention that there are multiple `REGISTER_ARC_*` that are designed to tune how `Handlers` are registered. We will not go into the details on what all of these parameters mean, because we will not create a `Handler` and register it inside of the 7-Zip source code, but use the separate dynamic library to register and implement `Handlers`. At the same time it is possible to add handler into the 7-Zip source code. However, we will need to compile 7-zip binaries and ship a custom version of the 7-Zip, which is not acceptable.

At the end, the macro `REGISTER_ARC_*` is expanded to the definition of the struct and the static variables to later on aggregate all of the handlers into the collection (by invoking the constructor of the static class on initialization phase). Once 7-Zip File Manager tries to open a file it will search for the matching handler for this file inside of the internal collection and then will use it to open the archive. For all of this to work the `REGISTER_ARC_*` must be defined into the unique namespace.

## Dynamic Plugins
The dynamic plugins are the DLLs that export functions defined in the `Archine2.def`. Looking ahead, not all of the function defined are required to be implemented. Usually 7-Zip will fallback to the default values for the most of the callbacks. The following functions are defined in the `Archine2.def`: