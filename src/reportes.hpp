#ifndef REPORTES_HPP
#define REPORTES_HPP

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
    HistorialClinico(int id, int idPaciente, int idMedico, const std::string& fecha, const std::string& descripcion);

    int getIdPaciente() const;
    int getIdMedico() const;
    void mostrarInfo(const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos) const;
};

std::vector<HistorialClinico> cargarHistorialClinico(const std::string& archivo);
void mostrarHistorialClinico(const std::vector<HistorialClinico>& historiales, const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos);

#endif