#ifndef CITAS_HPP
#define CITAS_HPP

#include "main.hpp"

class Paciente;
class Medico;

class Cita {
public:
    // Constructor modificado para incluir idPaciente y idMedico
    Cita(int id, int idPaciente, int idMedico,
         const std::string& fecha, const std::string& descripcion, const std::string& urgencia,
         const std::string& estado);

    // Métodos para mostrar información de la cita
    void mostrarInfo(const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos) const;

    // Getters
    int getId() const;
    int getIdPaciente() const;    // Getter para idPaciente
    int getIdMedico() const;      // Getter para idMedico
    std::string getNombrePaciente() const;
    std::string getNombreMedico() const;
    std::string getFecha() const;
    std::string getDescripcion() const;
    std::string getUrgencia() const;  // Getter para urgencia
    std::string getEstado() const;    // Getter para estado

    // Setters
    void setFecha(const std::string& nuevaFecha);
    void setDescripcion(const std::string& nuevaDescripcion);
    void setUrgencia(const std::string& nuevaUrgencia);  // Setter para urgencia
    void setEstado(const std::string& nuevoEstado);      // Setter para estado

private:
    int id;
    int idPaciente;   // id del paciente
    int idMedico;     // id del médico
    std::string nombrePaciente;
    std::string nombreMedico;
    std::string fecha;
    std::string descripcion;
    std::string urgencia;  // Atributo urgencia
    std::string estado;    // Nuevo atributo estado
};

// Funciones para cargar y guardar citas desde un archivo CSV
std::vector<Cita> cargarCitas(const std::string& filename);
void guardarCitas(const std::string& filename, const std::vector<Cita>& citas, 
                  const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos);
void mostrarCitas(const std::vector<Cita>& citas, const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos);
void eliminarCita(int id, std::vector<Cita>& citas);

#endif // CITAS_HPP
