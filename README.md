# Árboles — La Belleza Algorítmica de las Plantas

**Proyecto Final — Graficación 2025**
**Universidad Autónoma de Baja California (UABC)**
**Autor:** Jose Julian Parra Virgen

---

<p align="center">
  <img src="images/screenshot-main.webp" alt="Interfaz Principal" width="800">
</p>

## Descripción

**Árboles** es un visualizador interactivo de **Sistemas de Lindenmayer (L-Systems)** que genera estructuras botánicas y fractales matemáticos en tiempo real. El proyecto está inspirado en el libro *"The Algorithmic Beauty of Plants"* de Prusinkiewicz y Lindenmayer (1990).

Los L-Systems son gramáticas formales que utilizan reglas de reescritura paralela para modelar el crecimiento de plantas y generar patrones fractales autosimilares.

### ¿Qué puedes hacer con este programa?

- Visualizar árboles, helechos, flores y fractales generados proceduralmente
- Modificar axiomas y reglas en tiempo real
- Experimentar con diferentes ángulos y número de generaciones
- Explorar 12 presets botánicos y fractales predefinidos
- Navegar en 3D con una cámara orbital interactiva

---

## Galería

<table>
<tr>
<td align="center"><img src="images/screenshot-pino.webp" width="250"><br><b>Pino 3D</b></td>
<td align="center"><img src="images/screenshot-roble.webp" width="250"><br><b>Roble</b></td>
<td align="center"><img src="images/screenshot-cerezo.webp" width="250"><br><b>Cerezo en Flor</b></td>
</tr>
<tr>
<td align="center"><img src="images/screenshot-dragon.webp" width="250"><br><b>Curva del Dragón</b></td>
<td align="center"><img src="images/screenshot-koch.webp" width="250"><br><b>Copo de Koch</b></td>
<td align="center"><img src="images/screenshot-sierpinski.webp" width="250"><br><b>Triángulo de Sierpinski</b></td>
</tr>
</table>

<p align="center">
  <img src="images/growth.gif" alt="Animación de Crecimiento" width="400">
  <br><i>Evolución de un árbol a través de las generaciones</i>
</p>

---

## Requisitos del Sistema

### Dependencias

| Componente | Versión Mínima |
|------------|----------------|
| Compilador C++ | C++17 (Clang++ recomendado) |
| OpenGL | 3.3 Core Profile |
| GLFW3 | 3.x |
| GLM | 0.9.x |

### Instalación de Dependencias

#### Fedora / RHEL / CentOS
```bash
sudo dnf install glfw glfw-devel glm-devel clang
```

#### Ubuntu / Debian / Linux Mint
```bash
sudo apt install libglfw3-dev libglm-dev clang build-essential
```

#### Arch Linux
```bash
sudo pacman -S glfw glm clang
```

---

## Compilación e Instalación

### 1. Clonar el Repositorio
```bash
git clone https://github.com/julianparra1/arboles.git
cd arboles
```

### 2. Compilar
```bash
make
```

### 3. Ejecutar
```bash
./arboles
```

### Comandos Adicionales

| Comando | Descripción |
|---------|-------------|
| `make` | Compila el proyecto |
| `make clean` | Elimina archivos de compilación |
| `make clean && make` | Recompilación completa |

---

## Controles

### Navegación de Cámara

| Entrada | Acción |
|---------|--------|
| **Click izquierdo + Arrastrar** | Rotar la cámara alrededor del modelo |
| **Rueda del mouse** | Acercar / Alejar (Zoom) |
| **ESC** | Cerrar la aplicación |

### Panel de Control (ImGui)

El panel lateral izquierdo permite modificar todos los parámetros en tiempo real:

- **Axioma**: La cadena inicial del L-System
- **Reglas**: Las reglas de producción (ej: `F -> FF`, `X -> F[+X][-X]`)
- **Ángulo**: El ángulo de rotación para los comandos `+` y `-`
- **Generaciones**: Número de iteraciones del L-System (0-10)
- **Colores**: Color de ramas, hojas y flores
- **Presets**: 12 configuraciones predefinidas

---

## Comandos del L-System

El intérprete de Turtle Graphics soporta los siguientes comandos basados en la notación ABOP:

### Movimiento

| Comando | Descripción |
|---------|-------------|
| `F`, `G`, `A`, `B` | Avanzar dibujando un segmento de rama |
| `f` | Avanzar sin dibujar |

### Rotación 2D

| Comando | Descripción |
|---------|-------------|
| `+` | Girar a la izquierda (sentido antihorario) |
| `-` | Girar a la derecha (sentido horario) |
| `\|` | Dar vuelta 180° |

### Rotación 3D

| Comando | Descripción |
|---------|-------------|
| `&` | Pitch hacia abajo (inclinar hacia adelante) |
| `^` | Pitch hacia arriba (inclinar hacia atrás) |
| `\` | Roll a la izquierda |
| `/` | Roll a la derecha |

### Estado y Ramificación

| Comando | Descripción |
|---------|-------------|
| `[` | Guardar estado actual (posición, orientación, grosor) en la pila |
| `]` | Restaurar el último estado guardado de la pila |
| `!` | Reducir el grosor del segmento |
| `'` | Incrementar índice de color |

### Decoraciones

| Comando | Descripción |
|---------|-------------|
| `L` | Dibujar una hoja |
| `K` | Dibujar una flor |

### Variables (sin acción gráfica)

| Comando | Descripción |
|---------|-------------|
| `X`, `Y`, `S` | Variables auxiliares para las reglas de producción |

---

## Presets Incluidos

### Estructuras Botánicas 2D

