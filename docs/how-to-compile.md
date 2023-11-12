# How to compile 7-zip components

During my work on this documentation, I compiled the File Manager to be able to see the call stack and to debug the plugin system. This guide will allow you to compile any components you want to explore.

1. Navigate to the `CPP` folder and find `Build.mak`. This is the base make file that is referenced by the other components. Open it.
2. Find the line `CFLAGS = $(CFLAGS) -nologo` and add the `/Zi` compiler flag at the end (It instructs the compiler to create a symbols file).
4. Find the line `LFLAGS = $(LFLAGS) -nologo -OPT:REF -OPT:ICF -INCREMENTAL:NO` and add the `/DEBUG` linker flag.
5. Find the lines `CFLAGS_O1` and `CFLAGS_O2`, and disable the optimization flags `-Ox` by replacing them with `-Od`.
6. Go to any component you want to compile. For example, `CPP\7zip\UI\FileManager\`. Open the Visual Studio console there and type `nmake`. It should work without parameters.
7. The build process will create an `x64` folder. Open it and find the required component. Copy it to the installed 7-zip version. Done.