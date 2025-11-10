/*
 * =================================================================
 * ARCHIVO: UI.cpp
 * CONTENIDO: Implementacion de la clase UI.
 * DESCRIPCION:
 * Este archivo implementa la funcionalidad de la interfaz de usuario,
 * incluyendo ventanas de debug e informacion.
 *
 * AUTOR: Julian Parra
 * CURSO: Graficacion (2025)
 * PROYECTO: Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 * =================================================================
 */

#include "UI.h"

#include "Colors.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/*
 * @brief Constructor
 */
UI::UI(GLFWwindow* window, const char* glslVersion) {
    // Inicializar ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Establecer estilo
    ImGui::StyleColorsDark();

    // Inicializar backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    // Inicializar color de fondo por defecto
    constexpr auto defaultColor = Colors::Nord::POLAR_NIGHT_0;
    m_backgroundColor[0] = defaultColor.r;
    m_backgroundColor[1] = defaultColor.g;
    m_backgroundColor[2] = defaultColor.b;
    m_backgroundColor[3] = defaultColor.a;
}

/*
 * @brief Destructor
 */
UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/*
 * @brief Inicia un nuevo frame
 */
void UI::beginFrame() {  // NOLINT(readability-convert-member-functions-to-static)
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

/*
 * @brief Finaliza y renderiza el frame
 */
void UI::endFrame() {  // NOLINT(readability-convert-member-functions-to-static)
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/*
 * @brief Renderiza la ventana de debug
 */
void UI::renderDebugWindow(GLFWwindow* window) {
    ImGui::Begin("Debug Info");

    // Seccion de rendimiento
    ImGui::SeparatorText("Performance");
    ImGui::Text("FPS: %.1f", m_io->Framerate);
    constexpr float MS_PER_SECOND = 1000.0F;
    ImGui::Text("Frame time: %.3f ms", MS_PER_SECOND / m_io->Framerate);
    ImGui::Text("Delta time: %.3f ms", m_io->DeltaTime * MS_PER_SECOND);

    // Informacion de ventana
    ImGui::SeparatorText("Window");
    int fbWidth = 0;
    int fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    ImGui::Text("Framebuffer: %d x %d", fbWidth, fbHeight);
    ImGui::Text("Display size: %.0f x %.0f", m_io->DisplaySize.x, m_io->DisplaySize.y);
    ImGui::Text("Aspect ratio: %.3f", static_cast<float>(fbWidth) / static_cast<float>(fbHeight));

    // Informacion de OpenGL
    ImGui::SeparatorText("OpenGL");
    ImGui::Text("Version: %s", glGetString(GL_VERSION));
    ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
    ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
    ImGui::Text("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Informacion de entrada
    ImGui::SeparatorText("Input");
    ImGui::Text("Mouse pos: (%.0f, %.0f)", m_io->MousePos.x, m_io->MousePos.y);
    ImGui::Text("Mouse down: [%d %d %d %d %d]", static_cast<int>(m_io->MouseDown[0]),
                static_cast<int>(m_io->MouseDown[1]), static_cast<int>(m_io->MouseDown[2]),
                static_cast<int>(m_io->MouseDown[3]), static_cast<int>(m_io->MouseDown[4]));
    ImGui::Text("Mouse wheel: %.1f", m_io->MouseWheel);

    // Control de color de fondo
    ImGui::SeparatorText("Settings");
    ImGui::ColorEdit4("Background", m_backgroundColor);

    ImGui::End();
}
