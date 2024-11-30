#ifndef MEDICOS_HPP
#define MEDICOS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>  // Necesario para std::runtime_error

// Clase Medico que contiene la información de un médico
class Medico {
public:
    // Constructor
    Medico(int id, const std::string& nombre, const std::string& especialidad, int disponibilidad);

    // Métodos para mostrar información del médico
    void mostrarInfo() const;

    // Getters
    int getId() const;
    std::string getNombre() const;
    std::string getEspecialidad() const;
    int getDisponibilidad() const;

    // Setters
    void setDisponibilidad(int nuevaDisponibilidad);
    void setEspecialidad(const std::string& nuevaEspecialidad);
    void setNombre(const std::string& nuevoNombre);

private:
    int id;
    std::string nombre;
    std::string especialidad;
    int disponibilidad;
};

// Funciones para gestionar médicos
std::vector<Medico> cargarMedicos(const std::string& filename);
void guardarMedicos(const std::string& filename, const std::vector<Medico>& medicos);
void eliminarMedico(int id, std::vector<Medico>& medicos);
Medico buscarMedico(int id, const std::vector<Medico>& medicos);

// Función para modificar los datos de un médico
void modificarMedico(int id, std::vector<Medico>& medicos, const std::string& nuevoNombre, const std::string& nuevaEspecialidad, int nuevaDisponibilidad);

// Funciones adicionales para gestión de médicos
std::vector<Medico> buscarMedicosPorEspecialidad(const std::string& especialidad, const std::vector<Medico>& medicos);
std::vector<Medico> buscarMedicosPorDisponibilidad(int disponibilidad, const std::vector<Medico>& medicos);

#endif // MEDICOS_HPP
