#include "main.hpp" //bcolors
#include "medicos.hpp"

// Constructor de Medico
Medico::Medico(int id, const std::string& nombre, const std::string& especialidad, int disponibilidad)
    : id(id), nombre(nombre), especialidad(especialidad), disponibilidad(disponibilidad) {}

// Mostrar la información del médico
void Medico::mostrarInfo() const {
    std::cout << bcolors::VIOLET << "\nID: " << bcolors::RESET << id 
              << bcolors::VIOLET << "\nNombre: " << bcolors::RESET << nombre 
              << bcolors::VIOLET << "\nEspecialidad: " << bcolors::RESET << especialidad 
              << bcolors::VIOLET << "\nDisponibilidad: " 
              << bcolors::WHITE << (disponibilidad == 1 ? "Disponible" : "No Disponible") << std::endl;
}

int Medico::getId() const {
    return id;
}

std::string Medico::getNombre() const {
    return nombre;
}

std::string Medico::getEspecialidad() const {
    return especialidad;
}

int Medico::getDisponibilidad() const {
    return disponibilidad;
}

void Medico::setNombre(const std::string& nuevoNombre) {
    nombre = nuevoNombre;
}

void Medico::setEspecialidad(const std::string& nuevaEspecialidad) {
    especialidad = nuevaEspecialidad;
}

void Medico::setDisponibilidad(int nuevaDisponibilidad) {
    disponibilidad = nuevaDisponibilidad;
}

// Función para cargar médicos desde un archivo CSV
std::vector<Medico> cargarMedicos(const std::string& filename) {
    std::vector<Medico> medicos;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Error al abrir el archivo: " + filename);
    }

    std::string linea;
    std::getline(file, linea);  // Saltar la primera línea (encabezado)

    while (std::getline(file, linea)) {
        std::stringstream ss(linea);
        std::string token;

        int id, disponibilidad;
        std::string nombre, especialidad;

        try {
            // Leer ID
            std::getline(ss, token, ',');
            id = std::stoi(token);  // Convertir a int

            // Leer nombre
            std::getline(ss, nombre, ',');

            // Leer especialidad
            std::getline(ss, especialidad, ',');

            // Leer disponibilidad (debe ser 0 o 1)
            std::getline(ss, token, ',');
            disponibilidad = std::stoi(token);  // Convertir a int (0 o 1)

            // Crear médico y agregarlo al vector
            medicos.emplace_back(id, nombre, especialidad, disponibilidad);
        } catch (const std::invalid_argument& e) {
            // No hacer nada en caso de error de conversión
        } catch (const std::out_of_range& e) {
            // No hacer nada en caso de número fuera de rango
        }
    }

    return medicos;
}

// Función para guardar médicos en un archivo CSV
void guardarMedicos(const std::string& filename, const std::vector<Medico>& medicos) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error al abrir el archivo para guardar médicos.");
    }

    // Agregar encabezado
    file << "ID,nombre,especialidad,disponibilidad\n";

    for (const auto& medico : medicos) {
        file << medico.getId() << ","
             << medico.getNombre() << ","
             << medico.getEspecialidad() << ","
             << medico.getDisponibilidad() << "\n";
    }
}

// Función para eliminar un médico por ID
void eliminarMedico(int id, std::vector<Medico>& medicos) {
    auto it = std::remove_if(medicos.begin(), medicos.end(), [id](const Medico& m) {
        return m.getId() == id;
    });
    medicos.erase(it, medicos.end());
}

// Función para modificar los datos de un médico
void modificarMedico(int id, std::vector<Medico>& medicos, const std::string& nuevoNombre, const std::string& nuevaEspecialidad, int nuevaDisponibilidad) {
    auto medico = std::find_if(medicos.begin(), medicos.end(), [id](const Medico& m) { return m.getId() == id; });

    if (medico != medicos.end()) {
        medico->setNombre(nuevoNombre);
        medico->setEspecialidad(nuevaEspecialidad);
        medico->setDisponibilidad(nuevaDisponibilidad);
        return;
    }

    throw std::runtime_error("Médico no encontrado para modificar.");
}

// Función para buscar médicos por especialidad
std::vector<Medico> buscarMedicosPorEspecialidad(const std::string& especialidad, const std::vector<Medico>& medicos) {
    std::vector<Medico> resultados;
    std::copy_if(medicos.begin(), medicos.end(), std::back_inserter(resultados), [&especialidad](const Medico& m) {
        return m.getEspecialidad() == especialidad;
    });

    if (resultados.empty()) {
        throw std::runtime_error("No se encontraron médicos con esa especialidad.");
    }

    return resultados;
}

// Función para buscar médicos por disponibilidad
std::vector<Medico> buscarMedicosPorDisponibilidad(int disponibilidad, const std::vector<Medico>& medicos) {
    std::vector<Medico> resultados;
    std::copy_if(medicos.begin(), medicos.end(), std::back_inserter(resultados), [disponibilidad](const Medico& m) {
        return m.getDisponibilidad() == disponibilidad;
    });

    if (resultados.empty()) {
        throw std::runtime_error("No se encontraron médicos con esa disponibilidad.");
    }

    return resultados;
}
