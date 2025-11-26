/**
 * @file Shader.cpp
 * @brief Implementacion de la clase Shader.
 *
 * Contiene metodos para compilar, enlazar y usar shaders de OpenGL.
 * Incluye manejo de errores y funciones para establecer uniformes.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/*
 * @brief Constructor que compila y enlaza shaders
 */
Shader::Shader(const char* vertexSource, const char* fragmentSource) {
    // Compilar shaders
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0) {
        std::cerr << "Error: Failed to compile shaders\n";
        return;
    }

    // Enlazar programa
    m_program = linkProgram(vertexShader, fragmentShader);

    // Limpiar shaders individuales (ya no se necesitan)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (m_program == 0) {
        std::cerr << "Error: Failed to link shader program\n";
    }
}

/*
 * @brief Destructor que limpia recursos
 */
Shader::~Shader() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
    }
}

/*
 * @brief Activa el programa de shader
 */
void Shader::use() const {
    glUseProgram(m_program);
}

/*
 * @brief Establece un uniform mat4
 */
void Shader::setMat4(const char* name, const glm::mat4& value) const {
    GLint location = glGetUniformLocation(m_program, name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

/*
 * @brief Establece un uniform vec3
 */
void Shader::setVec3(const char* name, const glm::vec3& value) const {
    GLint location = glGetUniformLocation(m_program, name);
    if (location != -1) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }
}

/*
 * @brief Establece un uniform vec4
 */
void Shader::setVec4(const char* name, const glm::vec4& value) const {
    GLint location = glGetUniformLocation(m_program, name);
    if (location != -1) {
        glUniform4fv(location, 1, glm::value_ptr(value));
    }
}

/*
 * @brief Establece un uniform float
 */
void Shader::setFloat(const char* name, float value) const {
    GLint location = glGetUniformLocation(m_program, name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

/*
 * @brief Establece un uniform int
 */
void Shader::setInt(const char* name, int value) const {
    GLint location = glGetUniformLocation(m_program, name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

/*
 * @brief Compila un shader desde codigo fuente
 */
GLuint Shader::compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Verificar errores de compilacion
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        constexpr GLsizei LOG_SIZE = 512;
        GLchar infoLog[LOG_SIZE];
        glGetShaderInfoLog(shader, LOG_SIZE, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << '\n';
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

/*
 * @brief Enlaza shaders en un programa
 */
GLuint Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Verificar errores de enlazado
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 0) {
        constexpr GLsizei LOG_SIZE = 512;
        GLchar infoLog[LOG_SIZE];
        glGetProgramInfoLog(program, LOG_SIZE, nullptr, infoLog);
        std::cerr << "Error: Shader program linking failed\n" << infoLog << '\n';
        glDeleteProgram(program);
        return 0;
    }

    return program;
}
