/*
 * =================================================================
 * ARCHIVO: TurtleRenderer.cpp
 * CONTENIDO: Implementacion del renderizador de graficos de tortuga.
 * DESCRIPCION:
 * Este archivo implementa la interpretacion de cadenas de L-System
 * como comandos de tortuga y su renderizado en OpenGL. Utiliza
 * la pila de matrices GL_MODELVIEW como el estado de la tortuga,
 * eliminando la necesidad de rastrear posicion/orientacion
 * manualmente. Los comandos '[' y ']' mapean directamente a
 * glPushMatrix y glPopMatrix para implementar estructuras ramificadas.
 *
 * AUTOR: Julian Parra
 * CURSO: Graficacion (2025)
 * PROYECTO: Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas" *
 * =================================================================
 */

#include "TurtleRenderer.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constantes para rotaciones
constexpr float TURNAROUND_ANGLE = 180.0F;

/*
 * @brief Constructor del TurtleRenderer.
 * @param step Distancia de avance.
 * @param angle Angulo de rotacion en grados.
 * @param enable3D Modo de renderizado (2D o 3D).
 */
TurtleRenderer::TurtleRenderer(float step, float angle, bool enable3D)
    : stepSize(step), defaultAngle(angle), is3D(enable3D) {}

/*
 * @brief Destructor.
 */
TurtleRenderer::~TurtleRenderer() {
    // No requiere limpieza especial
}

/*
 * @brief Renderiza la cadena del L-System.
 * @param lsystemString Cadena generada por el L-System.
 * @param angle Angulo de rotacion a utilizar.
 * @note La matriz GL_MODELVIEW debe estar configurada antes de llamar
 *       a esta funcion. La tortuga siempre comienza en el origen (0,0,0)
 *       de su sistema de coordenadas local.
 */
void TurtleRenderer::draw(const std::string& lsystemString, float angle) {
    // (c): Iterar sobre cada simbolo de la cadena del L-System
    for (char command : lsystemString) {
        processCommand(command, angle);
    }
}

/*
 * @brief Procesa un comando individual de la tortuga.
 * @param command Simbolo del L-System (F, +, -, [, ], etc.)
 * @param angle Angulo de rotacion en grados.
 * @note Este metodo implementa el mapeo directo de simbolos
 *       de L-System a llamadas de OpenGL. La clave es que
 *       la matriz GL_MODELVIEW ES el estado de la tortuga.
 */
void TurtleRenderer::processCommand(char command, float angle) const {
    switch (command) {
        case 'F':
        case 'G':  // G tambien dibuja (usado en algunos L-Systems)
            // (c): Dibujar un segmento de linea desde el origen
            // hasta (stepSize, 0, 0) en el sistema de coordenadas actual.
            // Usamos GL_LINES (no GL_LINE_STRIP) para evitar conectar
            // ramas separadas despues de un glPopMatrix.
            glBegin(GL_LINES);
            if (is3D) {
                glVertex3f(0.0F, 0.0F, 0.0F);      // Punto inicial
                glVertex3f(0.0F, stepSize, 0.0F);  // Punto final (hacia arriba en Y)
            } else {
                glVertex2f(0.0F, 0.0F);      // Punto inicial (2D)
                glVertex2f(stepSize, 0.0F);  // Punto final (hacia derecha en X)
            }
            glEnd();

            // (c): Trasladar la matriz (y por tanto, la tortuga) al
            // final del segmento que acabamos de dibujar.
            if (is3D) {
                glTranslatef(0.0F, stepSize, 0.0F);
            } else {
                glTranslatef(stepSize, 0.0F, 0.0F);
            }
            break;

        case 'f':
            // (c): Mover hacia adelante SIN dibujar
            if (is3D) {
                glTranslatef(0.0F, stepSize, 0.0F);
            } else {
                glTranslatef(stepSize, 0.0F, 0.0F);
            }
            break;

        case '+':
            // (c): Girar a la izquierda (yaw - rotacion alrededor del eje Z en 2D,
            // o eje Y en 3D dependiendo de la convencion)
            if (is3D) {
                glRotatef(angle, 0.0F, 0.0F, 1.0F);  // Rotar alrededor de Z
            } else {
                glRotatef(angle, 0.0F, 0.0F, 1.0F);  // Rotar alrededor de Z (2D)
            }
            break;

        case '-':
            // (c): Girar a la derecha (yaw negativo)
            if (is3D) {
                glRotatef(-angle, 0.0F, 0.0F, 1.0F);
            } else {
                glRotatef(-angle, 0.0F, 0.0F, 1.0F);
            }
            break;

        case '&':
            // (c): Pitch down (rotar hacia abajo alrededor del eje X)
            // Solo relevante en 3D
            if (is3D) {
                glRotatef(angle, 1.0F, 0.0F, 0.0F);
            }
            break;

        case '^':
            // (c): Pitch up (rotar hacia arriba alrededor del eje X)
            // Solo relevante en 3D
            if (is3D) {
                glRotatef(-angle, 1.0F, 0.0F, 0.0F);
            }
            break;

        case '\\':
            // (c): Roll left (rotar alrededor del eje Y - heading)
            // Solo relevante en 3D
            if (is3D) {
                glRotatef(angle, 0.0F, 1.0F, 0.0F);
            }
            break;

        case '/':
            // (c): Roll right (rotar alrededor del eje Y)
            // Solo relevante en 3D
            if (is3D) {
                glRotatef(-angle, 0.0F, 1.0F, 0.0F);
            }
            break;

        case '[':
            // (c): CRITICO - Inicio de una rama.
            // Guardar el estado actual de la tortuga (la matriz GL_MODELVIEW)
            // en la pila de matrices. Esto nos permite "recordar" donde
            // estamos para poder regresar despues del comando ']'.
            glPushMatrix();
            break;

        case ']':
            // (c): CRITICO - Fin de una rama.
            // Restaurar el ultimo estado guardado desde la pila.
            // Esto "teletransporta" la tortuga de regreso al punto
            // donde se ejecuto el ultimo '[', permitiendo dibujar
            // multiples ramas desde el mismo punto de bifurcacion.
            glPopMatrix();
            break;

        case '|':
            // (c): Dar la vuelta (180 grados)
            glRotatef(TURNAROUND_ANGLE, 0.0F, 0.0F, 1.0F);
            break;

        // Otros simbolos (como 'X', 'Y', 'A', 'B', etc.) son ignorados
        // porque son solo variables del L-System sin interpretacion grafica
        default:
            // No hacer nada para simbolos no reconocidos
            break;
    }
}

/*
 * @brief Establece el tamano de paso.
 * @param step Nueva distancia de avance.
 */
void TurtleRenderer::setStepSize(float step) {
    stepSize = step;
}

/*
 * @brief Establece el angulo de rotacion.
 * @param angle Angulo en grados.
 */
void TurtleRenderer::setAngle(float angle) {
    defaultAngle = angle;
}

/*
 * @brief Configura el modo de renderizado.
 * @param enable3D true para 3D, false para 2D.
 */
void TurtleRenderer::set3DMode(bool enable3D) {
    is3D = enable3D;
}

/*
 * @brief Obtiene el tamano de paso.
 * @return Distancia de avance.
 */
float TurtleRenderer::getStepSize() const {
    return stepSize;
}

/*
 * @brief Obtiene el angulo.
 * @return Angulo en grados.
 */
float TurtleRenderer::getAngle() const {
    return defaultAngle;
}

/*
 * @brief Verifica el modo de renderizado.
 * @return true si esta en 3D, false si esta en 2D.
 */
bool TurtleRenderer::is3DMode() const {
    return is3D;
}
