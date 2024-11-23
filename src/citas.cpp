/*
TODO:
- Asignar citas a Pacientes/Medicos
- Ordenar citas por Fecha o Urgencia
- Registro de las Citas pasadas y Futuras
*/

#include "citas.hpp"

// Constructor
Cita::Cita(int id, const std::string& nombrePaciente, const std::string& nombreMedico, 
           const std::string& fecha, const std::string& descripcion)
    : id(id), nombrePaciente(nombrePaciente), nombreMedico(nombreMedico), 
      fecha(fecha), descripcion(descripcion) {}

// Métodos para mostrar información de la cita
void Cita::mostrarInfo() const {
    std::cout << "ID: " << id << "\n"
              << "Paciente: " << nombrePaciente << "\n"
              << "Médico: " << nombreMedico << "\n"
              << "Fecha: " << fecha << "\n"
              << "Descripción: " << descripcion << "\n";
}

// Getters
int Cita::getId() const { return id; }
std::string Cita::getNombrePaciente() const { return nombrePaciente; }
std::string Cita::getNombreMedico() const { return nombreMedico; }
std::string Cita::getFecha() const { return fecha; }
std::string Cita::getDescripcion() const { return descripcion; }

