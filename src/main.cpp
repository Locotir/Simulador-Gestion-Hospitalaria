#include <iostream>   // Proporciona funcionalidades de entrada y salida (cin, cout).
#include <fstream>    // Permite el manejo de flujos de archivos (lectura y escritura en archivos).
#include <csignal>    // Proporciona funciones para manejar eventos asincronos (senales).
#include <cstring>    // Proporciona funciones para manejar cadenas de caracteres estilo C (e.g., strcpy, strlen).
#include <sstream>    // Facilita las operaciones de flujo de cadenas para manipular cadenas como flujos.
#include <vector>     // Implementa el contenedor vector para el manejo de arreglos dinamicos.
#include <algorithm>  // Ofrece una coleccion de algoritmos (e.g., ordenamiento, busqueda).
#include <ctime>      // Proporciona funciones para manipular y obtener informacion sobre fechas y horas.
#include <string>
#include <stdexcept>
#include <filesystem>

#include "main.hpp" // Bcolors
#include "pacientes.hpp"
#include "medicos.hpp"
#include "citas.hpp"
#include "reportes.hpp"
#include "archivos.hpp" 


// Directorio de las bases de datos CSV
namespace fs = std::filesystem;
const std::string db_pacientes = "db/pacientes.csv";
const std::string db_medicos = "db/medicos.csv";
const std::string db_citas = "db/citas.csv";
const std::string db_historialClinico = "db/historial-clinico.csv";

const std::string bcolors::PURPLE = "\033[95m";
const std::string bcolors::BLUE = "\033[94m";
const std::string bcolors::BLUEL = "\033[96m";
const std::string bcolors::GREEN = "\033[92m";
const std::string bcolors::YELLOW = "\033[93m";
const std::string bcolors::RED = "\033[91m";
const std::string bcolors::UNDERLINE = "\033[4m";
const std::string bcolors::WHITE = "\033[37m";
const std::string bcolors::ORANGE = "\033[38;5;208m";
const std::string bcolors::VIOLET = "\033[38;5;135m";
const std::string bcolors::BLACK = "\033[30m";
const std::string bcolors::RESET = "\033[0m";

void gestionarPacientes(int operacion, int id, int edad, std::string nombre) {

    std::vector<Paciente> pacientes = cargarPacientes(db_pacientes);
    
    // Si no se tiene ID, pero se tiene nombre y edad, o si no se tiene ID y nombre/edad, pedimos esos datos
    if (id == -1) {
        // Si no se tienen nombre y edad, pedimos los datos al usuario
        if (nombre.empty() || edad == -1) {
            std::cout << "\nIngrese la edad del paciente: " << bcolors::GREEN;
            std::cin >> edad;
            std::cin.ignore();  // Limpiar el buffer de entrada
            std::cout << bcolors::YELLOW << "Ingrese el nombre del paciente: " << bcolors::GREEN;
            std::getline(std::cin, nombre);
        }

        // Buscar paciente por nombre y edad
        bool encontrado = false;
        for (const auto& paciente : pacientes) {
            if (paciente.getEdad() == edad && paciente.getNombre() == nombre) {
                id = paciente.getId();  // Asignar el id encontrado
                paciente.mostrarInfo();  // Mostrar la información del paciente
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            std::cout << "No se encontró ningún paciente con los datos proporcionados (edad: " << edad << ", nombre: " << nombre << ").\n";
            return;  // Si no se encuentra, terminamos la función
        }
    }
    // A partir de aquí, ya tenemos el id y no necesitamos pedirlo nuevamente
    switch (operacion) {
        case 1: {  // Alta/Baja
            try {
                auto it = std::find_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) {
                    return p.getId() == id;
                });

                if (it != pacientes.end()) {
                    int nuevaDisponibilidad;
                    std::cout << "\n¿Desea dar de alta (1) o de baja (0) al paciente? ";
                    std::cin >> nuevaDisponibilidad;

                    // Validar si ya está en el estado solicitado
                    if (it->getDisponibilidad() == nuevaDisponibilidad) {
                        std::cout << it->getNombre() 
                                << " ya está " << (nuevaDisponibilidad == 1 ? "de alta" : "de baja") 
                                << ".\n";
                    } else {
                        it->setDisponibilidad(nuevaDisponibilidad);  // Actualizar disponibilidad
                        std::cout << "\n[" << bcolors::GREEN << "+" << bcolors::WHITE << "] " << it->getNombre() 
                                << " ha sido dado " << (nuevaDisponibilidad == 1 ? "de alta" : "de baja") 
                                << " correctamente.\n";

                        // Solo guardar los cambios una vez
                        guardarPacientes("db/pacientes.csv", pacientes);
                    }
                } else {
                    throw std::runtime_error("Paciente no encontrado para realizar la operación.");
                }
            } catch (const std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
            break;
        }
        case 2: {  // Modificar Datos
            Paciente paciente = buscarPaciente(id, pacientes);
            paciente.mostrarInfo();  // Mostrar la información del paciente
            // Lógica para modificar datos
            break;
        }
        case 3: {  // Realizar Busqueda
            try {
                Paciente paciente = buscarPaciente(id, pacientes);
                paciente.mostrarInfo();
            } catch (const std::runtime_error& e) {
                std::cout << e.what() << std::endl;
            }
            break;
        }
        case 4: {  // Historial Clínico
            std::string registro;
            std::cout << "Ingrese el registro para el historial clínico: ";
            std::cin.ignore();
            std::getline(std::cin, registro);
            agregarHistorialClinico(id, registro);
            break;
        }
        default:
            std::cout << "Opción no válida." << std::endl;
            break;
    }
}





