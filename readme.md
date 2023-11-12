# 7-Zip (7z) Archiver Plugins

This repository contains source code for 2 basic archiver handlers that extend the functionality of the 7-Zip File Manager. Below, you can find details on the archive formats:

Criteria | SZ | SZE
-------- | ------ | -------
Name | Sample ZIP Archiver | Sample ZIP Archiver Extended
Format | Returns file entities. Three files named `sampleX.txt` with the text `sampleX`. File `sample3.txt` is in the subdirectory named `someDir`. Finally, the `child.sz` archive creates endless recursion. | Files are encoded into the "archive" using text format: `{fileName}.{fileExtension}-{fileContent}|{fileName}.{fileExtension}-{fileContent}.....`
Extensions | .sz | .sze, .szex
Signature | Binary "SZ" | Binary "SE"

<div align="center">

All of the documentation is hosted on [*GitHub Pages*](https://ikremniou.github.io/7z-assembly/)
</div>


Items in progress:
- [ ] Implement the SZE archive
- [ ] Restructure documentation in the `Plugin Interface` section
- [ ] Implement real-word example (probably for some zlib users like `rpa`)
