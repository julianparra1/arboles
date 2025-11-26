/**
 * @file TurtleGraphics.cpp
 * @brief Implementacion del renderizador unificado de graficos de tortuga.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#include "TurtleGraphics.h"

#include <array>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// =============================================================================
// Shader Sources - Line Rendering
// =============================================================================

static const char* LINE_VERTEX_SHADER = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 fragColor;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    fragColor = aColor;
}
)";

static const char* LINE_FRAGMENT_SHADER = R"(
#version 330 core
in vec3 fragColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(fragColor, 1.0);
}
)";

// =============================================================================
// Shader Sources - Cylinder Rendering (3D branches)
// =============================================================================

static const char* CYLINDER_VERTEX_SHADER = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Instance data
layout (location = 2) in vec3 iStart;
layout (location = 3) in vec3 iEnd;
layout (location = 4) in float iRadiusStart;
layout (location = 5) in float iRadiusEnd;
layout (location = 6) in vec3 iColor;

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;

void main() {
    // Calculate branch direction and length
    vec3 dir = iEnd - iStart;
    float len = length(dir);
    if (len < 0.0001) {
        dir = vec3(0.0, 1.0, 0.0);
        len = 1.0;
    }
    dir = normalize(dir);

    // Create rotation matrix to align cylinder with branch direction
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, dir);
    float rightLen = length(right);

    mat3 rotMat;
    if (rightLen < 0.0001) {
        // dir is parallel to up, use different reference
        right = normalize(cross(vec3(1.0, 0.0, 0.0), dir));
        vec3 forward = cross(dir, right);
        rotMat = mat3(right, dir, forward);
    } else {
        right = normalize(right);
        vec3 forward = cross(dir, right);
        rotMat = mat3(right, dir, forward);
    }

    // Interpolate radius along the cylinder (aPos.y is 0 at bottom, 1 at top)
    float radius = mix(iRadiusStart, iRadiusEnd, aPos.y);

    // Transform vertex position
    vec3 localPos = vec3(aPos.x * radius, aPos.y * len, aPos.z * radius);
    vec3 worldPos = rotMat * localPos + iStart;

    FragPos = worldPos;
    Normal = rotMat * aNormal;
    Color = iColor;

    gl_Position = projection * view * vec4(worldPos, 1.0);
}
)";

static const char* CYLINDER_FRAGMENT_SHADER = R"(
#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Luz ambiente calida
    vec3 warmAmbient = vec3(1.0, 0.95, 0.9);
    float ambientStrength = 0.35;
    vec3 ambient = ambientStrength * Color * warmAmbient;

    // Diffuse con wrap lighting para suavizar
    float NdotL = dot(norm, lightDir);
    float wrapDiffuse = max((NdotL + 0.5) / 1.5, 0.0);  // Wrap lighting
    vec3 diffuse = wrapDiffuse * Color;

    // Specular con Blinn-Phong
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfDir), 0.0), 32.0);
    vec3 specular = 0.15 * spec * vec3(1.0, 0.98, 0.95);

    // Rim lighting para dar profundidad
    float rim = 1.0 - max(dot(viewDir, norm), 0.0);
    rim = pow(rim, 3.0);
    vec3 rimColor = rim * Color * 0.2;

    // Fill light desde abajo (luz de relleno)
    vec3 fillLightDir = normalize(vec3(0.0, -1.0, 0.0));
    float fillDiff = max(dot(norm, fillLightDir) * 0.5 + 0.5, 0.0) * 0.15;
    vec3 fillLight = fillDiff * Color * vec3(0.6, 0.7, 1.0);  // Tinte azulado

    vec3 result = ambient + diffuse * 0.6 + specular + rimColor + fillLight;

    // Tonemap simple para evitar sobre-exposicion
    result = result / (result + vec3(1.0));

    FragColor = vec4(result, 1.0);
}
)";

// =============================================================================
// Shader Sources - Decoration Rendering (leaves/flowers)
// =============================================================================

static const char* DECORATION_VERTEX_SHADER = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Instance data
layout (location = 2) in vec3 iPosition;
layout (location = 3) in mat4 iOrientation;  // Uses locations 3, 4, 5, 6
layout (location = 7) in vec3 iColor;
layout (location = 8) in float iSize;

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;
out vec2 LocalPos;  // Para efectos en fragment shader

void main() {
    vec3 scaledPos = aPos * iSize;
    vec4 worldPos4 = iOrientation * vec4(scaledPos, 1.0);
    vec3 worldPos = worldPos4.xyz + iPosition;

    FragPos = worldPos;
    Normal = mat3(iOrientation) * aNormal;
    Color = iColor;
    LocalPos = aPos.xy;  // Coordenadas locales para efectos

    gl_Position = projection * view * vec4(worldPos, 1.0);
}
)";

static const char* DECORATION_FRAGMENT_SHADER = R"(
#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec2 LocalPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int decorationType;  // 0 = hoja, 1 = flor

out vec4 FragColor;

void main() {
    vec3 finalColor = Color;
    float alpha = 1.0;

    // Coordenadas locales normalizadas
    float x = LocalPos.x * 2.0;  // -1 a 1
    float y = LocalPos.y * 2.0 - 1.0;  // -1 a 1

    if (decorationType == 0) {
        // =====================================================================
        // HOJA: Forma botanica con venas y degradado natural
        // =====================================================================

        // Forma de hoja (elipse puntiaguda)
        float leafWidth = 0.7 * (1.0 - y * y);  // Mas ancha en el centro
        float leafEdge = abs(x) - leafWidth;

        if (leafEdge > 0.0) discard;  // Fuera de la forma de hoja

        // Suavizar bordes
        float edgeFade = 1.0 - smoothstep(-0.1, 0.0, leafEdge);

        // Vena central
        float centralVein = exp(-abs(x) * 15.0) * 0.4;

        // Venas laterales
        float veinY = fract(y * 4.0 + 0.5);
        float lateralVeins = exp(-abs(x - 0.3 * sin(y * 8.0)) * 8.0) * 0.2;
        lateralVeins += exp(-abs(x + 0.3 * sin(y * 8.0)) * 8.0) * 0.2;

        // Color base con variacion
        vec3 darkGreen = Color * 0.6;
        vec3 lightGreen = Color * 1.3;

        // Degradado desde el centro hacia afuera
        float centerDist = abs(x) / max(leafWidth, 0.01);
        finalColor = mix(darkGreen, lightGreen, centerDist * 0.5);

        // Agregar venas (mas oscuras)
        finalColor = mix(finalColor, darkGreen * 0.7, centralVein + lateralVeins);

        // Borde mas claro (translucido)
        finalColor = mix(finalColor, lightGreen, smoothstep(-0.15, 0.0, leafEdge) * 0.3);

        alpha = edgeFade;

    } else {
        // =====================================================================
        // FLOR: Forma de flor con petalos y centro
        // =====================================================================

        float dist = length(vec2(x, y));
        float angle = atan(y, x);

        // Centro de la flor (pistilo/estambre)
        float centerRadius = 0.18;
        float center = 1.0 - smoothstep(centerRadius - 0.05, centerRadius, dist);

        // Petalos (5 petalos con forma curva)
        float petalCount = 5.0;
        float petalAngle = mod(angle + 3.14159, 6.28318 / petalCount) - 3.14159 / petalCount;
        float petalWidth = cos(petalAngle * petalCount * 0.5) * 0.5 + 0.5;

        // Radio del petalo varia con el angulo
        float petalRadius = 0.4 + petalWidth * 0.5;
        float petalShape = 1.0 - smoothstep(petalRadius - 0.15, petalRadius, dist);

        // No dibujar fuera de la flor
        if (petalShape < 0.05 && center < 0.05) discard;

        // Colores
        vec3 petalColor = Color;
        vec3 petalTip = Color * 0.7;  // Puntas mas oscuras
        vec3 petalBase = mix(Color, vec3(1.0), 0.3);  // Base mas clara
        vec3 centerColor = vec3(1.0, 0.85, 0.2);  // Centro amarillo dorado

        // Degradado en petalos (del centro hacia afuera)
        float petalGradient = smoothstep(centerRadius, petalRadius, dist);
        vec3 currentPetalColor = mix(petalBase, petalTip, petalGradient);

        // Lineas radiales en petalos (estrias)
        float radialLines = sin(angle * 30.0) * 0.5 + 0.5;
        currentPetalColor = mix(currentPetalColor, currentPetalColor * 0.85, radialLines * 0.2 * petalGradient);

        // Combinar centro y petalos
        finalColor = mix(currentPetalColor, centerColor, center);

        // Puntos de polen en el centro
        if (center > 0.5) {
            float pollenDots = sin(angle * 8.0) * sin(dist * 50.0);
            if (pollenDots > 0.7) {
                finalColor = vec3(0.9, 0.7, 0.1);  // Puntos de polen
            }
        }

        alpha = max(petalShape, center);
    }

    // =========================================================================
    // ILUMINACION
    // =========================================================================
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Ambient
    float ambientStrength = 0.45;
    vec3 ambient = ambientStrength * finalColor;

    // Diffuse con two-sided lighting para hojas/petalos
    float diff = abs(dot(norm, lightDir)) * 0.5 + 0.5;
    vec3 diffuse = diff * finalColor;

    // Specular mas fuerte para flores
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float specPower = (decorationType == 1) ? 32.0 : 16.0;
    float specStrength = (decorationType == 1) ? 0.3 : 0.15;
    float spec = pow(max(dot(norm, halfDir), 0.0), specPower);
    vec3 specular = specStrength * spec * vec3(1.0);

    // Subsurface scattering simulado (luz atravesando hojas/petalos)
    float sss = pow(max(dot(-viewDir, lightDir), 0.0), 3.0) * 0.2;
    vec3 subsurface = sss * finalColor;

    vec3 result = ambient * 0.35 + diffuse * 0.55 + specular + subsurface;
    FragColor = vec4(result, alpha);
}
)";

// =============================================================================
// Shader Sources - Floor Rendering (piso con sombras suaves)
// =============================================================================

static const char* FLOOR_VERTEX_SHADER = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 FragPos;

void main() {
    FragPos = aPos;
    TexCoord = aTexCoord;
    gl_Position = projection * view * vec4(aPos, 1.0);
}
)";

static const char* FLOOR_FRAGMENT_SHADER = R"(
#version 330 core
in vec2 TexCoord;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 plantCenter;
uniform float plantRadius;

out vec4 FragColor;

void main() {
    // Color base del piso - degradado suave
    vec2 centered = TexCoord - vec2(0.5);
    float distFromCenter = length(centered);

    // Colores del piso (tierra/cesped suave)
    vec3 innerColor = vec3(0.35, 0.45, 0.25);   // Verde tierra
    vec3 outerColor = vec3(0.25, 0.35, 0.20);   // Verde mas oscuro en bordes
    vec3 baseColor = mix(innerColor, outerColor, smoothstep(0.0, 0.7, distFromCenter));

    // Patron de cesped/textura procedural
    float noise1 = fract(sin(dot(TexCoord * 50.0, vec2(12.9898, 78.233))) * 43758.5453);
    float noise2 = fract(sin(dot(TexCoord * 30.0, vec2(39.346, 11.135))) * 43758.5453);
    float grassPattern = mix(0.9, 1.1, noise1 * 0.5 + noise2 * 0.5);
    baseColor *= grassPattern;

    // Sombra suave de la planta (proyectada desde arriba)
    vec2 plantPos2D = vec2(plantCenter.x, plantCenter.z);
    vec2 fragPos2D = vec2(FragPos.x, FragPos.z);
    float distToPlant = length(fragPos2D - plantPos2D);

    // Sombra circular suave alrededor de la base de la planta
    float shadowRadius = plantRadius * 1.5;
    float shadow = 1.0 - smoothstep(shadowRadius * 0.3, shadowRadius, distToPlant) * 0.4;

    // Sombra direccional basada en la luz
    vec3 lightDir = normalize(lightPos);
    vec2 shadowOffset = -lightDir.xz * plantRadius * 0.5;
    float dirShadowDist = length(fragPos2D - plantPos2D - shadowOffset);
    float dirShadow = 1.0 - smoothstep(shadowRadius * 0.2, shadowRadius * 0.8, dirShadowDist) * 0.3;

    shadow = min(shadow, dirShadow);

    // Ambient occlusion cerca de la planta
    float ao = smoothstep(0.0, plantRadius * 0.5, distToPlant) * 0.3 + 0.7;

    // Iluminacion simple del piso
    vec3 normal = vec3(0.0, 1.0, 0.0);
    float diff = max(dot(normal, normalize(lightPos - FragPos)), 0.0) * 0.3 + 0.7;

    // Color final
    vec3 finalColor = baseColor * shadow * ao * diff;

    // Fade en los bordes para que no se vea el corte
    float edgeFade = 1.0 - smoothstep(0.45, 0.5, distFromCenter);

    FragColor = vec4(finalColor, edgeFade);
}
)";

// =============================================================================
// Constructor / Destructor
// =============================================================================

TurtleGraphics::TurtleGraphics() = default;

TurtleGraphics::~TurtleGraphics() {
    // Clean up line resources
    if (m_lineVAO != 0)
        glDeleteVertexArrays(1, &m_lineVAO);
    if (m_lineVBO != 0)
        glDeleteBuffers(1, &m_lineVBO);
    if (m_lineShader != 0)
        glDeleteProgram(m_lineShader);

    // Clean up cylinder resources
    if (m_cylinderVAO != 0)
        glDeleteVertexArrays(1, &m_cylinderVAO);
    if (m_cylinderVBO != 0)
        glDeleteBuffers(1, &m_cylinderVBO);
    if (m_cylinderInstanceVBO != 0)
        glDeleteBuffers(1, &m_cylinderInstanceVBO);
    if (m_cylinderShader != 0)
        glDeleteProgram(m_cylinderShader);

    // Clean up decoration resources
    if (m_decorationVAO != 0)
        glDeleteVertexArrays(1, &m_decorationVAO);
    if (m_decorationVBO != 0)
        glDeleteBuffers(1, &m_decorationVBO);
    if (m_decorationInstanceVBO != 0)
        glDeleteBuffers(1, &m_decorationInstanceVBO);
    if (m_decorationShader != 0)
        glDeleteProgram(m_decorationShader);

    // Clean up floor resources
    if (m_floorVAO != 0)
        glDeleteVertexArrays(1, &m_floorVAO);
    if (m_floorVBO != 0)
        glDeleteBuffers(1, &m_floorVBO);
    if (m_floorShader != 0)
        glDeleteProgram(m_floorShader);
}

// =============================================================================
// Initialization
// =============================================================================

bool TurtleGraphics::initialize() {
    if (m_initialized) {
        return true;
    }

    if (!compileShaders()) {
        std::cerr << "TurtleGraphics: Failed to compile shaders\n";
        return false;
    }

    // -------------------------------------------------------------------------
    // Setup Line VAO/VBO
    // -------------------------------------------------------------------------
    glGenVertexArrays(1, &m_lineVAO);
    glGenBuffers(1, &m_lineVBO);

    glBindVertexArray(m_lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);

    // Position (3 floats) + Color (3 floats) = 6 floats per vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // -------------------------------------------------------------------------
    // Setup Cylinder VAO/VBO for instanced rendering
    // -------------------------------------------------------------------------
    std::vector<float> cylinderVertices;
    for (int i = 0; i <= CYLINDER_SEGMENTS; ++i) {
        float theta = 2.0F * static_cast<float>(M_PI) * static_cast<float>(i) /
                      static_cast<float>(CYLINDER_SEGMENTS);
        float cosTheta = std::cos(theta);
        float sinTheta = std::sin(theta);

        // Bottom vertex: position + normal
        cylinderVertices.push_back(cosTheta);  // x
        cylinderVertices.push_back(0.0F);      // y (bottom)
        cylinderVertices.push_back(sinTheta);  // z
        cylinderVertices.push_back(cosTheta);  // normal x
        cylinderVertices.push_back(0.0F);      // normal y
        cylinderVertices.push_back(sinTheta);  // normal z

        // Top vertex: position + normal
        cylinderVertices.push_back(cosTheta);  // x
        cylinderVertices.push_back(1.0F);      // y (top)
        cylinderVertices.push_back(sinTheta);  // z
        cylinderVertices.push_back(cosTheta);  // normal x
        cylinderVertices.push_back(0.0F);      // normal y
        cylinderVertices.push_back(sinTheta);  // normal z
    }

    glGenVertexArrays(1, &m_cylinderVAO);
    glGenBuffers(1, &m_cylinderVBO);
    glGenBuffers(1, &m_cylinderInstanceVBO);

    glBindVertexArray(m_cylinderVAO);

    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(cylinderVertices.size() * sizeof(float)),
                 cylinderVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Instance data buffer setup
    glBindBuffer(GL_ARRAY_BUFFER, m_cylinderInstanceVBO);
    constexpr GLsizei instanceStride =
        11 * sizeof(float);  // start(3) + end(3) + r1(1) + r2(1) + color(3)

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, instanceStride, nullptr);  // iStart
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, instanceStride,
                          reinterpret_cast<void*>(3 * sizeof(float)));  // iEnd
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, instanceStride,
                          reinterpret_cast<void*>(6 * sizeof(float)));  // iRadiusStart
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, instanceStride,
                          reinterpret_cast<void*>(7 * sizeof(float)));  // iRadiusEnd
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);

    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, instanceStride,
                          reinterpret_cast<void*>(8 * sizeof(float)));  // iColor
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(6, 1);

    // -------------------------------------------------------------------------
    // Setup Decoration VAO/VBO (leaves/flowers as quads)
    // -------------------------------------------------------------------------
    float decorationVertices[] = {
        // positions            // normals
        -0.5F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.5F,  0.0F, 0.0F, 0.0F, 0.0F, 1.0F,
        0.5F,  1.0F, 0.0F, 0.0F, 0.0F, 1.0F, -0.5F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F,
        0.5F,  1.0F, 0.0F, 0.0F, 0.0F, 1.0F, -0.5F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F,
    };

    glGenVertexArrays(1, &m_decorationVAO);
    glGenBuffers(1, &m_decorationVBO);
    glGenBuffers(1, &m_decorationInstanceVBO);

    glBindVertexArray(m_decorationVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_decorationVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(decorationVertices), decorationVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Instance data: position(3) + orientation(16) + color(3) + size(1) = 23 floats
    glBindBuffer(GL_ARRAY_BUFFER, m_decorationInstanceVBO);
    constexpr GLsizei decorInstanceStride = 23 * sizeof(float);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, decorInstanceStride, nullptr);  // iPosition
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    // iOrientation (mat4 = 4 vec4s at locations 3, 4, 5, 6)
    for (int i = 0; i < 4; ++i) {
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, decorInstanceStride,
                              reinterpret_cast<void*>((3 + i * 4) * sizeof(float)));
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1);
    }

    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, decorInstanceStride,
                          reinterpret_cast<void*>(19 * sizeof(float)));  // iColor
    glEnableVertexAttribArray(7);
    glVertexAttribDivisor(7, 1);

    glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, decorInstanceStride,
                          reinterpret_cast<void*>(22 * sizeof(float)));  // iSize
    glEnableVertexAttribArray(8);
    glVertexAttribDivisor(8, 1);

    glBindVertexArray(0);

    // -------------------------------------------------------------------------
    // Setup Floor
    // -------------------------------------------------------------------------
    setupFloor();

    m_initialized = true;
    std::cout << "TurtleGraphics: Initialized successfully\n";
    return true;
}

bool TurtleGraphics::compileShaders() {
    auto compileShader = [](const char* source, GLenum type) -> GLuint {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == 0) {
            std::array<char, 512> infoLog{};
            glGetShaderInfoLog(shader, 512, nullptr, infoLog.data());
            std::cerr << "Shader compilation error:\n" << infoLog.data() << '\n';
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    };

    auto linkProgram = [](GLuint vertex, GLuint fragment) -> GLuint {
        GLuint program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);

        GLint success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (success == 0) {
            std::array<char, 512> infoLog{};
            glGetProgramInfoLog(program, 512, nullptr, infoLog.data());
            std::cerr << "Shader linking error:\n" << infoLog.data() << '\n';
            glDeleteProgram(program);
            return 0;
        }
        return program;
    };

    // Compile line shaders
    GLuint lineVS = compileShader(LINE_VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint lineFS = compileShader(LINE_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
    if (lineVS == 0 || lineFS == 0)
        return false;
    m_lineShader = linkProgram(lineVS, lineFS);
    glDeleteShader(lineVS);
    glDeleteShader(lineFS);
    if (m_lineShader == 0)
        return false;

    // Compile cylinder shaders
    GLuint cylVS = compileShader(CYLINDER_VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint cylFS = compileShader(CYLINDER_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
    if (cylVS == 0 || cylFS == 0)
        return false;
    m_cylinderShader = linkProgram(cylVS, cylFS);
    glDeleteShader(cylVS);
    glDeleteShader(cylFS);
    if (m_cylinderShader == 0)
        return false;

    // Compile decoration shaders
    GLuint decVS = compileShader(DECORATION_VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint decFS = compileShader(DECORATION_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
    if (decVS == 0 || decFS == 0)
        return false;
    m_decorationShader = linkProgram(decVS, decFS);
    glDeleteShader(decVS);
    glDeleteShader(decFS);
    if (m_decorationShader == 0)
        return false;

    // Compile floor shaders
    GLuint floorVS = compileShader(FLOOR_VERTEX_SHADER, GL_VERTEX_SHADER);
    GLuint floorFS = compileShader(FLOOR_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
    if (floorVS == 0 || floorFS == 0)
        return false;
    m_floorShader = linkProgram(floorVS, floorFS);
    glDeleteShader(floorVS);
    glDeleteShader(floorFS);

    return m_floorShader != 0;
}

// =============================================================================
// L-System Interpretation
// =============================================================================

void TurtleGraphics::interpret(const std::string& lsystemString, float angle) {
    clear();

    // Reset turtle to initial state
    m_currentState = TurtleState{};
    m_currentState.width = m_initialWidth;
    m_currentState.color = m_branchColor;

    // Clear state stack
    while (!m_stateStack.empty()) {
        m_stateStack.pop();
    }

    // Process each command in the L-System string
    for (char cmd : lsystemString) {
        processCommand(cmd, angle);
    }

    // Upload geometry to GPU
    uploadBranchData();
    uploadDecorationData();

    std::cout << "TurtleGraphics: Generated " << m_branches.size() << " branches, "
              << m_decorations.size() << " decorations\n";
}

void TurtleGraphics::processCommand(char cmd, float angle) {
    switch (cmd) {
        // ---------------------------------------------------------------------
        // Movement Commands
        // ---------------------------------------------------------------------
        case 'F':
        case 'G':
        case 'A':
        case 'B': {
            // Move forward and draw a branch segment
            glm::vec3 endPos = m_currentState.position + m_currentState.heading * m_stepSize;

            BranchData branch{};
            branch.start = m_currentState.position;
            branch.end = endPos;
            branch.radiusStart = m_currentState.width;
            branch.radiusEnd = m_currentState.width * m_widthDecay;
            branch.color = m_currentState.color;
            m_branches.push_back(branch);

            m_currentState.position = endPos;
            break;
        }

        case 'f': {
            // Move forward without drawing
            m_currentState.position += m_currentState.heading * m_stepSize;
            break;
        }

        // ---------------------------------------------------------------------
        // Rotation Commands
        // ---------------------------------------------------------------------
        case '+': {
            // Turn left (yaw positive)
            glm::vec3 axis = m_is3D ? m_currentState.up : glm::vec3(0.0F, 0.0F, 1.0F);
            m_currentState.heading = rotateAroundAxis(m_currentState.heading, axis, angle);
            m_currentState.left = rotateAroundAxis(m_currentState.left, axis, angle);
            break;
        }

        case '-': {
            // Turn right (yaw negative)
            glm::vec3 axis = m_is3D ? m_currentState.up : glm::vec3(0.0F, 0.0F, 1.0F);
            m_currentState.heading = rotateAroundAxis(m_currentState.heading, axis, -angle);
            m_currentState.left = rotateAroundAxis(m_currentState.left, axis, -angle);
            break;
        }

        case '&': {
            // Pitch down
            m_currentState.heading =
                rotateAroundAxis(m_currentState.heading, m_currentState.left, angle);
            m_currentState.up = rotateAroundAxis(m_currentState.up, m_currentState.left, angle);
            break;
        }

        case '^': {
            // Pitch up
            m_currentState.heading =
                rotateAroundAxis(m_currentState.heading, m_currentState.left, -angle);
            m_currentState.up = rotateAroundAxis(m_currentState.up, m_currentState.left, -angle);
            break;
        }

        case '\\': {
            // Roll left
            m_currentState.left =
                rotateAroundAxis(m_currentState.left, m_currentState.heading, angle);
            m_currentState.up = rotateAroundAxis(m_currentState.up, m_currentState.heading, angle);
            break;
        }

        case '/': {
            // Roll right
            m_currentState.left =
                rotateAroundAxis(m_currentState.left, m_currentState.heading, -angle);
            m_currentState.up = rotateAroundAxis(m_currentState.up, m_currentState.heading, -angle);
            break;
        }

        case '|': {
            // Turn around (180 degrees)
            glm::vec3 axis = m_is3D ? m_currentState.up : glm::vec3(0.0F, 0.0F, 1.0F);
            m_currentState.heading = rotateAroundAxis(m_currentState.heading, axis, 180.0F);
            m_currentState.left = rotateAroundAxis(m_currentState.left, axis, 180.0F);
            break;
        }

        // ---------------------------------------------------------------------
        // Branching Commands
        // ---------------------------------------------------------------------
        case '[': {
            // Push current state (start branch)
            m_stateStack.push(m_currentState);
            m_currentState.depth++;
            m_currentState.width *= m_widthDecay;
            break;
        }

        case ']': {
            // Pop state (end branch)
            if (!m_stateStack.empty()) {
                m_currentState = m_stateStack.top();
                m_stateStack.pop();
            }
            break;
        }

        // ---------------------------------------------------------------------
        // Decoration Commands
        // ---------------------------------------------------------------------
        case 'L':
        case 'l': {
            // Create a leaf
            DecorationData leaf{};
            leaf.position = m_currentState.position;
            leaf.color = m_leafColor;
            leaf.size = m_leafSize;
            leaf.type = 0;

            // Build orientation matrix from turtle state vectors
            glm::vec3 forward = glm::normalize(m_currentState.heading);
            glm::vec3 right = glm::normalize(glm::cross(forward, m_currentState.up));
            glm::vec3 up = glm::cross(right, forward);
            leaf.orientation = glm::mat4(glm::vec4(right, 0.0F), glm::vec4(forward, 0.0F),
                                         glm::vec4(up, 0.0F), glm::vec4(0.0F, 0.0F, 0.0F, 1.0F));
            m_decorations.push_back(leaf);
            break;
        }

        case 'K':
        case 'k': {
            // Create a flower
            DecorationData flower{};
            flower.position = m_currentState.position;
            flower.color = m_flowerColor;
            flower.size = m_leafSize * 1.5F;
            flower.type = 1;

            glm::vec3 forward = glm::normalize(m_currentState.heading);
            glm::vec3 right = glm::normalize(glm::cross(forward, m_currentState.up));
            glm::vec3 up = glm::cross(right, forward);
            flower.orientation = glm::mat4(glm::vec4(right, 0.0F), glm::vec4(forward, 0.0F),
                                           glm::vec4(up, 0.0F), glm::vec4(0.0F, 0.0F, 0.0F, 1.0F));
            m_decorations.push_back(flower);
            break;
        }

        // ---------------------------------------------------------------------
        // Modifier Commands
        // ---------------------------------------------------------------------
        case '!': {
            // Decrease width
            m_currentState.width *= m_widthDecay;
            break;
        }

        case '\'': {
            // Shift color toward green (for gradient effect on branches)
            m_currentState.color.g = std::min(1.0F, m_currentState.color.g + 0.05F);
            m_currentState.color.r = std::max(0.0F, m_currentState.color.r - 0.02F);
            break;
        }

        // Variables and unknown symbols are ignored
        default:
            break;
    }
}

glm::vec3 TurtleGraphics::rotateAroundAxis(const glm::vec3& vec, const glm::vec3& axis,
                                           float angleDeg) {
    // Rodrigues' rotation formula
    float angleRad = glm::radians(angleDeg);
    float cosA = std::cos(angleRad);
    float sinA = std::sin(angleRad);
    glm::vec3 k = glm::normalize(axis);

    return vec * cosA + glm::cross(k, vec) * sinA + k * glm::dot(k, vec) * (1.0F - cosA);
}

// =============================================================================
// Data Upload
// =============================================================================

void TurtleGraphics::uploadBranchData() {
    if (m_branches.empty())
        return;

    // For line mode: 2 vertices per branch, 6 floats each (pos + color)
    std::vector<float> lineData;
    lineData.reserve(m_branches.size() * 12);

    for (const auto& branch : m_branches) {
        // Start vertex
        lineData.push_back(branch.start.x);
        lineData.push_back(branch.start.y);
        lineData.push_back(branch.start.z);
        lineData.push_back(branch.color.r);
        lineData.push_back(branch.color.g);
        lineData.push_back(branch.color.b);

        // End vertex
        lineData.push_back(branch.end.x);
        lineData.push_back(branch.end.y);
        lineData.push_back(branch.end.z);
        lineData.push_back(branch.color.r);
        lineData.push_back(branch.color.g);
        lineData.push_back(branch.color.b);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(lineData.size() * sizeof(float)),
                 lineData.data(), GL_DYNAMIC_DRAW);

    // For cylinder mode: instance data
    std::vector<float> instanceData;
    instanceData.reserve(m_branches.size() * 11);

    for (const auto& branch : m_branches) {
        instanceData.push_back(branch.start.x);
        instanceData.push_back(branch.start.y);
        instanceData.push_back(branch.start.z);
        instanceData.push_back(branch.end.x);
        instanceData.push_back(branch.end.y);
        instanceData.push_back(branch.end.z);
        instanceData.push_back(branch.radiusStart);
        instanceData.push_back(branch.radiusEnd);
        instanceData.push_back(branch.color.r);
        instanceData.push_back(branch.color.g);
        instanceData.push_back(branch.color.b);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_cylinderInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(instanceData.size() * sizeof(float)),
                 instanceData.data(), GL_DYNAMIC_DRAW);
}

void TurtleGraphics::uploadDecorationData() {
    if (m_decorations.empty())
        return;

    // Separar hojas y flores para renderizar por grupos
    m_leaves.clear();
    m_flowers.clear();

    for (const auto& decor : m_decorations) {
        if (decor.type == 0) {
            m_leaves.push_back(decor);
        } else {
            m_flowers.push_back(decor);
        }
    }

    // Reorganizar: primero hojas, luego flores
    std::vector<float> instanceData;
    instanceData.reserve(m_decorations.size() * 23);

    // Subir hojas primero
    for (const auto& decor : m_leaves) {
        instanceData.push_back(decor.position.x);
        instanceData.push_back(decor.position.y);
        instanceData.push_back(decor.position.z);

        const float* matPtr = glm::value_ptr(decor.orientation);
        for (int i = 0; i < 16; ++i) {
            instanceData.push_back(matPtr[i]);
        }

        instanceData.push_back(decor.color.r);
        instanceData.push_back(decor.color.g);
        instanceData.push_back(decor.color.b);
        instanceData.push_back(decor.size);
    }

    // Luego subir flores
    for (const auto& decor : m_flowers) {
        instanceData.push_back(decor.position.x);
        instanceData.push_back(decor.position.y);
        instanceData.push_back(decor.position.z);

        const float* matPtr = glm::value_ptr(decor.orientation);
        for (int i = 0; i < 16; ++i) {
            instanceData.push_back(matPtr[i]);
        }

        instanceData.push_back(decor.color.r);
        instanceData.push_back(decor.color.g);
        instanceData.push_back(decor.color.b);
        instanceData.push_back(decor.size);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_decorationInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(instanceData.size() * sizeof(float)),
                 instanceData.data(), GL_DYNAMIC_DRAW);
}

// =============================================================================
// Rendering
// =============================================================================

void TurtleGraphics::render(const glm::mat4& view, const glm::mat4& projection,
                            const glm::vec3& lightPos) {
    if (!m_initialized)
        return;

    // Renderizar piso primero (esta detras de todo)
    if (m_is3D) {
        renderFloor(view, projection, lightPos);
    }

    if (m_renderMode == RenderMode::Lines) {
        renderLines(view, projection);
    } else {
        renderCylinders(view, projection, lightPos);
    }

    renderDecorations(view, projection, lightPos);
}

void TurtleGraphics::renderLines(const glm::mat4& view, const glm::mat4& projection) {
    if (m_branches.empty())
        return;

    glUseProgram(m_lineShader);
    glUniformMatrix4fv(glGetUniformLocation(m_lineShader, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_lineShader, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));

    glLineWidth(2.0F);  // May not work on all drivers
    glBindVertexArray(m_lineVAO);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_branches.size() * 2));
    glBindVertexArray(0);
}

void TurtleGraphics::renderCylinders(const glm::mat4& view, const glm::mat4& projection,
                                     const glm::vec3& lightPos) {
    if (m_branches.empty())
        return;

    glm::vec3 viewPos = glm::vec3(glm::inverse(view)[3]);

    glUseProgram(m_cylinderShader);
    glUniformMatrix4fv(glGetUniformLocation(m_cylinderShader, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_cylinderShader, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(m_cylinderShader, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(m_cylinderShader, "viewPos"), 1, glm::value_ptr(viewPos));

    glBindVertexArray(m_cylinderVAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, CYLINDER_VERTEX_COUNT,
                          static_cast<GLsizei>(m_branches.size()));
    glBindVertexArray(0);
}

void TurtleGraphics::renderDecorations(const glm::mat4& view, const glm::mat4& projection,
                                       const glm::vec3& lightPos) {
    if (m_decorations.empty())
        return;

    glm::vec3 viewPos = glm::vec3(glm::inverse(view)[3]);

    // Habilitar blending para transparencia
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_decorationShader);
    glUniformMatrix4fv(glGetUniformLocation(m_decorationShader, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_decorationShader, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(m_decorationShader, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(m_decorationShader, "viewPos"), 1, glm::value_ptr(viewPos));

    glBindVertexArray(m_decorationVAO);

    // Renderizar hojas (tipo 0)
    if (!m_leaves.empty()) {
        glUniform1i(glGetUniformLocation(m_decorationShader, "decorationType"), 0);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(m_leaves.size()));
    }

    // Renderizar flores (tipo 1) - necesitamos reconfigurar el offset de instancias
    if (!m_flowers.empty()) {
        glUniform1i(glGetUniformLocation(m_decorationShader, "decorationType"), 1);

        // Ajustar el offset del buffer de instancias para las flores
        size_t leavesOffset = m_leaves.size() * 23 * sizeof(float);
        glBindBuffer(GL_ARRAY_BUFFER, m_decorationInstanceVBO);

        // Reconfiguramos los atributos de instancia con el offset correcto
        // iPosition (location 2)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 23 * sizeof(float),
                              reinterpret_cast<void*>(leavesOffset));
        // iOrientation (locations 3-6)
        for (int i = 0; i < 4; ++i) {
            glVertexAttribPointer(
                3 + i, 4, GL_FLOAT, GL_FALSE, 23 * sizeof(float),
                reinterpret_cast<void*>(leavesOffset + (3 + i * 4) * sizeof(float)));
        }
        // iColor (location 7)
        glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 23 * sizeof(float),
                              reinterpret_cast<void*>(leavesOffset + 19 * sizeof(float)));
        // iSize (location 8)
        glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, 23 * sizeof(float),
                              reinterpret_cast<void*>(leavesOffset + 22 * sizeof(float)));

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, static_cast<GLsizei>(m_flowers.size()));

        // Restaurar offset a 0 para la proxima vez
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 23 * sizeof(float),
                              reinterpret_cast<void*>(0));
        for (int i = 0; i < 4; ++i) {
            glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, 23 * sizeof(float),
                                  reinterpret_cast<void*>((3 + i * 4) * sizeof(float)));
        }
        glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 23 * sizeof(float),
                              reinterpret_cast<void*>(19 * sizeof(float)));
        glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, 23 * sizeof(float),
                              reinterpret_cast<void*>(22 * sizeof(float)));
    }

    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void TurtleGraphics::clear() {
    m_branches.clear();
    m_decorations.clear();
}

// =============================================================================
// Floor Rendering
// =============================================================================

void TurtleGraphics::setupFloor() {
    // Crear un quad grande para el piso
    // Vertices: posicion (3) + texcoord (2)
    // clang-format off
    float floorVertices[] = {
        // posicion           // texcoord
        -2.0f, 0.0f, -2.0f,   0.0f, 0.0f,
         2.0f, 0.0f, -2.0f,   1.0f, 0.0f,
         2.0f, 0.0f,  2.0f,   1.0f, 1.0f,

        -2.0f, 0.0f, -2.0f,   0.0f, 0.0f,
         2.0f, 0.0f,  2.0f,   1.0f, 1.0f,
        -2.0f, 0.0f,  2.0f,   0.0f, 1.0f,
    };
    // clang-format on

    glGenVertexArrays(1, &m_floorVAO);
    glGenBuffers(1, &m_floorVBO);

    glBindVertexArray(m_floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    // Posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // TexCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void TurtleGraphics::renderFloor(const glm::mat4& view, const glm::mat4& projection,
                                 const glm::vec3& lightPos) {
    if (!m_showFloor || m_floorShader == 0) {
        return;
    }

    // Habilitar blending para el fade de los bordes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_floorShader);
    glUniformMatrix4fv(glGetUniformLocation(m_floorShader, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_floorShader, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(m_floorShader, "lightPos"), 1, glm::value_ptr(lightPos));

    // Calcular centro y radio de la planta para sombras
    glm::vec3 plantCenter(0.0f, 0.0f, 0.0f);
    float plantRadius = 0.5f;  // Radio estimado de la planta

    glUniform3fv(glGetUniformLocation(m_floorShader, "plantCenter"), 1,
                 glm::value_ptr(plantCenter));
    glUniform1f(glGetUniformLocation(m_floorShader, "plantRadius"), plantRadius);

    glBindVertexArray(m_floorVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}
