#include "pacientes.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdexcept>

// Constructor de Paciente
Paciente::Paciente(int id, const std::string& nombre, int edad, const std::string& fechaIngreso, int disponibilidad)
    : id(id), nombre(nombre), edad(edad), fechaIngreso(fechaIngreso), disponibilidad(disponibilidad) {}

// Mostrar la informacion del paciente
void Paciente::mostrarInfo() const {
    std::cout << "ID: " << id << "\nNombre: " << nombre << "\nEdad: " << edad 
              << "\nFecha de Ingreso: " << fechaIngreso << "\nDisponibilidad: " 
              << (disponibilidad == 1 ? "Dado de alta" : "Dado de baja") << std::endl;
}

int Paciente::getEdad() const {
    return edad;
}

std::string Paciente::getNombre() const {
    return nombre;
}

int Paciente::getId() const {
    return id;
}

std::string Paciente::getFechaIngreso() const {
    return fechaIngreso;
}

int Paciente::getDisponibilidad() const {
    return disponibilidad;
}

void Paciente::setNombre(const std::string& nuevoNombre) {
    nombre = nuevoNombre;
}

void Paciente::setEdad(int nuevaEdad) {
    edad = nuevaEdad;
}

void Paciente::setDisponibilidad(int nuevaDisponibilidad) {
    disponibilidad = nuevaDisponibilidad;
}

// Función para cargar pacientes desde un archivo CSV
std::vector<Paciente> cargarPacientes(const std::string& filename) {
    std::vector<Paciente> pacientes;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Error al abrir el archivo: " + filename);
    }

    std::string linea;
    std::getline(file, linea);  // Saltar la primera línea (encabezado)

    while (std::getline(file, linea)) {
        std::stringstream ss(linea);
        std::string token;

        int id, edad, disponibilidad;
        std::string nombre, fechaIngreso;

        // Leer ID
        std::getline(ss, token, ',');
        id = std::stoi(token);  // Convertir a int

        // Leer nombre
        std::getline(ss, nombre, ',');

        // Leer edad
        std::getline(ss, token, ',');
        edad = std::stoi(token);  // Convertir a int

        // Leer fecha de ingreso
        std::getline(ss, fechaIngreso, ',');

        // Leer disponibilidad (debe ser 0 o 1)
        std::getline(ss, token, ',');
        disponibilidad = std::stoi(token);  // Convertir a int (0 o 1)

        // Crear paciente y agregarlo al vector
        pacientes.emplace_back(id, nombre, edad, fechaIngreso, disponibilidad);
    }

    return pacientes;
}

// Función para guardar pacientes en un archivo CSV
void guardarPacientes(const std::string& filename, const std::vector<Paciente>& pacientes) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error al abrir el archivo para guardar pacientes.");
    }

    // Agregar encabezado
    file << "ID,nombre,edad,fechaIngreso,disponibilidad\n";

    for (const auto& paciente : pacientes) {
        file << paciente.getId() << "," 
             << paciente.getNombre() << ","
             << paciente.getEdad() << ","
             << paciente.getFechaIngreso() << ","
             << paciente.getDisponibilidad() << "\n";
    }
}


// Función para dar de alta un paciente
void darDeAlta(int id, std::vector<Paciente>& pacientes) {
    auto it = std::find_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) {
        return p.getId() == id;
    });

    if (it != pacientes.end()) {
        it->setDisponibilidad(1); // Alta
        return;
    }

    throw std::runtime_error("Paciente no encontrado para dar de alta.");
}

// Función para dar de baja un paciente
void darDeBaja(int id, std::vector<Paciente>& pacientes) {
    auto it = std::find_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) {
        return p.getId() == id;
    });

    if (it != pacientes.end()) {
        it->setDisponibilidad(0); // Baja
        return;
    }

    throw std::runtime_error("Paciente no encontrado para dar de baja.");
}

// Función para eliminar un paciente por ID
void eliminarPaciente(int id, std::vector<Paciente>& pacientes) {
    auto it = std::remove_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) {
        return p.getId() == id;
    });
    pacientes.erase(it, pacientes.end());
}

// Función para modificar los datos de un paciente
void modificarPaciente(int id, std::vector<Paciente>& pacientes, const std::string& nuevoNombre, int nuevaEdad, int nuevaDisponibilidad) {
    auto paciente = std::find_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) { return p.getId() == id; });

    if (paciente != pacientes.end()) {
        paciente->setNombre(nuevoNombre);
        paciente->setEdad(nuevaEdad);
        paciente->setDisponibilidad(nuevaDisponibilidad);
        return;
    }

    throw std::runtime_error("Paciente no encontrado para modificar.");
}

// Buscar un paciente por ID
Paciente buscarPaciente(int id, const std::vector<Paciente>& pacientes) {
    auto it = std::find_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) {
        return p.getId() == id;
    });

    if (it != pacientes.end()) {
        return *it;
    }

    throw std::runtime_error("Paciente no encontrado.");
}

// Buscar un paciente por nombre
Paciente buscarPacientePorNombre(const std::string& nombre, const std::vector<Paciente>& pacientes) {
    auto it = std::find_if(pacientes.begin(), pacientes.end(), [&nombre](const Paciente& p) {
        return p.getNombre() == nombre;
    });

    if (it != pacientes.end()) {
        return *it;
    }

    throw std::runtime_error("Paciente no encontrado por nombre.");
}

// Buscar pacientes por fecha de ingreso
std::vector<Paciente> buscarPacientePorFechaIngreso(const std::string& fechaIngreso, const std::vector<Paciente>& pacientes) {
    std::vector<Paciente> resultados;
    std::copy_if(pacientes.begin(), pacientes.end(), std::back_inserter(resultados), [&fechaIngreso](const Paciente& p) {
        return p.getFechaIngreso() == fechaIngreso;
    });

    if (resultados.empty()) {
        throw std::runtime_error("No se encontraron pacientes con esa fecha de ingreso.");
    }

    return resultados;
}

// Función para agregar un registro al historial clínico
void agregarHistorialClinico(int pacienteID, const std::string& registro) {
    std::ofstream file("db/historial-clinico.csv", std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Error al abrir el archivo de historial clínico.");
    }
    file << pacienteID << "," << registro << "\n";
}

// Función para cargar el historial clínico de un archivo
void cargarHistorialClinico(const std::string& filename) {
    std::ifstream file(filename);
    std::string linea;

    while (std::getline(file, linea)) {
        std::stringstream ss(linea);
        std::string token;
        
        int pacienteID;
        std::string registro;
        
        std::getline(ss, token, ','); // ID paciente
        pacienteID = std::stoi(token);
        std::getline(ss, registro);  // Registro clínico

        std::cout << "Historial del Paciente ID " << pacienteID << ": " << registro << std::endl;
    }
}

