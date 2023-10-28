# How to compile 7-zip components

During my work on this documentation I compiled the File Manager to be able to see call stack and to debug the plugin system. This guide will allow you to compile any components you want to explore.

1. Navigate to the `CPP` folder and find `Build.mak`. This is base make file that is referenced by the other components. Open it.
2. Find `CFLAGS = $(CFLAGS) -nologo` ... line. Add `/Zi` compiler flag at the end (It instructs compiler to create a symbols file).
4. Find `LFLAGS = $(LFLAGS) -nologo -OPT:REF -OPT:ICF -INCREMENTAL:NO` and add `/DEBUG` is linker flag.
5. Find `CFLAGS_O1`, `CFLAGS_O2`, and disable optimization flags `-Ox` with `-Od`
6. Go to any component you want to compile. For example `CPP\7zip\UI\FileManager\`, open Visual Studio console there and type `nmake`. Should work without parameters.
7. Build process will create `x64` folder. Open it and find required component. Copy it to installed 7-zip version. Done.