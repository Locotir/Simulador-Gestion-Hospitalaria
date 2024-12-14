#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "pacientes.hpp"
#include "medicos.hpp"
#include "main.hpp"

class HistorialClinico {
private:
    int id;
    int idPaciente;
    int idMedico;
    std::string fecha;
    std::string descripcion;

public:
    HistorialClinico(int id, int idPaciente, int idMedico, const std::string& fecha, const std::string& descripcion)
        : id(id), idPaciente(idPaciente), idMedico(idMedico), fecha(fecha), descripcion(descripcion) {}

    int getIdPaciente() const { return idPaciente; }
    int getIdMedico() const { return idMedico; }
    const std::string& getFecha() const { return fecha; }
    const std::string& getDescripcion() const { return descripcion; }

    void mostrarInfo(const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos) const {
        // Buscar paciente por id
        auto itPaciente = std::find_if(pacientes.begin(), pacientes.end(), [this](const Paciente& paciente) {
            return paciente.getId() == this->idPaciente;
        });

        // Buscar médico por id
        auto itMedico = std::find_if(medicos.begin(), medicos.end(), [this](const Medico& medico) {
            return medico.getId() == this->idMedico;
        });

        // Mostrar información del historial clínico
        std::cout << bcolors::RED << "\nID Historial: " << bcolors::RESET << id << std::endl;

        if (itPaciente != pacientes.end()) {
            std::cout << bcolors::VIOLET << "Paciente: " << bcolors::RESET << itPaciente->getNombre() << std::endl;
        } else {
            std::cout << bcolors::VIOLET << "Paciente: No encontrado" << std::endl;
        }

        if (itMedico != medicos.end()) {
            std::cout << bcolors::VIOLET << "Médico: " << bcolors::RESET << itMedico->getNombre() << std::endl;
        } else {
            std::cout << bcolors::VIOLET << "Médico: No encontrado" << std::endl;
        }

        std::cout << bcolors::BLUE << "Fecha: " << bcolors::RESET << fecha << std::endl
                  << bcolors::BLUE << "Descripción: " << bcolors::RESET << descripcion << std::endl;
    }
};

std::vector<HistorialClinico> cargarHistorialClinico(const std::string& archivo) {
    std::vector<HistorialClinico> historiales;
    std::ifstream file(archivo);

    if (!file.is_open()) {
        std::cerr << bcolors::RED << "Error: No se pudo abrir el archivo " << archivo << bcolors::RESET << std::endl;
        return historiales;
    }

    std::string linea;
    std::getline(file, linea); // Saltar la cabecera

    while (std::getline(file, linea)) {
        std::istringstream stream(linea);
        std::string campo;
        std::vector<std::string> campos;

        while (std::getline(stream, campo, ',')) {
            campos.push_back(campo);
        }

        if (campos.size() == 5) {
            historiales.emplace_back(std::stoi(campos[0]), std::stoi(campos[1]), std::stoi(campos[2]),
                                     campos[3], campos[4]);
        }
    }

    file.close();
    return historiales;
}

void mostrarHistorialClinico(const std::vector<HistorialClinico>& historiales, const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos) {
    if (historiales.empty()) {
        std::cout << bcolors::YELLOW << "No hay historiales clínicos disponibles." << bcolors::RESET << std::endl;
        return;
    }

    for (const auto& historial : historiales) {
        historial.mostrarInfo(pacientes, medicos);
        std::cout << std::endl;
    }
}

std::string convertirFecha(const std::string& fecha) {
    // Se espera que la fecha esté en el formato "dd-mm-yyyy"
    if (fecha.size() != 10) return fecha;

    return fecha.substr(6, 4) + "-" + fecha.substr(3, 2) + "-" + fecha.substr(0, 2);  // Formato "yyyy-mm-dd"
}


