/**
 * @file Shader.h
 * @brief Manejo de programas de shaders de OpenGL.
 *
 * Maneja compilacion, enlazado y uso de shaders GLSL. Proporciona
 * una interfaz simple para crear programas de shaders y establecer
 * variables uniform.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>

/**
 * @class Shader
 * @brief Maneja compilacion y uso de programas de shaders de OpenGL.
 *
 * Encapsula toda la funcionalidad relacionada con shaders:
 * - Compilacion de vertex y fragment shaders
 * - Enlazado de programas
 * - Establecimiento de uniformes
 * - Manejo de errores
 */
class Shader {
public:
    /**
     * @brief Constructor que compila y enlaza shaders desde codigo fuente
     * @param vertexSource Codigo fuente del vertex shader
     * @param fragmentSource Codigo fuente del fragment shader
     */
    Shader(const char* vertexSource, const char* fragmentSource);

    /**
     * @brief Destructor que limpia recursos de OpenGL
     */
    ~Shader();

    // Deshabilitar copia
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /**
     * @brief Activa el programa de shader para su uso
     */
    void use() const;

    /**
     * @brief Obtiene el ID del programa de shader
     * @return ID del programa
     */
    GLuint getProgram() const {
        return m_program;
    }

    /**
     * @brief Establece un uniform mat4
     * @param name Nombre del uniform
     * @param value Valor de la matriz
     */
    void setMat4(const char* name, const glm::mat4& value) const;

    /**
     * @brief Establece un uniform vec3
     * @param name Nombre del uniform
     * @param value Valor del vector
     */
    void setVec3(const char* name, const glm::vec3& value) const;

    /**
     * @brief Establece un uniform vec4
     * @param name Nombre del uniform
     * @param value Valor del vector
     */
    void setVec4(const char* name, const glm::vec4& value) const;

    /**
     * @brief Establece un uniform float
     * @param name Nombre del uniform
     * @param value Valor del float
     */
    void setFloat(const char* name, float value) const;

    /**
     * @brief Establece un uniform int
     * @param name Nombre del uniform
     * @param value Valor del int
     */
    void setInt(const char* name, int value) const;

    /**
     * @brief Verifica si el shader esta correctamente inicializado
     * @return true si esta inicializado, false en caso contrario
     */
    bool isValid() const {
        return m_program != 0;
    }

private:
    /**
     * @brief Compila un shader desde codigo fuente
     * @param source Codigo fuente del shader
     * @param type Tipo de shader (GL_VERTEX_SHADER o GL_FRAGMENT_SHADER)
     * @return ID del shader compilado, 0 si hay error
     */
    static GLuint compileShader(const char* source, GLenum type);

    /**
     * @brief Enlaza vertex y fragment shaders en un programa
     * @param vertexShader ID del vertex shader
     * @param fragmentShader ID del fragment shader
     * @return ID del programa, 0 si hay error
     */
    static GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);

    GLuint m_program{0};  ///< ID del programa de shader
};

#endif  // SHADER_H
