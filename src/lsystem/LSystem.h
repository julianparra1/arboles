/*
 * =================================================================
 * ARCHIVO: LSystem.h
 * CONTENIDO: Declaracion de la clase LSystem.
 * DESCRIPCION:
 * Este archivo define la estructura de la clase LSystem, que
 * implementa un sistema de Lindenmayer (D0L-system) para la
 * generacion procedural de cadenas. La clase maneja el alfabeto,
 * el axioma, y las reglas de produccion, aplicando reescritura
 * paralela para simular el crecimiento de plantas.
 * Esta clase NO tiene dependencias de OpenGL.
 *
 * AUTOR: Julian Parra
 * CURSO: Graficacion (2025)
 * PROYECTO: Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas" *
 * =================================================================
 */

#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <map>
#include <string>

/*
 * @class LSystem
 * @brief Representa un sistema de Lindenmayer (L-System) deterministico
 *        y libre de contexto (D0L-system).
 *
 * Un L-System consta de:
 * - Alfabeto (V): conjunto de simbolos (variables y constantes)
 * - Axioma (omega): cadena inicial
 * - Reglas de produccion (P): mapeo de simbolos a cadenas de reemplazo
 *
 * La clase permite cargar reglas desde un archivo de texto y generar
 * la cadena resultante aplicando las reglas de forma paralela durante
 * 'n' iteraciones.
 */
class LSystem {
private:
    std::string axiom;                  // Cadena inicial (omega)
    std::map<char, std::string> rules;  // Reglas de produccion (P)
    std::string currentString;          // Cadena actual (resultado)
    float angle;                        // Angulo de rotacion (delta) en grados
    int currentGeneration;              // Numero de generacion actual

public:
    /*
     * @brief Constructor por defecto.
     * Inicializa un L-System vacio.
     */
    LSystem();

    /*
     * @brief Destructor.
     */
    ~LSystem();

    /*
     * @brief Carga las reglas de produccion desde un archivo de texto.
     * @param filename Ruta del archivo que contiene el axioma, angulo y reglas.
     * @return true si la carga fue exitosa, false en caso contrario.
     * @note Formato esperado del archivo:
     *       axiom: [cadena]
     *       angle: [valor]
     *       [simbolo]->[cadena_reemplazo]
     *       Ejemplo:
     *       axiom: F++F++F
     *       angle: 60
     *       F->F-F++F-F
     */
    bool loadRules(const char* filename);

    /*
     * @brief Aplica las reglas de produccion para generar la cadena final.
     * @pre La instancia debe tener un axioma y reglas cargadas.
     * @param generations Numero de iteraciones 'n' a aplicar.
     * @return void. La cadena resultante se almacena en currentString.
     * @note Utiliza reescritura paralela: todas las reglas se aplican
     *       simultaneamente en cada generacion, simulando crecimiento
     *       biologico donde todas las celulas se dividen al mismo tiempo.
     */
    void generate(int generations);

    /*
     * @brief Obtiene la cadena generada actual.
     * @return La cadena del L-System despues de aplicar las reglas.
     */
    std::string getString() const;

    /*
     * @brief Obtiene el angulo de rotacion configurado.
     * @return Angulo en grados para las instrucciones de rotacion (+, -, etc.)
     */
    float getAngle() const;

    /*
     * @brief Obtiene el axioma inicial.
     * @return Cadena del axioma.
     */
    std::string getAxiom() const;

    /*
     * @brief Obtiene el numero de generacion actual.
     * @return Numero de iteraciones aplicadas.
     */
    int getGeneration() const;

    /*
     * @brief Reinicia el L-System al axioma inicial.
     */
    void reset();
};

#endif  // LSYSTEM_H
