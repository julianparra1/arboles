/**
 * @file Camera.cpp
 * @brief Implementacion de la clase Camera.
 *
 * Implementa funciones de camara incluyendo calculos de proyeccion
 * ortografica y perspectiva, y controles de camara orbital.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#include "Camera.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

/*
 * @brief Constructor por defecto
 */
Camera::Camera() {
    // Inicializar con proyeccion perspectiva por defecto
    m_projectionMatrix = glm::perspective(glm::radians(45.0F), 1.0F, 0.1F, 100.0F);
    m_viewMatrix =
        glm::lookAt(m_position, glm::vec3(0.0F, 0.3F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F));
}

/*
 * @brief Actualiza la proyeccion ortografica basada en el aspecto de ventana
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

/*
 * @brief Actualiza la proyeccion perspectiva
 */
void Camera::updatePerspective(int width, int height, float fov, float near, float far) {
    if (height == 0) {
        height = 1;
    }

    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    m_projectionMatrix = glm::perspective(glm::radians(fov), m_aspectRatio, near, far);
}

/*
 * @brief Actualiza la matriz de vista usando posicion orbital
 */
void Camera::updateView(float distance, float angleX, float angleY, const glm::vec3& target) {
    // Convertir angulos a radianes
    float radX = glm::radians(angleX);
    float radY = glm::radians(angleY);

    // Calcular posicion de la camara en coordenadas esfericas
    m_position.x = distance * std::cos(radY) * std::sin(radX);
    m_position.y = distance * std::sin(radY) + target.y;
    m_position.z = distance * std::cos(radY) * std::cos(radX);

    // Crear matriz de vista
    m_viewMatrix = glm::lookAt(m_position, target, glm::vec3(0.0F, 1.0F, 0.0F));
}