| Preset | Axioma | Regla Principal | Ángulo |
|--------|--------|-----------------|--------|
| Sauce Elegante | `X` | `X -> F[@[-X]+X]` | 25° |
| Roble Clásico | `X` | `X -> F[+X]F[-X]+X` | 20° |
| Helecho Tropical | `X` | `X -> F+[[X]-X]-F[-FX]+X` | 25° |
| Rosa Trepadora | `X` | `X -> F[+X][-X]FX` | 25.7° |
| Girasol | `A` | `A -> [+A]FB[-A]` | 137.5° |
| Cerezo en Flor | `X` | `X -> F[+XK]F[-XL]` | 30° |

### Fractales Matemáticos

| Preset | Axioma | Reglas | Ángulo |
|--------|--------|--------|--------|
| Curva del Dragón | `FX` | `X -> X+YF+`, `Y -> -FX-Y` | 90° |
| Triángulo de Sierpinski | `F-G-G` | `F -> F-G+F+G-F`, `G -> GG` | 120° |
| Copo de Koch | `F` | `F -> F+F--F+F` | 60° |

### Estructuras 3D

| Preset | Axioma | Regla Principal | Ángulo |
|--------|--------|-----------------|--------|
| Árbol Espiral | `A` | `A -> F[&FL][^FL][/FL][\FL]A` | 25.7° |
| Arbusto | `A` | `A -> [&FL!A]/////'[&FL!A]` | 22.5° |
| Flor 3D | `P` | `P -> I+[P+K]--//[--L]I[++L]-[PK]++PK` | 18° |

---

## Estructura del Proyecto

```
arboles/
├── src/                          # Código fuente
│   ├── main.cpp                  # Punto de entrada de la aplicación
│   ├── core/                     # Utilidades centrales
│   ├── rendering/                # Sistema de renderizado
│   │   ├── Shader.cpp/.h         # Gestión de shaders GLSL
│   │   └── Camera.cpp/.h         # Sistema de cámara orbital
│   ├── lsystem/                  # Implementación de L-Systems
│   │   ├── LSystem.cpp/.h        # Motor de generación de cadenas
│   │   └── TurtleGraphics.cpp/.h # Intérprete de Turtle Graphics 2D/3D
│   ├── shapes/                   # Primitivas geométricas
│   └── ui/                       # Interfaz de usuario
│       └── UI.cpp/.h             # Panel de control con Dear ImGui
├── include/
│   └── Colors.h                  # Definiciones de colores (tema Nord)
├── external/
│   ├── glad/                     # Loader de funciones OpenGL
│   └── imgui/                    # Biblioteca Dear ImGui
├── images/                       # Capturas de pantalla para documentación
├── docs/                         # Presentación para GitHub Pages
├── build/                        # Artefactos de compilación (generado)
├── Makefile                      # Sistema de compilación
└── README.md                     # Este archivo
```

---

## Fundamentos Teóricos

### ¿Qué es un L-System?

Un **Sistema de Lindenmayer** (L-System) es una gramática formal definida por la tupla:

$$G = \langle V, \omega, P \rangle$$

Donde:
- **V** = Alfabeto de símbolos (variables y constantes)
- **ω** = Axioma (cadena inicial)
- **P** = Conjunto de reglas de producción

### Ejemplo: Algas de Lindenmayer

El primer L-System de la historia, creado por Aristid Lindenmayer en 1968:

```
Variables: A, B
Axioma: A
Reglas: A → AB, B → A

n=0: A
n=1: AB
n=2: ABA
n=3: ABAAB
n=4: ABAABABA
```

La longitud de la cadena sigue la secuencia de Fibonacci.

### Turtle Graphics

El sistema de Turtle Graphics interpreta cada símbolo de la cadena generada como un comando para una "tortuga" virtual que dibuja mientras se mueve:

1. La tortuga tiene una **posición** (x, y, z) y una **orientación** (heading, left, up)
2. Los comandos `F`, `G` mueven la tortuga hacia adelante, dibujando una línea
3. Los comandos `+`, `-` rotan la tortuga
4. Los corchetes `[` y `]` implementan una **pila** para guardar/restaurar estados, permitiendo ramificación

---

## Tecnologías Utilizadas

| Tecnología | Uso |
|------------|-----|
| **C++17** | Lenguaje principal con STL moderno |
| **OpenGL 3.3** | Renderizado gráfico (Core Profile) |
| **GLFW3** | Gestión de ventanas y eventos |
| **GLAD** | Loader de funciones OpenGL |
| **GLM** | Matemáticas 3D (vectores, matrices) |
| **Dear ImGui** | Interfaz gráfica inmediata |

---

## Referencias

- **Prusinkiewicz, P., & Lindenmayer, A.** (1990). *The Algorithmic Beauty of Plants*. Springer-Verlag.
- **Lindenmayer, A.** (1968). "Mathematical models for cellular interactions in development". *Journal of Theoretical Biology*, 18(3), 280-315.
- **Paul Bourke** — L-System User Notes. [paulbourke.net](http://paulbourke.net/fractals/lsys/)
- **Algorithmic Botany** — University of Calgary. [algorithmicbotany.org](http://algorithmicbotany.org/)

---

## Enlaces

- **Repositorio**: [github.com/julianparra1/arboles](https://github.com/julianparra1/arboles)
- **Presentación**: [julianparra1.github.io/arboles](https://julianparra1.github.io/arboles/)

---

## Licencia

Proyecto educativo — Graficación 2025, UABC.

---

<p align="center">
  <i>"Desde reglas simples emerge la complejidad de la naturaleza."</i>
</p>
