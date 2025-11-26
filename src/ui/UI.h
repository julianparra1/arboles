/**
 * @file UI.h
 * @brief Manejo de interfaz de usuario usando Dear ImGui.
 *
 * Proporciona ventanas para configuracion de L-System, controles de camara,
 * informacion de depuracion y ajustes visuales. Incluye presets curados
 * para visualizaciones hermosas de plantas en 2D y 3D.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#ifndef UI_H
#define UI_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>

#include <functional>
#include <glm/glm.hpp>
#include <string>

// Forward declarations
class TurtleGraphics;
class LSystem;

/**
 * @brief Preset completo de L-System con ajustes visuales.
 */
struct LSystemPreset {
    const char* name;         ///< Nombre a mostrar
    const char* description;  ///< Descripcion breve
    const char* axiom;        ///< Cadena inicial
    const char* rules;        ///< Reglas de produccion (separadas por salto de linea)
    float angle;              ///< Angulo de rotacion en grados
    int generations;          ///< Numero de iteraciones
    bool is3D;                ///< Usar modo de rotacion 3D
    bool useCylinders;        ///< Usar renderizado de cilindros (vs lineas)
};

/**
 * @class UI
 * @brief Maneja la interfaz grafica de usuario usando Dear ImGui.
 *
 * Controla todo el renderizado de UI incluyendo configuracion de L-System,
 * controles de camara e informacion de depuracion.
 */
class UI {
public:
    /**
     * @brief Inicializa el sistema de UI.
     * @param window Handle de ventana GLFW.
     * @param glslVersion Cadena de version GLSL para backend de ImGui.
     */
    UI(GLFWwindow* window, const char* glslVersion);

    ~UI();

    // No copiable
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    /**
     * @brief Inicia un nuevo frame de ImGui. Llamar al inicio del bucle de renderizado.
     */
    void beginFrame();

    /**
     * @brief Finaliza y renderiza el frame de ImGui. Llamar al final del bucle de renderizado.
     */
    void endFrame();

    /**
     * @brief Renderiza la ventana de informacion de depuracion.
     * @param window Ventana GLFW para consultas de framebuffer.
     */
    void renderDebugWindow(GLFWwindow* window);

    /**
     * @brief Renderiza la ventana de control de L-System.
     * @param turtle Referencia al renderizador de graficos de tortuga.
     * @param lsystem Referencia al generador de L-System.
     * @param onGenerate Callback invocado cuando se regenera el arbol.
     */
    void renderLSystemWindow(TurtleGraphics& turtle, LSystem& lsystem,
                             const std::function<void()>& onGenerate);

    /**
     * @brief Renderiza la ventana de controles de camara.
     */
    void renderCameraWindow(float& distance, float& angleX, float& angleY);

    /**
     * @brief Obtiene el color de fondo actual.
     * @return Puntero a arreglo de floats RGBA.
     */
    const float* getBackgroundColor() const {
        return m_backgroundColor;
    }

    /**
     * @brief Obtiene referencia a ImGuiIO para consultas de entrada.
     */
    ImGuiIO& getIO() {
        return *m_io;
    }

    // Accesores para parametros actuales de L-System
    const char* getAxiom() const {
        return m_axiom;
    }
    const char* getRules() const {
        return m_rules;
    }
    float getAngle() const {
        return m_angle;
    }
    int getGenerations() const {
        return m_generations;
    }

private:
    void loadPreset(int index);
    void applyPresetVisuals(TurtleGraphics& turtle, const LSystemPreset& preset);

    ImGuiIO* m_io{nullptr};
    float m_backgroundColor[4]{0.08F, 0.09F, 0.11F, 1.0F};

    // L-System parameters
    char m_axiom[256]{};
    char m_rules[2048]{};
    float m_angle{25.0F};
    int m_generations{4};
    int m_currentPreset{0};

    // Rendering parameters
    float m_branchColor[3]{0.45F, 0.30F, 0.15F};
    float m_leafColor[3]{0.2F, 0.65F, 0.2F};
    float m_flowerColor[3]{0.95F, 0.4F, 0.6F};
    bool m_is3DMode{false};
    bool m_useCylinders{true};
};

#endif  // UI_H
