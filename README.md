# STLRepair

### What is STLRepair?
STLRepair is a simple comment line tool for repairing broken binary STL files for 3D printing.

This tool attempts to fix common structural issues in files that some exporters can introduce that can bewilder importers. Common issues include:

*  File headers that fool importers into believing they're ASCII STL files.
* Truncated triangle data.
* Non-triangle data at the end of a file.
* Weird use of triangle attribute counts.

If you've encountered the dreaded "Error opening file" message in MeshMixer or "Failed to load X. The file could be corrupt, inaccessible, or it did not contain any models," message in Cura, this tool might be the tool to help!

### What STLRepair Is Not

STL Repair does NOT attempt to fix issues with geometry. There are some great visual tools for dealing with those sorts of things. STLRepair is focused entirely on file structure. All geometry information is preserved as it exists in the original file.

### How to Use STLRepair

Extract STLRepair to whereever makes sense for you and execute it like so...

`stlrepair <path_to_stl_file>`

STLRepair will ask you a series of questions on the types of repairs and it will produce a new file that contains your selected changes.

### System Requirements

Currently, only Windows platforms are supported. That being said, there are small number of changes needed to support Linux and OSX. That's in my short term plan. So if your platform isn't currently supported, check back periodically. It'll likely be supported soon.

### License (MIT)

Copyright 2024, Shane Kirk

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