void realizarBackup() {
    // Directorio donde se encuentran los archivos CSV
    fs::path dir = "./db";  

    // Comprobar si el directorio existe
    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        std::cerr << "El directorio no existe o no es válido." << std::endl;
        return;
    }

    // Iterar sobre los archivos en el directorio
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            std::string originalPath = entry.path().string();
            std::string backupPath = originalPath + ".BACKUP";

            // Abrir archivo original
            std::ifstream src(originalPath, std::ios::binary);
            if (!src) {
                std::cerr << "No se pudo abrir el archivo: " << originalPath << std::endl;
                continue;
            }

            // Crear archivo de backup
            std::ofstream dst(backupPath, std::ios::binary);
            if (!dst) {
                std::cerr << "No se pudo crear el archivo de backup: " << backupPath << std::endl;
                continue;
            }

            // Copiar contenido del archivo original al de backup
            dst << src.rdbuf();

            std::cout << "Backup creado para: " << originalPath << " -> " << backupPath << std::endl;
        }
    }
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Manegar CTL + C Exit +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void signalHandler([[maybe_unused]] int signum) {
    std::cout << bcolors::WHITE << "\n\n[" << bcolors::RED << "!" << bcolors::WHITE << "] ""Exiting...\n";
    exit(0); // Exit program
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Mostrar Mensaje de Ayuda +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void showHelp() {
    std::cout << "Uso: ./SGH [opciones]\n"
              << "Opciones:\n"
              << "  -X <buscar>       Buscar algo\n"
              << "  -h                Mostrar este mensaje\n"
              << "Ejemplos:\n"
              << "  ./SGH [ejemplo]\n"
              << "  ./SGH [otro_ejemplo]\n\n" 
              << bcolors::GREEN << "Si se ejecuta sin argumentos, el modo interactivo empezara." << std::endl;
} 

int main(int argc, char *argv[]) { // Coger argumentos de ejecuccion
    std::signal(SIGINT, signalHandler); // Interceptar CTL + C

    if (argc == 1) { // Modo interactivo :> sin argumentos
    int opcion;
    int operacion;
    std::cout << bcolors::YELLOW << R"(
       ______          __  _                __  __                 _ __        __           _      
      / ____/__  _____/ /_(_)___  ____     / / / /___  _________  (_) /_____ _/ /___ ______(_)___ _
     / / __/ _ \/ ___/ __/ / __ \/ __ \   / /_/ / __ \/ ___/ __ \/ / __/ __ `/ / __ `/ ___/ / __ `/
    / /_/ /  __(__  ) /_/ / /_/ / / / /  / __  / /_/ (__  ) /_/ / / /_/ /_/ / / /_/ / /  / / /_/ / 
    \____/\___/____/\__/_/\____/_/ /_/  /_/ /_/\____/____/ .___/_/\__/\__,_/_/\__,_/_/  /_/\__,_/  
    )" << bcolors::RESET << std::endl;

    // Mostrar Opciones
    std::cout << bcolors::GREEN <<  "\n1. Gestionar Pacientes" << bcolors::RESET << std::endl;
    std::cout << bcolors::BLUEL <<  "\n2. Gestionar Medicos" << bcolors::RESET << std::endl;
    std::cout << bcolors::RED <<    "\n3. Gestionar Citas Medicas" << bcolors::RESET << std::endl;
    std::cout << bcolors::PURPLE << "\n4. Realizar Backup" << bcolors::RESET << std::endl;
    std::cout << bcolors::YELLOW << "\n\n Opcion => " << bcolors::RESET << std::endl;
    std::cin >> opcion;

    switch (opcion) {
        case 1:
            std::cout << bcolors::GREEN << "\n1. Manejar el Alta/Baja";
            std::cout << bcolors::GREEN << "\n2. Modificar Datos";
            std::cout << bcolors::GREEN << "\n3. Realizar Busqueda";
            std::cout << bcolors::GREEN << "\n4. Historial Clinico";
            std::cout << bcolors::YELLOW << "\n\n Operacion => " << bcolors::WHITE;
            std::cin >> operacion;

            gestionarPacientes(operacion, -1, -1, "");  // Función para gestionar pacientes
            break;

        case 2:
            std::cout << bcolors::BLUEL << "\n1. Manejar el Alta/Baja";
            std::cout << bcolors::BLUEL << "\n2. Asignar Especialidad";
            std::cout << bcolors::BLUEL << "\n3. Listar * Especialidad/Disponibilidad";
            std::cout << bcolors::YELLOW << "\n\n Operacion => " << bcolors::WHITE;
            std::cin >> operacion;
            //gestionarMedicos();    // Función para gestionar medicos
            break;
        case 3:
            std::cout << bcolors::RED << "\n1. Asignar Cita";
            std::cout << bcolors::RED << "\n2. Ordenar Citas * Fecha/Urgencia";
            std::cout << bcolors::RED << "\n3. Registro de Citas";
            std::cout << bcolors::RED << "\n4. Cancelar/Modificar cita";
            std::cout << bcolors::YELLOW << "\n\n Operacion => " << bcolors::WHITE;
            std::cin >> operacion;
            //gestionarCitas(operacion);      // Funcion para gestionar citas
            break;
        case 4:
            realizarBackup();      // Función para realizar backup
            break;
        default:
            std::cout << bcolors::RED << "Opción inválida!" << std::endl << std::endl;
            showHelp();
        
            return 1;
    }

    } else { // Ejecutar argumentos en CLI
        std::cout << "Argumento recibido: " << argv[1] << std::endl;  // Mostrar el primer argumento

        // Ejemplo de cómo procesar mas de un argumento
        if (argc > 2) {
            std::cout << "Argumentos adicionales: ";
            for (int i = 2; i < argc; ++i) {
                std::cout << argv[i] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0; // Salida sin errores
}


