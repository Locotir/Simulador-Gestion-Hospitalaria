#ifndef REPORTES_HPP
#define REPORTES_HPP

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

std::string convertirFecha(const std::string& fecha);

void listarPacientesPorFecha(const std::vector<HistorialClinico>& historiales, 
                             const std::vector<Paciente>& pacientes, 
                             const std::string& fechaInicio, 
                             const std::string& fechaFin);

void citasPendientesPorMedicoOEspecialidad(const std::vector<Cita>& citas, 
                                           const std::vector<Medico>& medicos, 
                                           const std::vector<Paciente>& pacientes,  // Cambio aquí
                                           const std::string& filtro);


void reportePacientesCronicos(const std::vector<HistorialClinico>& historiales, const std::vector<Paciente>& pacientes);


#endif