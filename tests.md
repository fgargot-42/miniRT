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
| Absolute path				| 			| ✅		|
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
| Absolute path				| 			| ✅		|
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


## Parsing test cases

### .rt files
| Test case								| Manda		| Bonus		|
|-------------------------------------------|-----------|-----------|
| No A										| 			| 			|
| No C										| 			| 			|
| No L										| 			| 			|
| Multiple A								| 			| 			|
| Multiple C								| 			| 			|
| Multiple L (manda only)					| 			| 			|
| Multiple S (bonus only)					| 			| 			|
| Multiple SB (bonus only)					| 			| 			|
| S and SB present (bonus only)				| 			| 			|
| No shape									| 			| 			|
| Only non-bvh shapes (planes)				| 			| 			|
| Wrong parameters							| 			| 			|
| Bonus parameters (manda only)				| 			| 			|
| Wrong identifier							| 			| 			|
| Bonus object identifier (manda only)		| 			| 			|

### .obj file
| Test case									| Manda		| Bonus		|
|-------------------------------------------|-----------|-----------|
| Missing/additional parameter in `v`		| 			| 			|
| Missing/additional parameter in `vn`		| 			| 			|
| Missing/additional parameter in `vt`		| 			| 			|
| `f` has missing v/vn/vt					| 			| 			|
| `f` has missing parameter (0..2 vertices)	| 			| 			|
| `f` has wrong parameter					| 			| 			|
| `f` has a negative index					| 			| 			|
| `usemtl` with missing `mtllib`			| 			| 			|
| `usemtl` with wrong material name (not in mtl file)	| 			| 			|
| v/vt/vn/f has non-numeric parameter		| 			| 			|

### .mtl file
| Test case									| Manda		| Bonus		|
|-------------------------------------------|-----------|-----------|
| No `newmtl`								| 			| 			|
| Unnamed `newmtl`							| 			| 			|
| Wrong number of parameters (Ka 1.000000, Ns 250.000000 1.000000)	| 			| 			|
| Not valid value							| 			| 			|
| Empty material (nothing between two consecutive `newmtl` lines)	| 			| 			|
| Invalid parameter							| 			| 			|
| Invalid option (ex. map\_Kd -nope)		| 			| 			|
| Wrong number of option values (ex. map\_Kd -s 0.5)	| 			| 			|
