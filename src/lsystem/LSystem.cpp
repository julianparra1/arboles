/**
 * @file LSystem.cpp
 * @brief Implementacion de la clase LSystem.
 *
 * Contiene la logica para la generacion de cadenas de L-System, incluyendo
 * metodos para cargar reglas desde configuracion y aplicar el algoritmo de
 * reescritura paralela durante 'n' generaciones. Esta clase no tiene
 * dependencias de OpenGL.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#include "LSystem.h"

#include <fstream>
#include <iostream>

/*
 * @brief Constructor por defecto.
 * Inicializa un L-System con valores vacios.
 */
LSystem::LSystem() : angle(0.0F), currentGeneration(0) {}

/*
 * @brief Destructor.
 */
LSystem::~LSystem() {
    // No requiere limpieza especial
}

/*
 * @brief Carga las reglas de produccion desde un archivo.
 * @param filename Ruta al archivo de reglas.
 * @return true si se carga exitosamente, false en caso contrario.
 * @note El archivo debe tener el formato:
 *       axiom: [cadena]
 *       angle: [valor]
 *       [simbolo]->[reemplazo]
 */
bool LSystem::loadRules(const char* filename) {
    constexpr size_t PREFIX_LENGTH = 6;
    constexpr size_t ARROW_LENGTH = 2;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << '\n';
        return false;
    }

    rules.clear();
    std::string line;

    // (c): Parsear el archivo linea por linea
    while (std::getline(file, line)) {
        // Eliminar espacios en blanco al inicio
        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) {
            continue;  // Linea vacia
        }

        line = line.substr(start);

        // Parsear axioma
        if (line.find("axiom:") == 0) {
            axiom = line.substr(PREFIX_LENGTH);
            // Eliminar espacios
            axiom.erase(0, axiom.find_first_not_of(" \t"));
            currentString = axiom;
            std::cout << "Axioma cargado: " << axiom << '\n';
        }
        // Parsear angulo
        else if (line.find("angle:") == 0) {
            std::string angleStr = line.substr(PREFIX_LENGTH);
            angle = std::stof(angleStr);
            std::cout << "Angulo cargado: " << angle << " grados\n";
        }
        // Parsear reglas de produccion (formato: A->BC)
        else {
            size_t arrowPos = line.find("->");
            if (arrowPos != std::string::npos && arrowPos > 0) {
                char symbol = line[0];
                std::string replacement = line.substr(arrowPos + ARROW_LENGTH);
                rules[symbol] = replacement;
                std::cout << "Regla cargada: " << symbol << " -> " << replacement << '\n';
            }
        }
    }

    file.close();
    currentGeneration = 0;
    return true;
}

/*
 * @brief Genera la cadena del L-System aplicando las reglas 'n' veces.
 * @param generations Numero de iteraciones a aplicar.
 * @note Implementa reescritura paralela: todos los simbolos se
 *       reemplazan simultaneamente en cada generacion.
 */
void LSystem::generate(int generations) {
    // (c): Resetear a axioma antes de generar
    currentString = axiom;
    currentGeneration = 0;

    // (c): Aplicar reglas de produccion 'n' veces
    for (int gen = 0; gen < generations; gen++) {
        std::string nextString;

        // (c): Reescritura paralela - procesar cada simbolo
        // de la cadena actual simultaneamente
        for (char symbol : currentString) {
            // Si existe una regla para este simbolo, aplicarla
            if (rules.find(symbol) != rules.end()) {
                nextString += rules[symbol];
            }
            // Si no hay regla, el simbolo se copia tal cual (constante)
            else {
                nextString += symbol;
            }
        }

        // (c): Actualizar la cadena actual con la nueva generacion
        currentString = nextString;
        currentGeneration++;
    }

    std::cout << "Generacion " << currentGeneration << " completada.\n";
    std::cout << "Longitud de cadena: " << currentString.length() << " simbolos\n";
}

/*
 * @brief Obtiene la cadena generada actual.
 * @return Cadena del L-System.
 */
std::string LSystem::getString() const {
    return currentString;
}

/*
 * @brief Obtiene el angulo de rotacion.
 * @return Angulo en grados.
 */
float LSystem::getAngle() const {
    return angle;
}

/*
 * @brief Obtiene el axioma inicial.
 * @return Cadena del axioma.
 */
std::string LSystem::getAxiom() const {
    return axiom;
}

/*
 * @brief Obtiene la generacion actual.
 * @return Numero de iteraciones aplicadas.
 */
int LSystem::getGeneration() const {
    return currentGeneration;
}

/*
 * @brief Reinicia el L-System al estado inicial (axioma).
 */
void LSystem::reset() {
    currentString = axiom;
    currentGeneration = 0;
    std::cout << "L-System reiniciado al axioma.\n";
}

/*
 * @brief Establece el axioma directamente.
 */
void LSystem::setAxiom(const std::string& newAxiom) {
    axiom = newAxiom;
    currentString = axiom;
    currentGeneration = 0;
}

/*
 * @brief Establece el angulo directamente.
 */
void LSystem::setAngle(float newAngle) {
    angle = newAngle;
}

/*
 * @brief Agrega una regla de produccion.
 */
void LSystem::addRule(char symbol, const std::string& replacement) {
    rules[symbol] = replacement;
}

/*
 * @brief Limpia todas las reglas de produccion.
 */
void LSystem::clearRules() {
    rules.clear();
}
