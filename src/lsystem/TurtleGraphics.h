/**
 * @file TurtleGraphics.h
 * @brief Renderizador unificado de graficos de tortuga para visualizacion de L-Systems.
 *
 * Esta clase implementa interpretacion de graficos de tortuga para cadenas de L-System,
 * soportando modos de renderizado 2D y 3D. Basado en el formalismo descrito en
 * "The Algorithmic Beauty of Plants" por Prusinkiewicz & Lindenmayer (1990).
 *
 * La tortuga mantiene un estado consistiendo en posicion y orientacion en espacio 3D.
 * En modo 3D, la orientacion se define por tres vectores ortonormales (H, L, U):
 *   - H (Heading): Direccion hacia donde mira la tortuga
 *   - L (Left): Vector apuntando a la izquierda de la tortuga
 *   - U (Up): Vector apuntando hacia arriba desde la tortuga
 *
 * Simbolos de L-System soportados:
 *   Movimiento:
 *     F, G  - Avanzar dibujando un segmento de linea
 *     f     - Avanzar sin dibujar
 *
 *   Rotacion (modo 2D usa eje Z, modo 3D usa ejes locales de tortuga):
 *     +     - Girar izquierda (yaw) por angulo delta
 *     -     - Girar derecha (yaw) por angulo delta
 *     &     - Pitch hacia abajo por angulo delta
 *     ^     - Pitch hacia arriba por angulo delta
 *     \     - Roll izquierda por angulo delta
 *     /     - Roll derecha por angulo delta
 *     |     - Dar vuelta (180 grados)
 *
 *   Ramificacion:
 *     [     - Guardar estado actual en pila
 *     ]     - Restaurar estado desde pila
 *
 *   Decoraciones:
 *     L     - Colocar una hoja en posicion actual
 *     K     - Colocar una flor en posicion actual
 *
 *   Modificadores:
 *     !     - Disminuir ancho de linea por factor de decaimiento
 *     '     - Cambiar color hacia verde (para efectos de gradiente)
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#ifndef TURTLE_GRAPHICS_H
#define TURTLE_GRAPHICS_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>
#include <string>
#include <vector>

/**
 * @brief Modo de renderizado para graficos de tortuga.
 */
enum class RenderMode {
    Lines,     ///< Renderizado simple con GL_LINES (rapido, funciona en todo hardware)
    Cylinders  ///< Renderizado instanciado de cilindros 3D (mas realista)
};

/**
 * @brief Estado completo de la tortuga para interpretacion de L-System.
 *
 * Almacena posicion, orientacion (como tres vectores ortonormales), y
 * propiedades visuales como ancho de linea y color.
 */
struct TurtleState {
    glm::vec3 position{0.0f, 0.0f, 0.0f};  ///< Posicion actual en espacio mundial
    glm::vec3 heading{0.0f, 1.0f, 0.0f};   ///< Vector H: direccion de movimiento
    glm::vec3 left{-1.0f, 0.0f, 0.0f};     ///< Vector L: perpendicular izquierda
    glm::vec3 up{0.0f, 0.0f, 1.0f};        ///< Vector U: perpendicular arriba
    float width{0.02f};                    ///< Ancho actual de linea/rama
    glm::vec3 color{0.45f, 0.30f, 0.15f};  ///< Color actual de dibujo (cafe)
    int depth{0};                          ///< Profundidad de ramificacion para decaimiento
};

/**
 * @brief Datos para un solo segmento de rama.
 */
struct BranchData {
    glm::vec3 start;    ///< Posicion inicial
    glm::vec3 end;      ///< Posicion final
    float radiusStart;  ///< Radio al inicio
    float radiusEnd;    ///< Radio al final
    glm::vec3 color;    ///< Color de rama
};

/**
 * @brief Datos para decoracion de hoja o flor.
 */
struct DecorationData {
    glm::vec3 position;     ///< Posicion en espacio mundial
    glm::mat4 orientation;  ///< Matriz de orientacion
    glm::vec3 color;        ///< Color
    float size;             ///< Factor de escala
    int type;               ///< 0 = hoja, 1 = flor
};

/**
 * @class TurtleGraphics
 * @brief Renderizador unificado para graficos de tortuga de L-System en 2D y 3D.
 *
 * Esta clase proporciona una solucion completa para renderizar cadenas generadas
 * por L-System usando graficos de tortuga. Soporta renderizado simple de lineas
 * (adecuado para vista 2D o preview rapido) y renderizado completo de cilindros 3D
 * con iluminacion apropiada para visualizacion realista de arboles/plantas.
 *
 * Ejemplo de uso:
 * @code
 *   TurtleGraphics turtle;
 *   turtle.initialize();
 *   turtle.set3DMode(true);
 *   turtle.setRenderMode(RenderMode::Cylinders);
 *   turtle.interpret("F[+F][-F]F", 25.0f);
 *   // En bucle de renderizado:
 *   turtle.render(viewMatrix, projectionMatrix, lightPosition);
 * @endcode
 */
class TurtleGraphics {
public:
    TurtleGraphics();
    ~TurtleGraphics();

    // No copiable
    TurtleGraphics(const TurtleGraphics&) = delete;
    TurtleGraphics& operator=(const TurtleGraphics&) = delete;

    /**
     * @brief Inicializa recursos de OpenGL (shaders, buffers).
     * @return true si la inicializacion fue exitosa, false en caso contrario.
     */
    bool initialize();

    /**
     * @brief Interpreta una cadena de L-System y genera geometria.
     * @param lsystemString La cadena producida por generacion de L-System.
     * @param angle El angulo delta en grados para comandos de rotacion.
     */
    void interpret(const std::string& lsystemString, float angle);

