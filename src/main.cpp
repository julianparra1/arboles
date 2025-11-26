/**
 * @file main.cpp
 * @brief Punto de entrada de la aplicacion del visualizador de plantas L-System.
 *
 * Inicializa GLFW, OpenGL, Dear ImGui y ejecuta el bucle principal de renderizado.
 * Proporciona visualizacion interactiva 3D de plantas generadas por L-System con
 * controles de camara orbital.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <iostream>

#include "lsystem/LSystem.h"
#include "lsystem/TurtleGraphics.h"
#include "rendering/Camera.h"
#include "ui/UI.h"

// =============================================================================
// Constantes de Configuracion
// =============================================================================

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr float INITIAL_CAMERA_DISTANCE = 3.5F;
constexpr float INITIAL_CAMERA_ANGLE_Y = 20.0F;

// =============================================================================
// Estado Global para Manejo de Entrada
// =============================================================================

static bool g_mousePressed = false;
static double g_lastMouseX = 0.0;
static double g_lastMouseY = 0.0;
static float g_cameraDistance = INITIAL_CAMERA_DISTANCE;
static float g_cameraAngleX = 0.0F;
static float g_cameraAngleY = INITIAL_CAMERA_ANGLE_Y;

// =============================================================================
// Callbacks de GLFW
// =============================================================================

void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset) {
    g_cameraDistance -= static_cast<float>(yoffset) * 0.2F;
    g_cameraDistance = std::clamp(g_cameraDistance, 0.3F, 10.0F);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int /*mods*/) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        ImGuiIO& io = ImGui::GetIO();
        if (action == GLFW_PRESS && !io.WantCaptureMouse) {
            g_mousePressed = true;
            glfwGetCursorPos(window, &g_lastMouseX, &g_lastMouseY);
        } else if (action == GLFW_RELEASE) {
            g_mousePressed = false;
        }
    }
}

void cursorPositionCallback(GLFWwindow* /*window*/, double xpos, double ypos) {
    if (g_mousePressed) {
        float deltaX = static_cast<float>(xpos - g_lastMouseX);
        float deltaY = static_cast<float>(ypos - g_lastMouseY);

        g_cameraAngleX += deltaX * 0.3F;
        g_cameraAngleY += deltaY * 0.3F;
        g_cameraAngleY = std::clamp(g_cameraAngleY, -89.0F, 89.0F);

        g_lastMouseX = xpos;
        g_lastMouseY = ypos;
    }
}

void keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// =============================================================================
// Punto de Entrada Principal
// =============================================================================

int main() {
    // -------------------------------------------------------------------------
    // Inicializar GLFW
    // -------------------------------------------------------------------------
    if (glfwInit() == GLFW_FALSE) {
        std::cerr << "ERROR: Fallo al inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);  // Habilitar MSAA

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Arboles - The Algorithmic Beauty of Plants",
                         nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "ERROR: Fallo al crear ventana GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetKeyCallback(window, keyCallback);

    // -------------------------------------------------------------------------
    // Inicializar OpenGL via GLAD
    // -------------------------------------------------------------------------
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        std::cerr << "ERROR: Fallo al inicializar GLAD\n";
        glfwTerminate();
        return -1;
    }

    std::cout << "=== Informacion de OpenGL ===\n";
    std::cout << "Version:  " << glGetString(GL_VERSION) << '\n';
    std::cout << "GLSL:     " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
    std::cout << "=============================\n\n";

    // Habilitar caracteristicas de OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // -------------------------------------------------------------------------
    // Inicializar Componentes de Aplicacion
    // -------------------------------------------------------------------------
    Camera camera;
    camera.updatePerspective(WINDOW_WIDTH, WINDOW_HEIGHT);

    UI userInterface(window, "#version 330");

    LSystem lsystem;

    TurtleGraphics turtle;
    if (!turtle.initialize()) {
        std::cerr << "ERROR: Fallo al inicializar TurtleGraphics\n";
        glfwTerminate();
        return -1;
    }

    // -------------------------------------------------------------------------
    // Generar Planta Inicial (primer preset: Pino 3D)
    // -------------------------------------------------------------------------
    lsystem.setAxiom("A");
    lsystem.addRule('A', "F[&FL!A]/////[&FL!A]///////[&FL!A]");
    lsystem.setAngle(22.5F);
    lsystem.generate(6);
    turtle.set3DMode(true);
    turtle.setRenderMode(RenderMode::Cylinders);
    turtle.interpret(lsystem.getString(), lsystem.getAngle());

    std::cout << "Planta inicial generada: " << turtle.getBranchCount() << " ramas\n\n";

    // Posicion de luz para renderizado 3D
    glm::vec3 lightPos(5.0F, 8.0F, 5.0F);

    // -------------------------------------------------------------------------
    // Bucle Principal de Renderizado
    // -------------------------------------------------------------------------
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        // Obtener tamano de framebuffer para camara
        int fbWidth = 0, fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        // Actualizar camara
        camera.updatePerspective(fbWidth, fbHeight);
        camera.updateView(g_cameraDistance, g_cameraAngleX, g_cameraAngleY);

        // Iniciar frame de UI
        userInterface.beginFrame();

        // Renderizar ventanas de UI
        userInterface.renderLSystemWindow(turtle, lsystem, [&turtle]() {
            std::cout << "Planta regenerada: " << turtle.getBranchCount() << " ramas, "
                      << turtle.getDecorationCount() << " decoraciones\n";
        });
        userInterface.renderCameraWindow(g_cameraDistance, g_cameraAngleX, g_cameraAngleY);
        userInterface.renderDebugWindow(window);

        // Limpiar pantalla
        const float* bgColor = userInterface.getBackgroundColor();
        glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderizar la planta
        turtle.render(camera.getViewMatrix(), camera.getProjectionMatrix(), lightPos);

        // Finalizar frame de UI (renderiza ImGui encima)
        userInterface.endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // -------------------------------------------------------------------------
    // Limpieza
    // -------------------------------------------------------------------------
    glfwTerminate();
    std::cout << "\nAplicacion terminada exitosamente.\n";
    return 0;
}
