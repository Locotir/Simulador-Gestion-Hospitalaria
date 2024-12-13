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


