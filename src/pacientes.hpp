#ifndef PACIENTES_HPP
#define PACIENTES_HPP

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>  // Necesario para std::runtime_error
#include "citas.hpp"  // Acceder a la clase Cita
#include "medicos.hpp"

// Clase Paciente que contiene la información de un paciente.
class Paciente {
public:
    // Constructor
    Paciente(int id, const std::string& nombre, int edad, const std::string& fechaIngreso, int disponibilidad);

    // Métodos para mostrar información del paciente
    void mostrarInfo() const;

    // Actualizar historial clínico
    void actualizarHistorial(const std::string& nuevoHistorial);

    // Getters
    int getId() const;
    std::string getNombre() const;
    int getEdad() const;
    std::string getFechaIngreso() const;
    int getDisponibilidad() const;

    // Setters
    void setDisponibilidad(int nuevaDisponibilidad);
    void setEdad(int nuevaEdad);
    void setNombre(const std::string& nuevoNombre);

private:
    int id;
    std::string nombre;
    int edad;
    std::string fechaIngreso;
    int disponibilidad;
};

// Funciones para gestionar pacientes
std::vector<Paciente> cargarPacientes(const std::string& filename);
void guardarPacientes(const std::string& filename, const std::vector<Paciente>& pacientes);
void eliminarPaciente(int id, std::vector<Paciente>& pacientes);
Paciente buscarPaciente(int id, const std::vector<Paciente>& pacientes);

// Función para modificar los datos de un paciente
void modificarPaciente(int id, std::vector<Paciente>& pacientes, const std::string& nuevoNombre, int nuevaEdad, int nuevaDisponibilidad);

// Funciones para gestionar citas
std::vector<Cita> cargarCitas(const std::string& filename);
void guardarCitas(const std::string& filename, const std::vector<Cita>& citas);
void eliminarCita(int id, std::vector<Cita>& citas);
Cita buscarCita(int id, const std::vector<Cita>& citas);

// Funciones para gestionar historial clínico
void agregarHistorialClinico(int pacienteID, const std::string& registro);
void cargarHistorialClinico(const std::string& filename, const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos, size_t idPacienteBuscado);

#endif // PACIENTES_HPP
