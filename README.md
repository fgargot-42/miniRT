*This project has been created as part of the 42 curriculum by fgargot, mabarrer*

# (not so) miniRT

> *A sphere is nothing without light (and a camera).*

## Description

miniRT is a raytracer written in C using MacroLibX.

### Features

**Mandatory part**
- Basic primitives: spheres, planes and cylinders
- Ambient and lighting with shadows
- FPS-like controls
- BVH (Bounding Volume Hierarchy) with SAH (Surface Area Heuristic) for fast computation of ray/object intersection.
- Editor with sliders to tweak objects and lights in realtime

**Bonus part**
- Additional primitives: cones, hyperboloids, paraboloids and triangles
- `.obj` model loading, with material `.mtl` and texture parsing
- Texture mapping (uv mapping) on object
- Bump mapping for surface detail
- Extended lighting properties: specular, shininess, opacity, refraction, reflection
- Multithreaded rendering (pthreads)
- Skybox / sky color


## Instructions

### Compilation
```bash
# Build
make [bonus]

# Run
./miniRT FILE.rt
```

### Controls
| Key         | Action                  |
|-------------|-------------------------|
| `W A S D`   | Move camera             |
| `E` / `A`   | Camera height           |
| `MOUSE2`    | Camera panning          |
| `MOUSE1`    | Select / unselect object|
| `MWHEEL`    | Camera zoom             |
| `MOUSE3`    | Camera translation      |
| `T`         | Toggle transparency     |
| `G`         | Toggle specular         |
| `B`         | Toggle bump             |
| `Y`         | Toggle anti-aliasing    |
| `TAB`       | Toggle info             |
| `+` / `-`   | BVH display level       |

## Resources


[Ray-Box intersection algorithm](https://perso.univ-lyon1.fr/jean-claude.iehl/Public/educ/M1IMAGE/williams_box.pdf) - Visited: 2026/05/11

[How to build a BVH - Part 1: Basics](https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/) - Visited: 2026/05/11

[How to build a BVH - Part 2: Faster rays](https://jacco.ompf2.com/2022/04/18/how-to-build-a-bvh-part-2-faster-rays/) - Visited: 2026/06/12

[How to build a BVH - Part 3: Faster construction](https://jacco.ompf2.com/2022/04/21/how-to-build-a-bvh-part-3-quick-builds/) - Visited: 2026/06/17

[Video about BVH by Sebastien Lague](https://youtu.be/C1H4zIiCOaI?si=FIgXkzk2PjYqYfKb)

[How to make a raytracer from scratch playlist by The Cherno](https://youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&si=vxszfJgvRj4SNdXP)
