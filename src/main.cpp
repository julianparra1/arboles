/*
 * =================================================================
 * ARCHIVO: main.cpp
 * CONTENIDO: Punto de entrada de la aplicacion.
 * DESCRIPCION:
 * Este archivo contiene el main() que inicializa GLFW, OpenGL, ImGui
 * y ejecuta el bucle principal de renderizado.
 *
 * AUTOR: Julian Parra
 * CURSO: Graficacion (2025)
 * PROYECTO: Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 * =================================================================
 */

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <array>
#include <iostream>

#include "rendering/Camera.h"
#include "rendering/Shader.h"
#include "ui/UI.h"

// Constantes para dimensiones de ventana
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// Shaders
const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * vec4(aPos, 1.0);\n"
    "   vertexColor = aColor;\n"
    "}\0";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "in vec3 vertexColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(vertexColor, 1.0f);\n"
    "}\0";

// Vertices del triangulo equilatero
// Para un triangulo equilateral con base = 1.0, altura = sqrt(3)/2 ≈ 0.866
std::array<float, 18> vertices = {
    // positions                        // colors
    0.5F,  -0.289F, 0.0F, 1.0F, 0.0F, 0.0F,  // bottom right
    -0.5F, -0.289F, 0.0F, 0.0F, 1.0F, 0.0F,  // bottom left
    0.0F,  0.577F,  0.0F, 0.0F, 0.0F, 1.0F   // top
};

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

int main() {
    // Inicializar GLFW
    if (glfwInit() == 0) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Crear ventana
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Arboles", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Cargar OpenGL
    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Crear shader
    Shader shader(vertexShaderSource, fragmentShaderSource);
    if (!shader.isValid()) {
        std::cerr << "Failed to create shader\n";
        return -1;
    }

    // Crear camara
    Camera camera;
    camera.updateProjection(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Crear UI
    UI userInterface(window, "#version 330");

    // Configurar buffers de OpenGL
    GLuint VAO = 0;
    GLuint VBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    constexpr GLsizei STRIDE = 6;
    constexpr uintptr_t COLOR_OFFSET = 3 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float),
        reinterpret_cast<void*>(COLOR_OFFSET));  // NOLINT(performance-no-int-to-ptr)
    glEnableVertexAttribArray(1);

    // Bucle principal
    while (glfwWindowShouldClose(window) == 0) {
        processInput(window);

        // Obtener tamaño de framebuffer y actualizar camara
        int fbWidth = 0;
        int fbHeight = 0;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        camera.updateProjection(fbWidth, fbHeight);

        // Iniciar frame de UI
        userInterface.beginFrame();

        // Renderizar ventana de debug (solo registra comandos)
        userInterface.renderDebugWindow(window);

        // Limpiar pantalla
        const float* bgColor = userInterface.getBackgroundColor();
        glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        // Renderizar triangulo
        shader.use();
        shader.setMat4("projection", camera.getProjectionMatrix());

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Finalizar y renderizar UI (encima de todo)
        userInterface.endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpieza
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