void listarPacientesPorFecha(const std::vector<HistorialClinico>& historiales, 
                             const std::vector<Paciente>& pacientes, 
                             const std::string& fechaInicio, 
                             const std::string& fechaFin) {
    std::cout << bcolors::VIOLET << "\n=========== Pacientes atendidos del " 
              << bcolors::RED << fechaInicio << bcolors::VIOLET 
              << " al " << bcolors::RED << fechaFin 
              << bcolors::VIOLET << " ===========\n" << bcolors::RESET;
    
    // Convierte las fechas de inicio y fin a "yyyy-mm-dd"
    std::string fechaInicioConvertida = convertirFecha(fechaInicio);
    std::string fechaFinConvertida = convertirFecha(fechaFin);

    for (const auto& historial : historiales) {
        std::string fechaHistorialConvertida = convertirFecha(historial.getFecha());

        if (fechaHistorialConvertida >= fechaInicioConvertida && fechaHistorialConvertida <= fechaFinConvertida) {
            auto itPaciente = std::find_if(pacientes.begin(), pacientes.end(), [&historial](const Paciente& paciente) {
                return paciente.getId() == historial.getIdPaciente();
            });

            if (itPaciente != pacientes.end()) {
                std::cout << "- " << bcolors::ORANGE<< itPaciente->getNombre() << bcolors::RESET << " (Fecha: " << historial.getFecha() << ")\n";
            }
        }
    }
}


void citasPendientesPorMedicoOEspecialidad(const std::vector<Cita>& citas, 
                                           const std::vector<Medico>& medicos, 
                                           const std::vector<Paciente>& pacientes, 
                                           const std::string& filtro) {
    std::cout << bcolors::VIOLET << "\n====================== Citas pendientes " 
          << (filtro.empty() ? "" : "para " + bcolors::RED + filtro) 
          << bcolors::VIOLET << " ======================\n" << bcolors::RESET;
    
    // Recorre todos los médicos
    for (const auto& medico : medicos) {
        // Compara el filtro con el nombre del médico o con la especialidad
        if (filtro.empty() || 
            medico.getEspecialidad() == filtro || 
            medico.getNombre() == filtro) {
            std::cout << bcolors::BLUEL << "Médico: " << medico.getNombre() << bcolors::GREEN << " (Especialidad: " << medico.getEspecialidad() << ")\n";
            
            // Recorre todas las citas
            for (const auto& cita : citas) {
                if (cita.getIdMedico() == medico.getId() && cita.getEstado() == "Pendiente") {
                    // Busca el paciente por su ID
                    auto itPaciente = std::find_if(pacientes.begin(), pacientes.end(), [&cita](const Paciente& paciente) {
                        return paciente.getId() == cita.getIdPaciente();
                    });

                    // Si el paciente existe, muestra el nombre del paciente
                    if (itPaciente != pacientes.end()) {
                        std::cout << bcolors::ORANGE << "  - Cita con paciente: " << bcolors::RESET << itPaciente->getNombre() 
                                  << " (ID Paciente: " << cita.getIdPaciente() << ") el " << cita.getFecha() << "\n";
                    }
                }
            }
        }
    }
}


void reportePacientesCronicos(const std::vector<HistorialClinico>& historiales, const std::vector<Paciente>& pacientes) {
    std::cout << bcolors::VIOLET << "\n=========== Pacientes con enfermedades crónicas ===========\n";
    
    for (const auto& historial : historiales) {
        // Buscar enfermedades crónicas en la descripción
        if (historial.getDescripcion().find("Sin enfermedades crónicas") == std::string::npos) {
            
            // Buscar el nombre del paciente utilizando el ID del paciente
            auto itPaciente = std::find_if(pacientes.begin(), pacientes.end(), 
                [&historial](const Paciente& paciente) {
                    return paciente.getId() == historial.getIdPaciente();
                });

            if (itPaciente != pacientes.end()) {
                std::cout << bcolors::BLUEL << "- " << itPaciente->getNombre() << bcolors::YELLOW <<" (ID " << historial.getIdPaciente() << ")" << bcolors::RESET << ":\n";
                std::cout << "  * " << historial.getDescripcion() << "\n\n";
            } else {
                std::cout << "- Paciente ID " << historial.getIdPaciente() << ": No encontrado\n";
            }
        }
    }
}

