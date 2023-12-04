# Implementation of the SZ Archive

The implementation of the SZ sample archive can be found in [sz-archive.h](https://github.com/ikremniou/7z-assembly/blob/master/src/archive/sz-archive.h) and [sz-archive.cc](https://github.com/ikremniou/7z-assembly/blob/master/src/archive/sz-archive.cc). First we define the `SzInArchive` that will be created by the [CreateObject](./plugin-api-def.md#createobject) function. Now, let's implement SZ sample archive. 

### Definition of the SzInArchive

<<< ../src/archive/sz-archive.h#snippet{C++}

We implement the following interfaces:
1. The `IInArchive` interface.
2. The `IUnknown` interface.

I used the macro `Z7_IFACES_IMP_UNK_1` to create a declarations for `IUnknown` and `IInArchive` interfaces, and inherited from `CMyUnknownImpl` to provide definitions for `IUnknown`.


## Definition of the virtual files.

<<< ../src/archive/sz-archive.cc#files_def{C++}

## [IInArchive::Open](./plugin-api-inarc.md#open)

Opens archive and validates signature.

<<< ../src/archive/sz-archive.cc#open{C++}

## [IInArchive::GetNumberOfItems](./plugin-api-inarc.md#getnumberofitems)

Returns number of files in archive.

<<< ../src/archive/sz-archive.cc#get_number_of_items{C++}

## [IInArchive::GetProperty](./plugin-api-inarc.md#getproperty) 

Returns the file properties.

<<< ../src/archive/sz-archive.cc#get_property{C++}

All of the above properties are queried from by the plugin host with current setup. Most of them are defaulted or ignored by returning `S_OK` and not touching the `value` variant.

## [IInArchive::Extract](./plugin-api-inarc.md#extract) 

Extracts files.

<<< ../src/archive/sz-archive.cc#extract{C++}

## [IInArchive::GetArchiveProperty](./plugin-api-inarc.md#getarchiveproperty)

Returns the properties of the archive file.

<<< ../src/archive/sz-archive.cc#get_archive_property{C++}

All of the above properties are queried by the plugin host with current setup. Pay attention that we mark archive as **readonly**.

## [IInArchive::GetNumberOfProperties](./plugin-api-inarc.md#getnumberofproperties) 

Returns 0.

<<< ../src/archive/sz-archive.cc#get_number_of_properties{C++}

We return 0 to indicate that we do not display properties of the files **inside** of the archive.

::: tip
Rest of the `IInArchive` methods can safely return `S_OK` right away.
:::