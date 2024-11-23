


#ifndef CITAS_HPP
#define CITAS_HPP

#include <string>
#include <iostream>

class Cita {
public:
    // Constructor
    Cita(int id, const std::string& nombrePaciente, const std::string& nombreMedico, 
         const std::string& fecha, const std::string& descripcion);

    // Metodos para mostrar información de la cita
    void mostrarInfo() const;

    // Getters
    int getId() const;
    std::string getNombrePaciente() const;
    std::string getNombreMedico() const;
    std::string getFecha() const;
    std::string getDescripcion() const;

private:
    int id;
    std::string nombrePaciente;
    std::string nombreMedico;
    std::string fecha;
    std::string descripcion;
};

#endif // CITAS_HPP