    /**
     * @brief Renderiza la geometria generada.
     * @param view Matriz de vista desde la camara.
     * @param projection Matriz de proyeccion.
     * @param lightPos Posicion de la fuente de luz (para modo 3D).
     */
    void render(const glm::mat4& view, const glm::mat4& projection,
                const glm::vec3& lightPos = glm::vec3(5.0f, 10.0f, 5.0f));

    /**
     * @brief Limpia toda la geometria generada.
     */
    void clear();

    // =========================================================================
    // Setters de Configuracion
    // =========================================================================

    void set3DMode(bool enable) {
        m_is3D = enable;
    }
    void setRenderMode(RenderMode mode) {
        m_renderMode = mode;
    }
    void setStepSize(float step) {
        m_stepSize = step;
    }
    void setInitialWidth(float width) {
        m_initialWidth = width;
    }
    void setWidthDecay(float decay) {
        m_widthDecay = decay;
    }
    void setBranchColor(const glm::vec3& color) {
        m_branchColor = color;
    }
    void setLeafColor(const glm::vec3& color) {
        m_leafColor = color;
    }
    void setFlowerColor(const glm::vec3& color) {
        m_flowerColor = color;
    }
    void setLeafSize(float size) {
        m_leafSize = size;
    }

    // =========================================================================
    // Getters de Configuracion
    // =========================================================================

    bool is3DMode() const {
        return m_is3D;
    }
    RenderMode getRenderMode() const {
        return m_renderMode;
    }
    float getStepSize() const {
        return m_stepSize;
    }
    float getInitialWidth() const {
        return m_initialWidth;
    }
    float getWidthDecay() const {
        return m_widthDecay;
    }
    glm::vec3 getBranchColor() const {
        return m_branchColor;
    }
    glm::vec3 getLeafColor() const {
        return m_leafColor;
    }
    glm::vec3 getFlowerColor() const {
        return m_flowerColor;
    }
    float getLeafSize() const {
        return m_leafSize;
    }

    // =========================================================================
    // Estadisticas
    // =========================================================================

    size_t getBranchCount() const {
        return m_branches.size();
    }
    size_t getDecorationCount() const {
        return m_decorations.size();
    }

    // =========================================================================
    // Control del Piso
    // =========================================================================

    void setShowFloor(bool show) {
        m_showFloor = show;
    }
    bool getShowFloor() const {
        return m_showFloor;
    }

private:
    // =========================================================================
    // Metodos Internos
    // =========================================================================

    void setupFloor();
    void renderFloor(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightPos);
    void processCommand(char cmd, float angle);
    bool compileShaders();
    void uploadBranchData();
    void uploadDecorationData();
    void renderLines(const glm::mat4& view, const glm::mat4& projection);
    void renderCylinders(const glm::mat4& view, const glm::mat4& projection,
                         const glm::vec3& lightPos);
    void renderDecorations(const glm::mat4& view, const glm::mat4& projection,
                           const glm::vec3& lightPos);

    /**
     * @brief Rota un vector alrededor de un eje usando la formula de Rodrigues.
     */
    static glm::vec3 rotateAroundAxis(const glm::vec3& vec, const glm::vec3& axis, float angleDeg);

    // =========================================================================
    // Estado
    // =========================================================================

    bool m_initialized{false};
    bool m_is3D{false};
    RenderMode m_renderMode{RenderMode::Lines};

    TurtleState m_currentState;
    std::stack<TurtleState> m_stateStack;

    // =========================================================================
    // Geometria Generada
    // =========================================================================

    std::vector<BranchData> m_branches;
    std::vector<DecorationData> m_decorations;
    std::vector<DecorationData> m_leaves;   ///< Hojas separadas para render
    std::vector<DecorationData> m_flowers;  ///< Flores separadas para render

    // =========================================================================
    // Parametros de Renderizado
    // =========================================================================

    float m_stepSize{0.08f};
    float m_initialWidth{0.02f};
    float m_widthDecay{0.7f};
    float m_leafSize{0.08f};
    glm::vec3 m_branchColor{0.4f, 0.25f, 0.1f};  // Marron mas rico
    glm::vec3 m_leafColor{0.15f, 0.55f, 0.15f};  // Verde mas vibrante
    glm::vec3 m_flowerColor{1.0f, 0.45f, 0.7f};  // Rosa intenso

    // =========================================================================
    // Recursos OpenGL - Modo Lineas
    // =========================================================================

    GLuint m_lineVAO{0};
    GLuint m_lineVBO{0};
    GLuint m_lineShader{0};

    // =========================================================================
    // Recursos OpenGL - Modo Cilindros
    // =========================================================================

    GLuint m_cylinderVAO{0};
    GLuint m_cylinderVBO{0};
    GLuint m_cylinderInstanceVBO{0};
    GLuint m_cylinderShader{0};

    // =========================================================================
    // Recursos OpenGL - Decoraciones (hojas/flores)
    // =========================================================================

    GLuint m_decorationVAO{0};
    GLuint m_decorationVBO{0};
    GLuint m_decorationInstanceVBO{0};
    GLuint m_decorationShader{0};

    // =========================================================================
    // Recursos OpenGL - Piso con sombras
    // =========================================================================

    GLuint m_floorVAO{0};
    GLuint m_floorVBO{0};
    GLuint m_floorShader{0};
    bool m_showFloor{true};

    // =========================================================================
    // Constantes
    // =========================================================================

    static constexpr int CYLINDER_SEGMENTS = 8;
    static constexpr int CYLINDER_VERTEX_COUNT = (CYLINDER_SEGMENTS + 1) * 2;
};

#endif  // TURTLE_GRAPHICS_H
