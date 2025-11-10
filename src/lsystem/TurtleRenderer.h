/*
 * =================================================================
 * ARCHIVO: TurtleRenderer.h
 * CONTENIDO: Declaracion del renderizador de graficos de tortuga.
 * DESCRIPCION:
 * Este archivo define la clase TurtleRenderer, responsable de
 * interpretar la cadena generada por un L-System y renderizarla
 * usando graficos de tortuga (turtle graphics) en OpenGL.
 * La tortuga es un cursor conceptual con posicion y orientacion,
 * y la pila de matrices de OpenGL (glPushMatrix/glPopMatrix)
 * se utiliza para manejar el estado de la tortuga y las estructuras
 * ramificadas.
 *
 * AUTOR: Julian Parra
 * CURSO: Graficacion (2025)
 * PROYECTO: Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas" *
 * =================================================================
 */

#ifndef TURTLERENDERER_H
#define TURTLERENDERER_H

#include <string>

/*
 * @class TurtleRenderer
 * @brief Interpreta cadenas de L-System como comandos de tortuga y
 *        los renderiza usando OpenGL.
 *
 * La tortuga tiene un estado definido por su posicion y orientacion.
 * En lugar de rastrear este estado manualmente en variables C++,
 * utilizamos la matriz GL_MODELVIEW como el estado de la tortuga.
 *
 * Mapeo de simbolos a comandos OpenGL:
 * - F: Dibujar linea hacia adelante y avanzar
 * - f: Avanzar sin dibujar
 * - +: Girar a la izquierda (yaw)
 * - -: Girar a la derecha (yaw)
 * - &: Pitch down (3D)
 * - ^: Pitch up (3D)
 * - \: Roll left (3D)
 * - /: Roll right (3D)
 * - [: Guardar estado (glPushMatrix)
 * - ]: Restaurar estado (glPopMatrix)
 */
class TurtleRenderer {
private:
    float stepSize;      // Distancia de avance por comando 'F'
    float defaultAngle;  // Angulo de rotacion por defecto (grados)
    bool is3D;           // Modo de renderizado: 2D o 3D

    // Valores por defecto
    static constexpr float DEFAULT_STEP = 1.0F;
    static constexpr float DEFAULT_ANGLE = 25.0F;

public:
    /*
     * @brief Constructor.
     * @param step Distancia de avance para el comando 'F'.
     * @param angle Angulo de rotacion en grados.
     * @param enable3D Si es true, renderiza en 3D; si es false, en 2D.
     */
    TurtleRenderer(float step = DEFAULT_STEP, float angle = DEFAULT_ANGLE, bool enable3D = false);

    /*
     * @brief Destructor.
     */
    ~TurtleRenderer();

    /*
     * @brief Renderiza una cadena de L-System usando graficos de tortuga.
     * @param lsystemString La cadena generada por el L-System.
     * @param angle Angulo de rotacion (sobrescribe el angulo por defecto).
     * @note Este metodo debe llamarse dentro del callback display() de OpenGL,
     *       despues de configurar la matriz GL_MODELVIEW.
     * @note Utiliza la pila de matrices de OpenGL para manejar el branching:
     *       - '[' llama a glPushMatrix() (guardar estado)
     *       - ']' llama a glPopMatrix() (restaurar estado)
     */
    void draw(const std::string& lsystemString, float angle);

    /*
     * @brief Establece el tamano de paso.
     * @param step Nueva distancia de avance.
     */
    void setStepSize(float step);

    /*
     * @brief Establece el angulo de rotacion por defecto.
     * @param angle Angulo en grados.
     */
    void setAngle(float angle);

    /*
     * @brief Habilita o deshabilita el modo 3D.
     * @param enable3D true para 3D, false para 2D.
     */
    void set3DMode(bool enable3D);

    /*
     * @brief Obtiene el tamano de paso actual.
     * @return Distancia de avance.
     */
    float getStepSize() const;

    /*
     * @brief Obtiene el angulo actual.
     * @return Angulo en grados.
     */
    float getAngle() const;

    /*
     * @brief Verifica si el modo 3D esta habilitado.
     * @return true si esta en modo 3D, false si esta en 2D.
     */
    bool is3DMode() const;

private:
    /*
     * @brief Procesa un comando individual de la tortuga (uso interno).
     * @param command El simbolo a interpretar (F, +, -, [, ], etc.)
     * @param angle Angulo de rotacion a utilizar.
     */
    void processCommand(char command, float angle) const;
};

#endif  // TURTLERENDERER_H
