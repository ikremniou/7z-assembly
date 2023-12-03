# IOutArchive

`IOutArchive` allows archives to be modified. It should used in conjunction with `IInArchive`.

### GetFileTimeType

```C++
HRESULT SzeInArchive::GetFileTimeType(
    UInt32* type
) noexcept;
```

Called first to determine the file time type, as it can be different depending on the OS. 

:::tip
Starting from **22.00** can return NFileTimeType::EEnum::kNotDefined as any file time type is supported.
:::

`[out] type`

The file time type from `NFileTimeType::EEnum` enum.

### UpdateItems

``` C++
HRESULT IOutArchive::UpdateItems(
    ISequentialOutStream* outStream,
    UInt32 numItems,
    IArchiveUpdateCallback* updateCallback
) noexcept;
```

Updates items in the archive. The plugin must write modified archive to the `outStream`.

`[in] outStream`

The output stream to write modified archive to.

`[in] numItems`

Number of items to update.

`[in] updateCallback`

Update callback is used to get the content and properties of the modified items. See [IArchiveUpdateCallback](./plugin-api-update-callback.md).
