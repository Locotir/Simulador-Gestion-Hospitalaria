#ifndef BCOLORS_H
#define BCOLORS_H


#include <iostream>         // Proporciona funcionalidades de entrada y salida (cin, cout).
#include <fstream>          // Permite el manejo de flujos de archivos (lectura y escritura en archivos).
#include <csignal>          // Proporciona funciones para manejar eventos asincronos (señales).
#include <cstring>          // Proporciona funciones para manejar cadenas de caracteres estilo C (e.g., strcpy, strlen).
#include <sstream>          // Facilita las operaciones de flujo de cadenas para manipular cadenas como flujos.
#include <vector>           // Implementa el contenedor vector para el manejo de arreglos dinámicos.
#include <algorithm>        // Ofrece una colección de algoritmos (e.g., ordenamiento, búsqueda).
#include <ctime>            // Proporciona funciones para manipular y obtener información sobre fechas y horas.
#include <string>           // Proporciona la clase std::string para manejo de cadenas de texto dinámicas.
#include <stdexcept>        // Permite lanzar y capturar excepciones estándar.
#include <filesystem>       // Proporciona funcionalidades para interactuar con el sistema de archivos (e.g., crear, eliminar archivos y directorios).
#include <unordered_map>    // Implementa la estructura de datos mapa sin orden para almacenamiento eficiente de claves y valores.
#include <cctype>           // Para funciones de manipulación de caracteres, como std::tolower.
#include <sstream>          // Proporciona flujos de cadenas para operaciones de entrada/salida en memoria.

#ifdef UTF8_SUPPORT
#include <locale>           // Necesario para setlocale y soporte de locales, especialmente en UTF-8.
#endif

// ++++++++++++++++++++++++++++++++++++++++++++++++++++ Clase de Codigos de Color ++++++++++++++++++++++++++++++++++++++++++++++++++++ 
class bcolors {
public:
    static const std::string PURPLE;
    static const std::string BLUE;
    static const std::string BLUEL;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string RED;
    static const std::string UNDERLINE;
    static const std::string WHITE;
    static const std::string ORANGE;
    static const std::string VIOLET;
    static const std::string MAGENTA;
    static const std::string BLACK;
    static const std::string RESET;
};

#endif
