# MCXX

MCXX is a small voxel renderer built with OpenGL 3.3. It generates terrain from Perlin noise, streams chunks around the player, builds visible block faces into GPU meshes, and renders shadows, clouds, a sun sprite, block selection, and an ImGui hotbar.

## Requirements

- Visual Studio with the Desktop development with C++ workload
- CMake 3.20 or newer
- Git, for CMake FetchContent dependencies
- An OpenGL 3.3-capable graphics driver

The project is configured for the MSVC toolchain. Run CMake from a Visual Studio Developer PowerShell or a terminal where the MSVC environment has been initialized.

## Build

Configure and build from the repository root:

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

The executable is written to `build/mcxx.exe`. CMake exports `build/compile_commands.json`; VS Code uses this file for C++ IntelliSense so compiler options and include paths stay aligned with the build.

## Run

Run from the repository root so shader and texture paths resolve correctly:

```powershell
build\mcxx.exe
```

## Controls

- `W`, `A`, `S`, `D`: move
- Mouse: look around
- `Space`: jump; press twice to toggle flight
- `Left Ctrl`: crouch or descend while flying
- `Left Shift`: sprint
- Left mouse button: remove the targeted block
- Right mouse button: place a block next to the targeted face
- Number keys `1` through `9`: select a hotbar slot
- `Escape`: close the window

## Rendering pipeline

Each frame updates player movement, chunk availability, cloud placement, block targeting, and the projection matrices. Rendering then proceeds in this order:

1. Render visible chunk geometry into a depth texture for shadow mapping.
2. Render chunk geometry with directional lighting, percentage-closer filtered shadows, and distance fog.
3. Render the camera-facing sun sprite.
4. Render transparent Perlin-noise cloud geometry.
5. Draw the selected block outline.
6. Draw the 2D crosshair and ImGui hotbar.

## Source layout

- `src/game/main.cpp`: application startup, input loop, OpenGL setup, and frame rendering
- `src/game/classes/world`: chunk streaming and world block edits
- `src/game/classes/chunk`: terrain storage and visible-face mesh generation
- `src/game/classes/entity`: player camera, movement, jumping, flight, and collision integration
- `src/game/classes/physics`: axis-aligned bounding-box collision helpers
- `src/game/classes/raycast`: block targeting rays
- `src/game/classes/ui`: ImGui hotbar rendering
- `src/game/utility`: shader source loading, shader programs, and texture loading
- `src/shaders`: OpenGL vertex and fragment shaders grouped by render pass
- `assets/sprites`: textures used by the renderer
- `vendor`: pinned third-party headers and sources
