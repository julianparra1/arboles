/*
 * =================================================================
 * ARCHIVO: UI.h
 * CONTENIDO: Declaracion de la clase UI.
 * DESCRIPCION:
 * Este archivo define la clase UI que maneja toda la interfaz
 * de usuario usando ImGui. Proporciona ventanas de debug y controles.
 *
 * AUTOR: Julian Parra
 * CURSO: Graficacion (2025)
 * PROYECTO: Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 * =================================================================
 */

#ifndef UI_H
#define UI_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "imgui.h"

/**
 * @class UI
 * @brief Maneja toda la interfaz de usuario con ImGui
 */
class UI {
public:
    /**
     * @brief Constructor
     * @param window Ventana de GLFW
     * @param glslVersion Version de GLSL para ImGui
     */
    UI(GLFWwindow* window, const char* glslVersion);

    /**
     * @brief Destructor
     */
    ~UI();

    // Deshabilitar copia
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    /**
     * @brief Inicia un nuevo frame de ImGui
     */
    void beginFrame();

    /**
     * @brief Finaliza y renderiza el frame de ImGui
     */
    void endFrame();

    /**
     * @brief Renderiza la ventana de debug
     * @param window Ventana de GLFW
     */
    void renderDebugWindow(GLFWwindow* window);

    /**
     * @brief Obtiene el color de fondo actual
     * @return Array de 4 floats (RGBA)
     */
    const float* getBackgroundColor() const {
        return m_backgroundColor;
    }

    /**
     * @brief Obtiene la estructura ImGuiIO
     * @return Referencia a ImGuiIO
     */
    ImGuiIO& getIO() {
        return *m_io;
    }

private:
    ImGuiIO* m_io{nullptr};      ///< ImGui IO
    float m_backgroundColor[4];  ///< Color de fondo (RGBA)
};

#endif  // UI_H
