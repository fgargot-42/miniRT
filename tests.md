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
| Path with spaces			| 			| ✅		|
| Quoted path with spaces	| 			| ✅		|

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
| Path with spaces			| 			| ✅		|

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
| Path with spaces			| 			| ✅		|
| Quoted path with spaces	| 			| ✅		|

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


### General / cross-cutting
| Test case									| Manda		| Bonus		|
|-------------------------------------------|-----------|-----------|
| No argument given (0 args)					| 			| ✅			|
| Too many arguments (multiple `.rt` files)	| 			| ✅			|
| Same file passed twice					| 			| ✅			|
| File extension in different case (`SCENE.RT`)	| 			| 			|
| Symbolic link to a valid file				| 			| ✅			|
| Symlink name has wrong extension, points to valid file (e.g. `link_no_ext -> scene.rt`)	| 			| ✅			|
| Symlink has correct extension, points to file with different/no extension (e.g. `link.rt -> scene.txt`)	| 			| ⚠️	(extension of pointed file not checked)		|
| Symlink has correct extension, points to a directory	| 			| ✅			|
| Broken symbolic link						| 			| ✅			|
| Symlink loop (`ln -s a b; ln -s b a`)		| 			| ✅			|
| File is a FIFO/pipe or special device file	| 			| ⚠️ (hanging on FIFO if no timeout)			|
| Filename with `..` / relative traversal	| 			| 			|
| Extremely long filename/path (near `PATH_MAX`)	| 			| 			|
| Multiple extensions, only last matters (`scene.txt.rt`)	| 			| 			|
| Hidden file that's valid (`.scene.rt`)		| 			| 			|
| Read permission on file but not on parent directory	| 			| 			|

**Notes:**
- Extension checks should be performed on the path/name as given (the symlink's own name), not on the resolved target. `open()` transparently follows the symlink regardless of the name check, so a symlink named `link_no_ext` pointing to a valid `.rt` file should still fail the extension check. Conversely, a symlink named `link.rt` pointing to a directory or wrong-extension file should pass the extension check and be caught later (directory check / open failure / content parsing), not by the extension check itself.
- `cat scene.rt > fifo.rt & timeout 5 ./miniRT fifo.rt` to run miniRT with a FIFO file. `mkfifo fifo.rt` to create FIFO file.

### File content edge cases
| Test case									| Manda		| Bonus		|
|-------------------------------------------|-----------|-----------|
| Correct extension but 0 bytes (empty)		| 			| 			|
| File contains only whitespace/newlines	| 			| 			|
| File contains null bytes or binary garbage	| 			| 			|
| Non-UTF-8 encoding (invalid bytes)		| 			| 			|
| Line without trailing newline at EOF		| 			| 			|
| Extremely large file (stress/performance, e.g. huge `.obj` mesh)	| 			| 			|
| Windows-style line endings (`\r\n`)		| 			| 			|

### Referenced sub-files (`.obj`, `.mtl`, images)
| Test case									| Manda		| Bonus		|
|-------------------------------------------|-----------|-----------|
| Relative path resolved from wrong base directory	| 			| 			|
| Same image/material file referenced multiple times	| 			| 			|
| Circular reference (e.g. `.mtl` referencing itself)	| 			| 			|

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
