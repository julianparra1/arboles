/*
 * =================================================================
 * ARCHIVO: Camera.cpp
 * CONTENIDO: Implementacion de la clase Camera.
 * DESCRIPCION:
 * Este archivo implementa las funciones de la camara, incluyendo
 * el calculo de proyeccion ortografica que mantiene el aspecto.
 *
 * AUTOR: Julian Parra
 * CURSO: Graficacion (2025)
 * PROYECTO: Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 * =================================================================
 */

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

/*
 * @brief Constructor por defecto
 */
Camera::Camera() {
    // Inicializar con proyeccion ortografica por defecto
    m_projectionMatrix = glm::ortho(-1.0F, 1.0F, -1.0F, 1.0F, -1.0F, 1.0F);
    m_viewMatrix = glm::mat4(1.0F);
}

/*
 * @brief Actualiza la proyeccion basada en el aspecto de ventana
 */
void Camera::updateProjection(int width, int height) {
    if (height == 0) {
        height = 1;  // Evitar division por cero
    }

    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    // Crear proyeccion ortografica que mantiene el aspecto
    if (m_aspectRatio >= 1.0F) {
        // Mas ancho que alto - expandir ancho
        m_projectionMatrix = glm::ortho(-m_aspectRatio, m_aspectRatio, -1.0F, 1.0F, -1.0F, 1.0F);
    } else {
        // Mas alto que ancho - expandir alto
        float invAspect = 1.0F / m_aspectRatio;
        m_projectionMatrix = glm::ortho(-1.0F, 1.0F, -invAspect, invAspect, -1.0F, 1.0F);
    }
}
