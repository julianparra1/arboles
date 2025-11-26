/**
 * @file Camera.h
 * @brief Clase de camara para manejo de matrices de vista y proyeccion.
 *
 * Maneja proyeccion ortografica y perspectiva con ajuste automatico
 * de aspect ratio. Soporta controles de camara orbital para navegacion
 * de escenas 3D.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

/**
 * @class Camera
 * @brief Maneja transformaciones de camara para renderizado 2D y 3D.
 *
 * Calcula y mantiene matrices de proyeccion y vista,
 * adaptandose automaticamente a cambios de aspect ratio.
 */
class Camera {
public:
    /**
     * @brief Constructor por defecto
     */
    Camera();

    /**
     * @brief Actualiza la matriz de proyeccion ortografica
     * @param width Ancho del framebuffer
     * @param height Alto del framebuffer
     */
    void updateProjection(int width, int height);

    /**
     * @brief Actualiza la matriz de proyeccion perspectiva
     * @param width Ancho del framebuffer
     * @param height Alto del framebuffer
     * @param fov Campo de vision en grados
     * @param near Plano cercano
     * @param far Plano lejano
     */
    void updatePerspective(int width, int height, float fov = 45.0F, float near = 0.1F,
                           float far = 100.0F);

    /**
     * @brief Actualiza la matriz de vista usando posicion orbital
     * @param distance Distancia desde el origen
     * @param angleX Angulo horizontal (yaw) en grados
     * @param angleY Angulo vertical (pitch) en grados
     * @param target Punto al que mira la camara
     */
    void updateView(float distance, float angleX, float angleY,
                    const glm::vec3& target = glm::vec3(0.0F, 0.3F, 0.0F));

    /**
     * @brief Obtiene la matriz de proyeccion
     * @return Matriz de proyeccion 4x4
     */
    const glm::mat4& getProjectionMatrix() const {
        return m_projectionMatrix;
    }

    /**
     * @brief Obtiene la matriz de vista
     * @return Matriz de vista 4x4
     */
    const glm::mat4& getViewMatrix() const {
        return m_viewMatrix;
    }

    /**
     * @brief Obtiene la posicion de la camara
     * @return Posicion 3D de la camara
     */
    const glm::vec3& getPosition() const {
        return m_position;
    }

    /**
     * @brief Obtiene el ratio de aspecto actual
     * @return Ratio de aspecto (width/height)
     */
    float getAspectRatio() const {
        return m_aspectRatio;
    }

private:
    glm::mat4 m_projectionMatrix{1.0F};      ///< Matriz de proyeccion
    glm::mat4 m_viewMatrix{1.0F};            ///< Matriz de vista
    glm::vec3 m_position{0.0F, 0.0F, 3.0F};  ///< Posicion de la camara
    float m_aspectRatio{1.0F};               ///< Ratio de aspecto actual
};

#endif  // CAMERA_H
