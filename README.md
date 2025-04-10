#  Talon Engine

**Talon Engine** is a lightweight, modular 2D game engine written in C++ using SDL2.  
Inspired by Unity, designed from scratch, and built with pure muscle.

---

## Features

- **GameObject + Component System**  
  Inspired by Unity, every object is modular, dynamic, and follows a clean lifecycle: `Awake()`, `Start()`, `Update()`.

- **Input System**  
  Handles real-time keyboard input (WASD, spacebar, Shift) using SDL2's raw scancodes.

- **Animator with Frame Events**  
  Spritesheet-based animation with timing control, direction switching, and frame-triggered callbacks (`std::function` per frame).

- **SpriteRenderer**  
  Renders sprites with `SetSourceRect()` support for pixel-perfect control over frames and spritesheets.

- **Rigidbody2D Physics**  
  - Gravity, drag, velocity, acceleration  
  - `AddForce()` and `ApplyImpulse()`  
  - Fully frame-based physics update with jump support

- **BoxCollider**  
  Collision detection with offset support, overlap handling.

- **CollisionManager**  
  Handles collision resolution and prediction before applying movement.

- **Vector2 Math Library**  
  Clean utility methods like `Normalize()`, `Length()`, `Dot()`, and operator overloads.

- **Folder-based project layout**  
  Organized includes, systems, and components — Visual Studio–friendly.

-  **Built From Scratch**  
  No engine templates, no frameworks — just pure C++ and SDL2.

---

## In Progress / Planned

- JSON-based prefab & scene loading
- Tilemap & Z-Ordering
- CircleCollider
- In-engine level editor (drag, drop, save)  
- Audio system (sound FX, music)  
- Save/load game state  
- Component editor UI  
- Particle system & animation blending


## Requirements

- [SDL2](https://www.libsdl.org/download-2.0.php)
- [vcpkg](https://github.com/microsoft/vcpkg)
- C++17 or higher
- Visual Studio (recommended)

---

## 🔧 Getting Started

Follow these steps to build and run **Talon Engine** locally.

### 1. Clone the Repository

```bash
git clone https://github.com/severmanolescu/Talon.git
```

### 2. Install Dependencies with vcpkg

If you don’t have vcpkg installed yet:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
```
Then install SDL2 and SDL2-Image:
```bash
vcpkg install sdl2 sdl2-image 
```
### 3. Open the Project in Visual Studio

-   Open `Talon.sln`
    
-   Go to `Project Properties > VC++ Directories`
    
-   Add:
    
    -   **Include Directories**: `C:\path\to\vcpkg\installed\x64-windows\include`
        
    -   **Library Directories**: `C:\path\to\vcpkg\installed\x64-windows\lib`
    
### 4. Link SDL2 Libraries

Go to `Project Properties > Linker > Input > Additional Dependencies`, and add:

`SDL2.lib SDL2main.lib SDL2_image.lib` 

**Copy all the dlls from  `C:\path\to\vcpkg\installed\x64-windows\bin` next to the `.exe` at runtime**
