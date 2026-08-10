# MiniRT test cases

OK: ✅ 	KO: ❌

## File opening cases

### .rt files
| Test case					| Manda		| Bonus		|
|---------------------------|-----------|-----------|
| File does not exist		| ✅		| ✅		|
| Does not end with `.rt`	| 			| 			|
| 	- `scenert`				| ✅		| ✅		|
| 	- `scene.rtrt`			| ✅		| ✅		|
| Is a directory `dir.rt`	| ✅		| ✅		|
| Is named `.rt`			| ✅		| ✅		|
| No read permission		| ✅		| ✅		|
| No write/exec permission	| ✅		| ✅		|
| Absolute path				| ✅		| ✅		|
| Path with spaces			| ✅		| ✅		|


### .obj files
| Test case					| Manda		| Bonus		|
|---------------------------|-----------|-----------|
| File does not exist		| 			| ✅		|
| Does not end with `.obj`	| 			|			|
| 	- `cubeobj`				| 			| ✅		|
| 	- `cube.objobj`			| 			| ✅		|
| Is a directory `dir.obj`	| 			| ✅		|
| Is named `.obj`			| 			| ✅		|
| No read permission		| 			| ✅		|
| No write/exec permission	| 			| ✅		|
| Absolute path				| 			| ❌ (not functional)		|
| Path with spaces			| 			| ❌ (not functional, leak)	|

**Notes:**
- Absolute path fails to open
- Path with spaces fails to open, tries (and fails) to open the part after space as image, leak occurs in `parse_scene`


### .mtl files
| Test case					| Manda		| Bonus		|
|---------------------------|-----------|-----------|
| File does not exist		| 			| ✅		|
| Does not end with `.mtl`	| 			|			|
| 	- `materialmtl`			| 			| ✅		|
| 	- `material.mtlmtl`		| 			| ✅		|
| Is a directory `dir.mtl`	| 			| ✅		|
| Is named `.mtl`			| 			| ✅		|
| No read permission		| 			| ✅		|
| No write/exec permission	| 			| ✅		|
| Absolute path				| 			| ✅		|
| Path with spaces			| 			| ❌ (not functional) |

**Notes:**
- Conditional jump occurs in `parse_scene` when `.mtl` file successfully opens

### Images in .rt files
| Test case					| Manda		| Bonus		|
|---------------------------|-----------|-----------|
| File does not exist		| 			| ✅		|
| Wrong file extension		| 			| 			|
| 	- `imagepng`			| 			| ✅		|
| 	- `image.pngpng`		| 			| ✅		|
| Is a directory `dir.png`	| 			| ✅		|
| Is named `.png`			| 			| ✅		|
| Is empty file				| 			| ✅		|
| No read permission		| 			| ✅		|
| No write/exec permission	| 			| ✅		|
| Absolute path				| 			| ❌ (not functional)			|
| Path with spaces			| 			| ❌ (not functional)			|

**Notes:**
- absolute path is appended to the .rt filepath as if it was a relative path
- Parts of a pathname with spaces are conducted separately, even with single quotes. This behavior is expected only without single quotes.

### Images in .mtl files
| Test case					| Manda		| Bonus		|
|---------------------------|-----------|-----------|
| File does not exist		| 			| ✅		|
| Wrong file extension		| 			| 			|
| 	- `imagepng`			| 			| ✅		|
| 	- `image.pngpng`		| 			| ✅		|
| Is a directory `dir.png`	| 			| ✅		|
| Is named `.png`			| 			| ✅		|
| Is empty file				| 			| ✅		|
| No read permission		| 			| ✅		|
| No write/exec permission	| 			| ✅		|
| Absolute path				| 			| ✅		|
| Path with spaces			| 			| ✅		|


Parsing test cases

.rt files:

	- no A
	- no C
	- no L
	- multiple A
	- multiple C
	- multiple L (manda only)
	- multiple S (bonus only)
	- multiple SB (bonus only)
	- S and SB present (bonus only)
	- no shape
	- only non-bvh shapes (planes)
	- wrong parameters
	- bonus parameters (manda only)
	- wrong identifier
	- bonus object identifier (manda only)
