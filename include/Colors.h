/*
 * =================================================================
 * ARCHIVO: Colors.h
 * CONTENIDO: Utilidades para manejo de colores en OpenGL.
 * DESCRIPCION:
 * Este archivo proporciona macros y funciones para convertir
 * colores hexadecimales (formato web #RRGGBB) a valores flotantes
 * normalizados [0.0, 1.0] que OpenGL requiere.
 *
 * Uso:
 *   constexpr auto BACKGROUND = HEX_COLOR(0x2E3440);
 *   glClearColor(BACKGROUND.r, BACKGROUND.g, BACKGROUND.b, 1.0F);
 *
 * AUTOR: Julian Parra
 * CURSO: Graficacion (2025)
 * PROYECTO: Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 * =================================================================
 */

#ifndef COLORS_H
#define COLORS_H

#include <cstdint>

/*
 * @struct Color
 * @brief Representa un color RGBA con componentes flotantes normalizados.
 */
struct Color {
    float r, g, b, a;

    constexpr Color(float red, float green, float blue, float alpha = 1.0F)
        : r(red), g(green), b(blue), a(alpha) {}
};

/*
 * @brief Convierte un valor de color hexadecimal a flotante normalizado.
 * @param component Componente del color (0-255)
 * @return Valor normalizado (0.0-1.0)
 */
constexpr float hexToFloat(uint8_t component) {
    return static_cast<float>(component) / 255.0F;
}

/*
 * @brief Crea un Color desde un valor hexadecimal (formato 0xRRGGBB).
 * @param hex Valor hexadecimal del color (ej: 0x2E3440)
 * @param alpha Valor alpha opcional (default 1.0)
 * @return Estructura Color con componentes normalizados
 *
 * Ejemplos:
 *   HEX_COLOR(0xFF0000)     -> Rojo puro
 *   HEX_COLOR(0x00FF00)     -> Verde puro
 *   HEX_COLOR(0x0000FF)     -> Azul puro
 *   HEX_COLOR(0x2E3440)     -> Gris oscuro (Nord theme)
 */
constexpr Color HEX_COLOR(uint32_t hex, float alpha = 1.0F) {
    return Color(hexToFloat((hex >> 16) & 0xFF),  // Red
                 hexToFloat((hex >> 8) & 0xFF),   // Green
                 hexToFloat(hex & 0xFF),          // Blue
                 alpha);
}

/*
 * @brief Crea un Color desde un valor hexadecimal con alpha (formato
 * 0xRRGGBBAA).
 * @param hex Valor hexadecimal del color con alpha (ej: 0xFF0000FF)
 * @return Estructura Color con componentes normalizados
 */
constexpr Color HEX_COLOR_ALPHA(uint32_t hex) {
    return Color(hexToFloat((hex >> 24) & 0xFF),  // Red
                 hexToFloat((hex >> 16) & 0xFF),  // Green
                 hexToFloat((hex >> 8) & 0xFF),   // Blue
                 hexToFloat(hex & 0xFF));         // Alpha
}

// ============================================================================
// PALETA DE COLORES PREDEFINIDOS
// ============================================================================

namespace Colors {
// Colores basicos
constexpr auto BLACK = HEX_COLOR(0x000000);
constexpr auto WHITE = HEX_COLOR(0xFFFFFF);
constexpr auto RED = HEX_COLOR(0xFF0000);
constexpr auto GREEN = HEX_COLOR(0x00FF00);
constexpr auto BLUE = HEX_COLOR(0x0000FF);
constexpr auto YELLOW = HEX_COLOR(0xFFFF00);
constexpr auto CYAN = HEX_COLOR(0x00FFFF);
constexpr auto MAGENTA = HEX_COLOR(0xFF00FF);

// Nord Theme (popular en programacion)
// https://www.nordtheme.com/
namespace Nord {
constexpr auto POLAR_NIGHT_0 = HEX_COLOR(0x2E3440);  // Mas oscuro
constexpr auto POLAR_NIGHT_1 = HEX_COLOR(0x3B4252);
constexpr auto POLAR_NIGHT_2 = HEX_COLOR(0x434C5E);
constexpr auto POLAR_NIGHT_3 = HEX_COLOR(0x4C566A);

constexpr auto SNOW_STORM_0 = HEX_COLOR(0xD8DEE9);
constexpr auto SNOW_STORM_1 = HEX_COLOR(0xE5E9F0);
constexpr auto SNOW_STORM_2 = HEX_COLOR(0xECEFF4);  // Mas claro

constexpr auto FROST_0 = HEX_COLOR(0x8FBCBB);  // Cyan suave
constexpr auto FROST_1 = HEX_COLOR(0x88C0D0);  // Azul claro
constexpr auto FROST_2 = HEX_COLOR(0x81A1C1);  // Azul medio
constexpr auto FROST_3 = HEX_COLOR(0x5E81AC);  // Azul oscuro

constexpr auto AURORA_RED = HEX_COLOR(0xBF616A);
constexpr auto AURORA_ORANGE = HEX_COLOR(0xD08770);
constexpr auto AURORA_YELLOW = HEX_COLOR(0xEBCB8B);
constexpr auto AURORA_GREEN = HEX_COLOR(0xA3BE8C);
constexpr auto AURORA_PURPLE = HEX_COLOR(0xB48EAD);
}  // namespace Nord

// Dracula Theme
// https://draculatheme.com/
namespace Dracula {
constexpr auto BACKGROUND = HEX_COLOR(0x282A36);
constexpr auto CURRENT_LINE = HEX_COLOR(0x44475A);
constexpr auto FOREGROUND = HEX_COLOR(0xF8F8F2);
constexpr auto COMMENT = HEX_COLOR(0x6272A4);
constexpr auto CYAN = HEX_COLOR(0x8BE9FD);
constexpr auto GREEN = HEX_COLOR(0x50FA7B);
constexpr auto ORANGE = HEX_COLOR(0xFFB86C);
constexpr auto PINK = HEX_COLOR(0xFF79C6);
constexpr auto PURPLE = HEX_COLOR(0xBD93F9);
constexpr auto RED = HEX_COLOR(0xFF5555);
constexpr auto YELLOW = HEX_COLOR(0xF1FA8C);
}  // namespace Dracula

// Colores naturales para arboles/plantas
namespace Nature {
constexpr auto BARK_BROWN = HEX_COLOR(0x4A3728);
constexpr auto DARK_WOOD = HEX_COLOR(0x5C4033);
constexpr auto LEAF_GREEN = HEX_COLOR(0x2D5016);
constexpr auto BRIGHT_GREEN = HEX_COLOR(0x76B947);
constexpr auto GRASS_GREEN = HEX_COLOR(0x4F7942);
constexpr auto SKY_BLUE = HEX_COLOR(0x87CEEB);
constexpr auto EARTH_BROWN = HEX_COLOR(0x8B4513);
constexpr auto FOREST_GREEN = HEX_COLOR(0x228B22);
}  // namespace Nature

}  // namespace Colors

#endif  // COLORS_H
