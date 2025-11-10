/*
 * =================================================================
 * ARCHIVO: Camera.h
 * CONTENIDO: Declaracion de la clase Camera.
 * DESCRIPCION:
 * Este archivo define la clase Camera que maneja las matrices de
 * vista y proyeccion para renderizado 2D y 3D. Proporciona
 * proyeccion ortografica con ajuste de aspecto automatico.
 *
 * AUTOR: Julian Parra
 * CURSO: Graficacion (2025)
 * PROYECTO: Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 * =================================================================
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

/**
 * @class Camera
 * @brief Maneja las transformaciones de camara para renderizado
 *
 * Esta clase calcula y mantiene las matrices de proyeccion y vista,
 * adaptandose automaticamente al aspecto de la ventana.
 */
class Camera {
public:
    /**
     * @brief Constructor por defecto
     */
    Camera();

    /**
     * @brief Actualiza la matriz de proyeccion basada en el tamaño de ventana
     * @param width Ancho del framebuffer
     * @param height Alto del framebuffer
     */
    void updateProjection(int width, int height);

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
     * @brief Obtiene el ratio de aspecto actual
     * @return Ratio de aspecto (width/height)
     */
    float getAspectRatio() const {
        return m_aspectRatio;
    }

private:
    glm::mat4 m_projectionMatrix{1.0F};  ///< Matriz de proyeccion
    glm::mat4 m_viewMatrix{1.0F};        ///< Matriz de vista
    float m_aspectRatio{1.0F};           ///< Ratio de aspecto actual
};

#endif  // CAMERA_H
