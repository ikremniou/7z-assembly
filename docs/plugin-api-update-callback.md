# IArchiveUpdateCallback

Update callback is used in [IOutArchive::UpdateItems](./plugin-api-outarc.md#updateitems). It defines the following methods:


### GetUpdateItemInfo

```C++
HRESULT GetUpdateItemInfo(
    UInt32 index,
    Int32 *newData,
    Int32 *newProps,
    UInt32 *indexInArchive
)
```

Use this method to get information about updated items.

`[in] index`

The index of the item. Starts from 0 up to the `numOrItems` passed to [IOutArchive::UpdateItems](./plugin-api-outarc.md#updateitems).

`[out] newData`

This flag indicates whether the item contains new data. `0` means that the item does not contain new data. `1` means that the item contains new data.

`[out] newProps`

This flag indicates whether the item contains new properties. `0` means that the item does not contain new properties. `1` means that the item contains new properties.

`[out] indexInArchive`

The index of the item in the archive. If this value in `-1` then item is a new one and should be added to archive, otherwise existing item should be updated.

## GetProperty
```C++
HRESULT GetProperty(
    UInt32 index,
    PROPID propID,
    PROPVARIANT *value
)
```

In case of the `newProps==1` use this method to get properties of the item in the archive.

`[in] index`

The index of the item to update. Starts from 0 up to the `numOrItems` passed to [IOutArchive::UpdateItems](./plugin-api-outarc.md#updateitems).

`[in] propID`

The property ID. See props defined in `ProgID.h` for details.

`[out] value`

The value of the property to set.

## GetStream
```C++
HRESULT GetStream(
    UInt32 index,
    ISequentialInStream **inStream
)
```

In case of `newData==1` use this method to get the stream of the item in the archive.

`[in] index`

The index of the item to update. Starts from 0 up to the `numOrItems` passed to [IOutArchive::UpdateItems](./plugin-api-outarc.md#updateitems).

`[out] inStream`

Input stream that can be used to read the data of the item.

## SetOperationResult
```C++
HRESULT SetOperationResult(
    Int32 operationResult
)
```

Optionally, this method can be used to report back the progress of the update operation.

`[in] operationResult`

Int value used to report progress of the operation.
