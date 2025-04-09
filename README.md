
#  Talon Engine

**Talon Engine** is a lightweight, modular 2D game engine written in C++ using SDL2.  
Inspired by Unity, designed from scratch, and built with pure muscle.

---

## Features

-  **GameObject + Component system** (Unity-style)
-  **Rigidbody2D physics** with mass, drag, and gravity
-  **Per-axis collision detection and resolution** (no wall clipping)
-  **Input system** with real-time WASD and jump support
-  **SpriteRenderer** for rendering shapes via SDL2
- **CollisionManager** handles all collidable objects
- Clean, readable architecture using smart pointers and lifecycle methods (`Awake`, `Start`, `Update`)

---

## Requirements

- [SDL2](https://www.libsdl.org/download-2.0.php)
- [vcpkg](https://github.com/microsoft/vcpkg) (for dependency management)
- C++17 or higher
- Visual Studio (recommended)
