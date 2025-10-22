
# General Format

```ElementIO
DECLARE ElementIO V:1 !!DECLARE
SECTION TYPELIST
	TYPE [TypeName] [--r] !!TYPE
	...
!!SECTION TYPELIST
SECTION CONFIGURATION
	CONFIG [ConfigName]:[Value] !!CONFIG
!!SECTION CONFIGURATION
SECTION BLOBS
	BLOB [ID] [--f & [FormatName]] [Size] [Data...] !!BLOB
	...
!!SECTION BLOBS
SECTION ELEMENTS
	ELEMENT [ID] [TypeName] [[AttrName]:[AttrValue]...] !!ELEMENT [ID]
	ELEMENT [ID] [TypeName] [[AttrName]:[AttrValue]...]
		ELEMENT [ID] [TypeName] [[AttrName]:[AttrValue]...] ...(Either end or multiline)
		...
	!!ELEMENT [ID]
!!SECTION ELEMENTS
```

# Rules
## General Rules
### Tags
A tag is defined to have an opener, and a closer tag. The opener tag is one of the following keywords:
1. `DECLARE`: Starts out the document, by defining the standard and the standard version.
2. `SECTION`: Describes a block of the file, followed by `TYPELIST`, `CONFIGURATION`, `BLOBS`, or `ELEMENTS`. 
3. `TYPE`:  Describes a single type name, and if its required.
4. `CONFIG`: Describes a single configuration Name:Value pair. 
5. `BLOB`: Describes a block of raw binary data.
6. `ELEMENT`: Describes an element.

Tags are closed using the following rules, for each of the tags:
1. `DECLARE`: Ended with `!!DECLARE`. This is a *single line only* tag.
2. `SECTION`: Ended with `!!SECTION [SectionType]`. This is a *multi-line only* tag. 
3. `TYPE`:  Ended with `!!TYPE`. This is a *single line only* tag.
4. `CONFIG`: Ended with `!!CONFIG` This is a *single line only* tag.
5. `BLOB`: Ended with `!!BLOB`. This is a *single line only* tag.
6. `ELEMENT`: Ended with `!!ELEMENT [ID]`. This is a *single line* or *multi-line* tag.

If a tag is not closed properly, the reader will throw an error and will not continue reading.