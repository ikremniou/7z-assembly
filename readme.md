# 7-Zip (7z) Archiver Plugins

This repository contains source code for 2 basic archiver handlers that extend the functionality of the 7-Zip File Manager. Below, you can find details about the properties of the archiver:

Criteria | The SZ | The SZE
-------- | ------ | -------
Name | Sample ZIP Archiver | Sample ZIP Archiver Extended
Format | Returns file entities. Three files named `sampleX.txt` with the text `sampleX`. File `sample3.txt` is in the subdirectory named `someDir`. Finally, the `child.sz` archive creates endless recursion. | Files are encoded into the "archive" using text format: `{fileName}.{fileExtension}-{fileContent}|{fileName}.{fileExtension}-{fileContent}.....`
Extensions | .sz | .sze, .szex
Signature | Binary "SZ" | Binary "SE"

All of the documentation is hosted on [GitHub Pages](ikremniou.github.io/7z-assembly);