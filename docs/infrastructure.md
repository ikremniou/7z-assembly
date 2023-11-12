# Infrastructure

Here we will review initial infrastructure to create handlers for `sz` and `sze` sample archives.

TBA

And finally plugin host will create InArchive object using [CreateObject](./plugin-api-def.md#createobject-const-guid-clsid-const-guid-iid-void-outobject) method.

``` C++
STDAPI_LIB CreateObject(const GUID* clsid, const GUID* iid, void** outObject) {
  if (*clsid == SzHandlerGuid && *iid == IID_IInArchive) {
    *outObject = new archive::SzInArchive();
  }

  static_cast<IUnknown*>(*outObject)->AddRef();
  return S_OK;
}
```