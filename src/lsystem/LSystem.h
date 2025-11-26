/**
 * @file LSystem.h
 * @brief Implementacion de Sistema de Lindenmayer (L-System) para generacion procedural.
 *
 * Implementa un L-System deterministico y libre de contexto (D0L-system) para
 * generar cadenas mediante reglas de reescritura paralela. Esta clase maneja
 * el alfabeto, axioma y reglas de produccion para simular patrones de
 * crecimiento de plantas.
 *
 * Esta clase NO tiene dependencias de OpenGL - solo genera cadenas.
 *
 * @author Julian Parra
 * @date 2025
 * @course Graficacion
 * @project Proyecto Final - "Arboles: La Belleza Algoritmica de las Plantas"
 */

#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <map>
#include <string>

/**
 * @class LSystem
 * @brief Representa un Sistema de Lindenmayer deterministico y libre de contexto.
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

    /*
     * @brief Establece el axioma directamente.
     * @param newAxiom Nuevo axioma.
     */
    void setAxiom(const std::string& newAxiom);

    /*
     * @brief Establece el angulo directamente.
     * @param newAngle Nuevo angulo en grados.
     */
    void setAngle(float newAngle);

    /*
     * @brief Agrega una regla de produccion.
     * @param symbol Simbolo a reemplazar.
     * @param replacement Cadena de reemplazo.
     */
    void addRule(char symbol, const std::string& replacement);

    /*
     * @brief Limpia todas las reglas de produccion.
     */
    void clearRules();
};

#endif  // LSYSTEM_H
