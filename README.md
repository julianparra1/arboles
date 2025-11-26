# Árboles - The Algorithmic Beauty of Plants

**Computer Graphics (2025) - Final Project**

## Description

An interactive L-System visualizer for generating beautiful botanical structures including trees, flowers, ferns, and mathematical fractals. Built with OpenGL 3.3 Core Profile and Dear ImGui.

Inspired by concepts from *"The Algorithmic Beauty of Plants"* by Prusinkiewicz and Lindenmayer.

## Features

- **Unified Turtle Graphics Engine**: Single renderer supporting both 2D and 3D visualization modes
- **L-System Generation**: Configurable axiom, rules, angle, and iteration count
- **12 Built-in Presets**: Beautiful botanical and fractal examples
  - **2D Botanical**: Elegant Willow, Classic Oak, Tropical Fern, Climbing Rose, Garden Sunflower, Cherry Blossom
  - **2D Fractal**: Dragon Curve, Sierpinski Triangle, Koch Snowflake
  - **3D Botanical**: Spiral Tree, Bush, 3D Flower
- **Interactive Camera**: Orbital camera with zoom and rotation controls
- **Real-time Parameters**: Adjust L-System rules, colors, and rendering on the fly
- **Flower & Leaf Decorations**: Support for botanical decorations with `L` and `K` commands

## Requirements

### System Dependencies
- C++17 compatible compiler (Clang++ recommended)
- OpenGL 3.3+
- GLFW3
- GLM (OpenGL Mathematics)

### Linux (Fedora/RHEL)
```bash
sudo dnf install glfw glfw-devel glm-devel clang
```

### Linux (Debian/Ubuntu)
```bash
sudo apt install libglfw3-dev libglm-dev clang
```

## Project Structure

```
arbol/
├── src/
│   ├── main.cpp                    # Application entry point
│   ├── core/                       # Core application utilities
│   ├── rendering/                  # Rendering system
│   │   ├── Shader.cpp/h            # GLSL shader management
│   │   └── Camera.cpp/h            # Orbital camera system
│   ├── lsystem/                    # L-System implementation
│   │   ├── LSystem.cpp/h           # L-System string generation
│   │   └── TurtleGraphics.cpp/h    # Unified 2D/3D turtle graphics renderer
│   ├── shapes/                     # Geometric primitives
│   └── ui/                         # User interface
│       └── UI.cpp/h                # Dear ImGui wrapper with presets
├── include/
│   └── Colors.h                    # Color definitions (Nord theme)
├── external/
│   ├── glad/                       # OpenGL function loader
│   └── imgui/                      # Dear ImGui library
├── build/                          # Compilation artifacts (generated)
├── Makefile                        # Build system
├── .clangd                         # clangd LSP configuration
└── .gitignore
```

## Build

### Compile
```bash
make
```

### Clean Build
```bash
make clean && make
```

### Run
```bash
./arboles
```

## L-System Commands

The turtle graphics interpreter supports the following commands based on ABOP notation:

| Command | Description |
|---------|-------------|
| `F`, `G` | Move forward and draw a branch segment |
| `f` | Move forward without drawing |
| `+` | Turn left by angle (yaw in 3D) |
| `-` | Turn right by angle (yaw in 3D) |
| `&` | Pitch down (3D only) |
| `^` | Pitch up (3D only) |
| `\` | Roll left (3D only) |
| `/` | Roll right (3D only) |
| `\|` | Turn around 180° |
| `[` | Push current state onto stack |
| `]` | Pop state from stack |
| `!` | Decrease segment thickness |
| `'` | Increment color index |
| `L` | Draw a leaf decoration |
| `K` | Draw a flower decoration |
| `X`, `Y`, `S` | Variables (no drawing action) |

## Module Description

### Rendering
- **Shader**: GLSL shader compilation and management
- **Camera**: Orbital camera with perspective/orthographic projection

### UI
- **UI**: Dear ImGui interface with preset selector and parameter controls

### L-System
- **LSystem**: Rule-based string generation engine
- **TurtleGraphics**: Unified 2D/3D turtle interpreter and OpenGL renderer

## Controls

| Input | Action |
|-------|--------|
| **Left Mouse + Drag** | Rotate camera |
| **Scroll Wheel** | Zoom in/out |
| **ESC** | Close application |

## Example Presets

### 2D - Elegant Willow
```
Axiom: X
Rules: X -> F[@[-X]+X], F -> FF
Angle: 25°, Generations: 7
```

### 3D - Spiral Tree
```
Axiom: A
Rules: A -> F[&FL][^FL][/FL][\FL]A, F -> FF
Angle: 25.7°, Generations: 5
```

## Screenshots

*Coming soon...*

## Credits

- **"The Algorithmic Beauty of Plants"** - Prusinkiewicz & Lindenmayer
- **Dear ImGui**: [github.com/ocornut/imgui](https://github.com/ocornut/imgui)
- **GLAD**: OpenGL function loader generator
- **GLFW**: Cross-platform windowing library
- **GLM**: OpenGL Mathematics library

## License

Educational use - Computer Graphics 2025
