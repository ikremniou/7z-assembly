# Infrastructure

Here we will review initial infrastructure to create handlers for `sz` and `sze` sample archives. If you need additional info see [7z-assembly.h](https://github.com/ikremniou/7z-assembly/blob/master/src/7z-assembly.cc)

## 1. Define archive handlers

<<< ../src/7z-assembly.cc#archive_handlers{C++}

Later on we define SZ and SZE handlers with corresponding GUIDs, extensions, etc.

::: info
It worth to mention that `NArcInfoFlags::kByExtOnlyOpen` to avoid cases when 7zip will recognize archive inside of the archive because of weak signatures.
:::


## 2. Create Object

[CreateObject](./plugin-api-def.md#createobject) method is used to create Archive Handlers: 

<<< ../src/7z-assembly.cc#create_object{C++}

## 3. GetHandlerProperty2

Simply query for the properties. All of the handlers are defined in `handlers` array.

<<< ../src/7z-assembly.cc#get_handler_property{C++}

## 4. GetHandlerProperty and GetNumberOfFormats

We have 2 formats: `sz` and `sze`. And redirecting calls from `GetHandlerProperty` to `GetHandlerProperty2`

<<< ../src/7z-assembly.cc#rest{C++}

All set. Now we can start implementation of the [SZ](./sz-impl.md) and [SZE](./sze-impl.md) handlers.