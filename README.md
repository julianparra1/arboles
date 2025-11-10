# Arboles - La Belleza Algorítmica de las Plantas

 Graficación (2025) -  Proyecto Final

## Descripción

Proyecto de graficación que explora la naturaleza algorítmica de las plantas mediante L-Systems (Lindenmayer Systems) y renderizado OpenGL moderno.

## Características

- Sistema de L-Systems para generación procedural

## Requisitos

### Dependencias del Sistema
- C++17 compatible compiler (Clang++ recomendado)
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

## Estructura del Proyecto

```
arbol/
├── src/
│   ├── main.cpp              # Punto de entrada
│   ├── core/                 # Núcleo de la aplicación
│   ├── rendering/            # Sistema de renderizado
│   │   ├── Shader.cpp/h      # Gestión de shaders
│   │   └── Camera.cpp/h      # Sistema de cámara
│   ├── lsystem/              # L-Systems
│   │   ├── LSystem.cpp/h     # Implementación de L-Systems
│   │   └── TurtleRenderer.cpp/h  # Renderizado tipo turtle graphics
│   ├── shapes/               # Primitivas geométricas
│   └── ui/                   # Interfaz de usuario
│       └── UI.cpp/h          # Wrapper de ImGui
├── include/
│   └── Colors.h              # Definiciones de colores (Nord theme)
├── external/
│   ├── glad/                 # OpenGL loader
│   └── imgui/                # Dear ImGui library
├── build/                    # Archivos de compilación (generado)
├── Makefile                  # Sistema de construcción
├── .clangd                   # Configuración de clangd
└── .gitignore
```

## Compilación

### Build
```bash
make
```

### Run
```bash
make run
# o directamente
./arboles
```

### Limpiar
```bash
make clean
```

## Módulos

### Rendering
- **Shader**: Compilación y gestión de shaders GLSL
- **Camera**: Proyección ortográfica con corrección de aspect ratio

### UI
- **UI**: Interfaz ImGui
### L-System
- **LSystem**: Generación de cadenas mediante reglas de L-Systems
- **TurtleRenderer**: Interpretación y renderizado de L-Systems

## Controles

- `ESC`: Cerrar aplicación

## Licencia

No se...

## Créditos

- **Dear ImGui**: [github.com/ocornut/imgui](https://github.com/ocornut/imgui)
- **GLAD**: OpenGL loader
- **GLFW**: Biblioteca de windowing
- **GLM**: OpenGL Mathematics
