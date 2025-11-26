/**
 * @file UI.cpp
 * @brief Implementacion de la clase de manejo de interfaz de usuario.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#include "UI.h"

#include <cstring>

#include "Colors.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "lsystem/LSystem.h"
#include "lsystem/TurtleGraphics.h"

// =============================================================================
// Presets de L-System - Basados en "The Algorithmic Beauty of Plants"
// =============================================================================
// Reglas probadas y verificadas del libro de Prusinkiewicz & Lindenmayer.
// Cada preset produce resultados visualmente atractivos.
// Incluye arboles 3D, flores, helechos, y fractales 2D clasicos.

static const LSystemPreset PRESETS[] = {
    // =========================================================================
    // ARBOLES 3D REALISTAS - Del libro ABOP (Algorithmic Beauty of Plants)
    // =========================================================================

    // Figura 1.24(d) del libro - Arbol tipo Honda con hojas
    {"Pino 3D", "Arbol conifero con hojas verdes", "A", "A->F[&FLL!A]/////[&FLL!A]///////[&FLL!A]",
     22.5F, 6, true, true},

    // Variacion del arbol Honda - mas denso con hojas
    {"Abeto 3D", "Arbol conifero denso con follaje", "A", "A->F[&FLLA]////[&FLLA]////[&FLLA]",
     25.7F, 7, true, true},

    // Arbol con estructura tipo roble con muchas hojas
    {"Roble 3D", "Arbol robusto con copa frondosa", "A", "A->F[^FLLA]//[^FLLA]//[^FLLA]//[^FLLA]",
     30.0F, 6, true, true},

    // Arbol con ramas alternadas y hojas colgantes
    {"Sauce 3D", "Arbol con hojas caidas elegantes", "A",
     "A->F[&F[&FLLA]//[&FLLA]]////F[&F[&FLLA]//[&FLLA]]", 22.5F, 5, true, true},

    // Magnolia - arbol con flores grandes en estructura 3D
    {"Magnolia 3D", "Arbol con flores grandes", "A", "A->FF[&FKK!A]////[&FKK!A]////[&FKK!A]", 28.0F,
     5, true, true},

    // Cerezo japones - estructura 3D ramificada con muchas flores
    {"Cerezo 3D", "Arbol japones con flores rosadas", "A", "A->F[&FKK!A]////[&FKK!A]////[&FKK!A]",
     30.0F, 5, true, true},

    // =========================================================================
    // PLANTAS 2D CLASICAS - Wikipedia/ABOP Examples
    // =========================================================================

    // Helecho fractal - Figura clasica del libro ABOP (Example 7 Wikipedia)
    {"Helecho 2D", "Planta fractal clasica estilo Barnsley", "X", "X->F+[[X]-X]-F[-FX]+X,F->FF",
     25.0F, 6, false, false},

    // Arbol binario simetrico
    {"Arbol Binario 2D", "Bifurcacion perfecta", "F", "F->FF+[+F-F-F]-[-F+F+F]", 22.5F, 4, false,
     false},

    // Arbusto denso 2D
    {"Arbusto 2D", "Arbusto ramificado natural", "F", "F->F[+F]F[-F][F]", 20.0F, 5, false, false},

    // Planta con flores 2D
    {"Flor 2D", "Planta con flores en las puntas", "X", "X->F[+XK][-XK]FXK,F->FF", 25.7F, 5, false,
     false},

    // =========================================================================
    // FRACTALES GEOMETRICOS 2D
    // =========================================================================

    // Curva de Koch
    {"Curva Koch 2D", "Copo de nieve fractal", "F", "F->F+F--F+F", 60.0F, 4, false, false},

    // Triangulo de Sierpinski
    {"Sierpinski 2D", "Triangulo fractal clasico", "F-G-G", "F->F-G+F+G-F,G->GG", 120.0F, 5, false,
     false},

    // Curva del Dragon
    {"Dragon 2D", "Curva del dragon fractal", "FX", "X->X+YF+,Y->-FX-Y", 90.0F, 12, false, false},

    // Curva de Hilbert
    {"Hilbert 2D", "Curva que llena el espacio", "X", "X->-YF+XFX+FY-,Y->+XF-YFY-FX+", 90.0F, 5,
     false, false},

    // =========================================================================
    // ARBOLES 3D CON FLORES
    // =========================================================================

    // Manzano en flor - estructura frutal 3D con ramas y flores
    {"Manzano 3D", "Arbol frutal con flores y hojas", "A",
     "A->F[&FLLKK!A]////[&FLLKK!A]////[&FLLKK!A]", 25.0F, 5, true, true},

    // Arbol primaveral completo
    {"Primavera 3D", "Arbol con hojas y flores mezcladas", "A",
     "A->F[&FLLK!A]////[&FKLL!A]////[&FLKL!A]", 25.0F, 5, true, true},

    // Bonsai - arbol pequeno y artistico
    {"Bonsai 3D", "Arbol pequeno estilo japones", "A", "A->F[&^FLL!A]//[&&FLL!A]////[&^FLL!A]",
     35.0F, 5, true, true},

    // Arbol de navidad con decoraciones
    {"Navidad 3D", "Pino decorado con ornamentos", "A",
     "A->F[&FKKLL!A]/////[&FLLKK!A]///////[&FKKLK!A]", 20.0F, 6, true, true},
};

static constexpr int NUM_PRESETS = sizeof(PRESETS) / sizeof(PRESETS[0]);

// =============================================================================
// Constructor / Destructor
// =============================================================================

UI::UI(GLFWwindow* window, const char* glslVersion) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Establecer tema oscuro con colores personalizados
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0F;
    style.FrameRounding = 4.0F;
    style.GrabRounding = 4.0F;

    // Inicializar backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    // Establecer color de fondo desde Colors.h
    constexpr auto bgColor = Colors::Nord::POLAR_NIGHT_0;
    m_backgroundColor[0] = bgColor.r;
    m_backgroundColor[1] = bgColor.g;
    m_backgroundColor[2] = bgColor.b;
    m_backgroundColor[3] = bgColor.a;

    // Cargar primer preset
    loadPreset(0);
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// =============================================================================
// Manejo de Frames
// =============================================================================

void UI::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// =============================================================================
// Ventana de Depuracion
// =============================================================================

void UI::renderDebugWindow(GLFWwindow* window) {
    ImGui::Begin("Info de Depuracion");

    // Seccion de rendimiento
    ImGui::SeparatorText("Rendimiento");
    ImGui::Text("FPS: %.1f", m_io->Framerate);
    ImGui::Text("Frame Time: %.3f ms", 1000.0F / m_io->Framerate);

    // Window info
    ImGui::SeparatorText("Pantalla");
    int fbWidth = 0, fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    ImGui::Text("Resolucion: %d x %d", fbWidth, fbHeight);
    ImGui::Text("Relacion de Aspecto: %.3f",
                static_cast<float>(fbWidth) / static_cast<float>(fbHeight));

    // Informacion de OpenGL
    ImGui::SeparatorText("OpenGL");
    ImGui::Text("Version: %s", glGetString(GL_VERSION));
    ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));

    // Color de fondo
    ImGui::SeparatorText("Ajustes");
    ImGui::ColorEdit4("Fondo", m_backgroundColor);

    ImGui::End();
}

// =============================================================================
// Ventana de Control de L-System
// =============================================================================

void UI::renderLSystemWindow(TurtleGraphics& turtle, LSystem& lsystem,
                             const std::function<void()>& onGenerate) {
    ImGui::Begin("Generador de L-System", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // -------------------------------------------------------------------------
    // Seleccion de Preset
    // -------------------------------------------------------------------------
    ImGui::SeparatorText("Presets");

    if (ImGui::BeginCombo("Seleccionar Preset", PRESETS[m_currentPreset].name)) {
        for (int i = 0; i < NUM_PRESETS; ++i) {
            bool isSelected = (m_currentPreset == i);
            if (ImGui::Selectable(PRESETS[i].name, isSelected)) {
                m_currentPreset = i;
                loadPreset(i);
                applyPresetVisuals(turtle, PRESETS[i]);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
            // Mostrar tooltip con descripcion
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("%s", PRESETS[i].description);
                ImGui::EndTooltip();
            }
        }
        ImGui::EndCombo();
    }

    // Mostrar descripcion del preset actual
    ImGui::TextWrapped("%s", PRESETS[m_currentPreset].description);
    ImGui::Spacing();

    // -------------------------------------------------------------------------
    // Parametros de L-System
    // -------------------------------------------------------------------------
    ImGui::SeparatorText("Parametros de L-System");

    ImGui::Text("Axioma:");
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##axiom", m_axiom, sizeof(m_axiom));

    ImGui::Text("Reglas de Produccion (formato: X->reemplazo):");
    ImGui::SetNextItemWidth(-1);
    ImGui::InputTextMultiline("##rules", m_rules, sizeof(m_rules), ImVec2(-1, 100));

    ImGui::SliderFloat("Angulo", &m_angle, 1.0F, 120.0F, "%.1f grados");
    ImGui::SliderInt("Generaciones", &m_generations, 1, 10);

    if (m_generations > 7) {
        ImGui::TextColored(ImVec4(1.0F, 0.6F, 0.0F, 1.0F), "Advertencia: Puede ser lento!");
    }

    // -------------------------------------------------------------------------
    // Modo de Renderizado
    // -------------------------------------------------------------------------
    ImGui::SeparatorText("Modo de Renderizado");

    if (ImGui::Checkbox("Rotaciones 3D", &m_is3DMode)) {
        turtle.set3DMode(m_is3DMode);
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("2D: +/- rotan en plano XY");
        ImGui::Text("3D: +/- yaw, &/^ pitch, \\ / roll");
        ImGui::EndTooltip();
    }

    if (ImGui::Checkbox("Cilindros 3D", &m_useCylinders)) {
        turtle.setRenderMode(m_useCylinders ? RenderMode::Cylinders : RenderMode::Lines);
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("Lineas: Renderizado rapido con GL_LINES");
        ImGui::Text("Cilindros: Ramas 3D realistas con iluminacion");
        ImGui::EndTooltip();
    }

    // -------------------------------------------------------------------------
    // Boton de Generar
    // -------------------------------------------------------------------------
    ImGui::Spacing();
    if (ImGui::Button("Generar", ImVec2(-1, 35))) {
        // Parsear y aplicar reglas de L-System
        lsystem.setAxiom(m_axiom);
        lsystem.clearRules();

        std::string rulesStr(m_rules);
        size_t pos = 0;
        while (pos < rulesStr.size()) {
            // Buscar fin de linea (soporta \n y , como separadores)
            size_t lineEndNewline = rulesStr.find('\n', pos);
            size_t lineEndComma = rulesStr.find(',', pos);
            size_t lineEnd = std::min(lineEndNewline, lineEndComma);
            if (lineEnd == std::string::npos) {
                lineEnd = rulesStr.size();
            }

            std::string line = rulesStr.substr(pos, lineEnd - pos);
            pos = lineEnd + 1;

            // Remover espacios al inicio y final
            size_t start = line.find_first_not_of(" \t");
            if (start != std::string::npos) {
                line = line.substr(start);
            }

            size_t arrowPos = line.find("->");
            if (arrowPos != std::string::npos && arrowPos > 0) {
                char symbol = line[0];
                std::string replacement = line.substr(arrowPos + 2);
                lsystem.addRule(symbol, replacement);
            }
        }

        lsystem.setAngle(m_angle);
        lsystem.generate(m_generations);
        turtle.interpret(lsystem.getString(), m_angle);

        if (onGenerate) {
            onGenerate();
        }
    }

    // -------------------------------------------------------------------------
    // Apariencia Visual
    // -------------------------------------------------------------------------
    ImGui::SeparatorText("Apariencia");

    float stepSize = turtle.getStepSize();
    if (ImGui::SliderFloat("Largo de Rama", &stepSize, 0.01F, 0.3F)) {
        turtle.setStepSize(stepSize);
    }

    float width = turtle.getInitialWidth();
    if (ImGui::SliderFloat("Ancho de Rama", &width, 0.002F, 0.05F)) {
        turtle.setInitialWidth(width);
    }

    float decay = turtle.getWidthDecay();
    if (ImGui::SliderFloat("Decaimiento de Ancho", &decay, 0.5F, 1.0F)) {
        turtle.setWidthDecay(decay);
    }

    float leafSize = turtle.getLeafSize();
    if (ImGui::SliderFloat("Tamano de Hoja", &leafSize, 0.01F, 0.2F)) {
        turtle.setLeafSize(leafSize);
    }

    // -------------------------------------------------------------------------
    // Colores
    // -------------------------------------------------------------------------
    ImGui::SeparatorText("Colores");

    glm::vec3 branchCol = turtle.getBranchColor();
    m_branchColor[0] = branchCol.r;
    m_branchColor[1] = branchCol.g;
    m_branchColor[2] = branchCol.b;
    if (ImGui::ColorEdit3("Rama", m_branchColor)) {
        turtle.setBranchColor(glm::vec3(m_branchColor[0], m_branchColor[1], m_branchColor[2]));
    }

    glm::vec3 leafCol = turtle.getLeafColor();
    m_leafColor[0] = leafCol.r;
    m_leafColor[1] = leafCol.g;
    m_leafColor[2] = leafCol.b;
    if (ImGui::ColorEdit3("Hoja", m_leafColor)) {
        turtle.setLeafColor(glm::vec3(m_leafColor[0], m_leafColor[1], m_leafColor[2]));
    }

    glm::vec3 flowerCol = turtle.getFlowerColor();
    m_flowerColor[0] = flowerCol.r;
    m_flowerColor[1] = flowerCol.g;
    m_flowerColor[2] = flowerCol.b;
    if (ImGui::ColorEdit3("Flor", m_flowerColor)) {
        turtle.setFlowerColor(glm::vec3(m_flowerColor[0], m_flowerColor[1], m_flowerColor[2]));
    }

    // -------------------------------------------------------------------------
    // Opciones de Escena
    // -------------------------------------------------------------------------
    ImGui::SeparatorText("Escena");

    bool showFloor = turtle.getShowFloor();
    if (ImGui::Checkbox("Mostrar Piso", &showFloor)) {
        turtle.setShowFloor(showFloor);
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Muestra un piso verde con sombra suave");
    }

    // -------------------------------------------------------------------------
    // Estadisticas
    // -------------------------------------------------------------------------
    ImGui::SeparatorText("Estadisticas");
    ImGui::Text("Longitud de Cadena: %zu", lsystem.getString().size());
    ImGui::Text("Ramas: %zu", turtle.getBranchCount());
    ImGui::Text("Decoraciones: %zu", turtle.getDecorationCount());

    ImGui::End();
}

// =============================================================================
// Ventana de Controles de Camara
// =============================================================================

void UI::renderCameraWindow(float& distance, float& angleX, float& angleY) {
    ImGui::Begin("Camara", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SeparatorText("Camara Orbital");
    ImGui::SliderFloat("Distancia", &distance, 0.5F, 10.0F);
    ImGui::SliderFloat("Horizontal", &angleX, -180.0F, 180.0F);
    ImGui::SliderFloat("Vertical", &angleY, -89.0F, 89.0F);

    if (ImGui::Button("Reiniciar Vista", ImVec2(-1, 0))) {
        distance = 3.5F;
        angleX = 0.0F;
        angleY = 20.0F;
    }

    ImGui::SeparatorText("Controles");
    ImGui::TextWrapped("Arrastrar izquierdo: Rotar\nScroll: Zoom");

    ImGui::End();
}

// =============================================================================
// Metodos Privados
// =============================================================================

void UI::loadPreset(int index) {
    if (index < 0 || index >= NUM_PRESETS)
        return;

    const auto& preset = PRESETS[index];

    strncpy(m_axiom, preset.axiom, sizeof(m_axiom) - 1);
    m_axiom[sizeof(m_axiom) - 1] = '\0';

    strncpy(m_rules, preset.rules, sizeof(m_rules) - 1);
    m_rules[sizeof(m_rules) - 1] = '\0';

    m_angle = preset.angle;
    m_generations = preset.generations;
    m_is3DMode = preset.is3D;
    m_useCylinders = preset.useCylinders;
}

void UI::applyPresetVisuals(TurtleGraphics& turtle, const LSystemPreset& preset) {
    turtle.set3DMode(preset.is3D);
    turtle.setRenderMode(preset.useCylinders ? RenderMode::Cylinders : RenderMode::Lines);
}
