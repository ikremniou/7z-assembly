# IArchiveOpenCallback

Use this interface to report back the progress of opening the archive. See [IInArchive::Open](./plugin-api-inarc.md#open). It defines the following methods:

### SetTotal
```C++
HRESULT SetTotal(
    UInt64 numItems
);
```

Use this method to set the total number of 'steps' to be processed. It can be the total number of items in the archive or any other value.

`[in] numItems`

The total number of 'steps' to process.

### SetCompleted
```C++
HRESULT SetCompleted(
    const UInt64 *completeValue
);
```

Use this method to increment the number of already processed 'steps'.

`[in] completeValue`

The number of already processed 'steps'.

