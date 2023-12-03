# IArchiveExtractCallback

This interface is used to extract items from the archive. See [IInArchive::Extract](./plugin-api-inarc.md#extract). It defines the following methods:

## GetStream
```C++
HRESULT GetStream(
    UInt32 index,
    ISequentialOutStream **outStream,
    Int32 askExtractMode
)
```

Use this method to get output steam for the item.

`[in] index`

The index of the item. Starts from 0 up to the `numItems` passed to [IInArchive::Extract](./plugin-api-inarc.md#extract).

`[out] outStream`

The output stream to write item data to.

`[in] askExtractMode`

If `askMode != NArchive::NExtract::NAskMode::kExtract` the callee is not allowed to write data to the stream.

## PrepareOperation
```C++
HRESULT PrepareOperation(
    Int32 askExtractMode
)
```

Should be called at the beginning of the extract of each item.

`[in] askExtractMode`

Should be one of the `NArchive::NExtract::NAskMode::kTest` if `testMode` is true and `NArchive::NExtract::NAskMode::kExtract` otherwise. 

## SetOperationResult
```C++
HRESULT SetOperationResult(
    Int32 opRes
)
```

Should be called at the end of the extraction of each item.

`[in] opRes`

The operation result from `NArchive::NExtract::NOperationResult` enum.