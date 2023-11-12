# IInArchive

### Open
```C++
HRESULT Open(
    IInStream *stream,
    const UInt64 *maxCheckStartPosition,
    IArchiveOpenCallback *openCallback
);
```

Function invoked when plugin host will find the fitting archive to open. Usually it is not mandatory to read all archive in memory. Most archives read only head(table) section of the archive. This gives basic understanding of the properties of the archive that can be queried later on.

`[in] stream`

The file stream that was opened upon reading the archive file.

`[in] maxCheckStartPosition`

If `NULL`, the handler can try to search archive start in stream.
If `*0`, the handler must check only current position as archive start

`[in] openCallback`

The callback to report the progress of opening the archive.

### Close
```C++
Close();
```

Function called to close the archive. Dispose the resources allocated in `Open` function.


### GetNumberOfItems 
```C++
GetNumberOfItems(
    UInt32 *numItems
);
```

`[out] numItems`

Return the number of items(files, dirs) stored in archive. The index of the item will be used later to extract it or to get metadata about it.

### GetProperty
```C++
GetProperty(
    UInt32 index,
    PROPID propID,
    PROPVARIANT *value
);
```

This function must return the property of the particular item. Item is indexed using `GetNumberOfItems` function. The property ids can be located in `ProgID.h` file.

### Extract
```C++
Extract(
    const UInt32 *indices,
    UInt32 numItems,
    Int32 testMode,
    IArchiveExtractCallback *extractCallback
);
```

Extract the items indexed by `indices`. Use extract callback to query for the stream to write to.

`[in] indices`

The array of the items to extract.

`[in] numItems`

The size of the `indices` array.

`[in] testMode`

Indicates that archive must test the ability to extract the given items, without doing so.

`[in] extractCallback`

The callback to report the progress of extracting the items. Also, this callback can be used to query for the stream to write to.

### GetArchiveProperty
```C++
GetArchiveProperty(
    PROPID propID,
    PROPVARIANT *value
);
```

Gets general archive properties like `PhysicalSize` or `Readonly` or error properties if to check if there was any error during `Open`.

`[in] propID`

The id of the property to query. Properties enum can be found in `ProgID.h`.

`[out] value`

The value to the property to required to return.

### GetNumberOfProperties
```C++
GetNumberOfProperties(
    UInt32 *numProps
);
```

Gets the number of properties in the archive.

`[out] numProps`

Return the number of the properties that will later on by queried using `GetPropertyInfo`. 

::: tip TIP 1
If you return `0` rest 3 functions can safely return `E_NOTIMPL`.
:::

::: tip TIP 2
It is easy to confuse the `GetPropertyInfo` and `GetProperty` functions. So, `GetNumberOfProperties` is used for `GetPropertyInfo`, and `GetPropertyInfo` is used for the *display*(like giving names to the column in FileManager) and `GetProperty` is used by the plugin host to correctly work with files depending of their metadata.
:::

### GetPropertyInfo
```C++
GetPropertyInfo(
    UInt32 index,
    BSTR *name,
    PROPID *propID,
    VARTYPE *varType
);
```

Assigns names to the properties of the archive items.

`[in] index`

The index of the property to get info about.

`[out] name`

Assign the name of the property that will be displayed.

`[out] propID`

The corresponding property id from `GetProperty` to link with the given name.

`[out] varType`

The variant type(short) of the property.

### GetNumberOfArchiveProperties 
```C++
GetNumberOfArchiveProperties(
    UInt32 *numProps
);
```

`[out] numProps`

Return the number of the names properties you want display for the archive.

### GetArchivePropertyInfo
```C++
GetArchivePropertyInfo(
    PROPID propID,
    BSTR *name,
    PROPID *propID,
    VARTYPE *varType
);
```

Assign the name to the archive properties.

`[in] propID`

The property id of the archive, indexed with the help of `GetNumberOfArchiveProperties`

`[out] name`

The name to assign to the archive property.

`[out] propID`

The property id from `GetArchiveProperty` to associate with the names property.

`[out] varType`

The variant type(short) of the property.
