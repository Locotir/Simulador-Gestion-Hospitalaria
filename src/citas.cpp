#include "main.hpp" // bcolors
#include "citas.hpp"
#include "pacientes.hpp"
#include "medicos.hpp"


Cita::Cita(int id, int idPaciente, int idMedico,
           const std::string& fecha, const std::string& descripcion, const std::string& urgencia,
           const std::string& estado)
    : id(id), idPaciente(idPaciente), idMedico(idMedico),
      fecha(fecha), descripcion(descripcion), urgencia(urgencia), estado(estado) {}

std::string Cita::getUrgencia() const { return urgencia; }
void Cita::setUrgencia(const std::string& nuevaUrgencia) { urgencia = nuevaUrgencia; }

std::string Cita::getEstado() const { return estado; }
void Cita::setEstado(const std::string& nuevoEstado) { estado = nuevoEstado; }

void Cita::mostrarInfo(const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos) const {
    // Buscar paciente por id
    auto itPaciente = std::find_if(pacientes.begin(), pacientes.end(), [this](const Paciente& paciente) {
        return paciente.getId() == this->idPaciente; // Ahora buscamos por ID
    });

    // Buscar medico por id
    auto itMedico = std::find_if(medicos.begin(), medicos.end(), [this](const Medico& medico) {
        return medico.getId() == this->idMedico; // Ahora buscamos por ID
    });

    // Mostrar información de la cita
    std::cout << bcolors::RED << "\nID: " << bcolors::RESET << id << std::endl;

    // Verificar si se encontró el paciente y mostrar su nombre
    if (itPaciente != pacientes.end()) {
        std::cout << bcolors::VIOLET << "Paciente: " << bcolors::RESET << itPaciente->getNombre() << std::endl;
    } else {
        std::cout << bcolors::VIOLET << "Paciente: No encontrado" << std::endl;
    }

    // Verificar si se encontró el médico y mostrar su nombre
    if (itMedico != medicos.end()) {
        std::cout << bcolors::VIOLET << "Médico: " << bcolors::RESET << itMedico->getNombre() << std::endl;
    } else {
        std::cout << bcolors::VIOLET << "Médico: No encontrado" << std::endl;
    }

    // Mostrar fecha, descripción, urgencia y estado de la cita
    std::cout << bcolors::BLUE << "Fecha: " << bcolors::RESET << fecha << std::endl
              << bcolors::BLUE << "Descripción: " << bcolors::RESET << descripcion << std::endl
              << bcolors::MAGENTA << "Urgencia: " << bcolors::RESET << urgencia << std::endl
              << bcolors::BLUEL << "Estado: " << bcolors::RESET << estado << std::endl;
}

int Cita::getId() const { return id; }
int Cita::getIdPaciente() const { return idPaciente; }
int Cita::getIdMedico() const { return idMedico; }
std::string Cita::getFecha() const { return fecha; }
std::string Cita::getDescripcion() const { return descripcion; }

void Cita::setFecha(const std::string& nuevaFecha) { fecha = nuevaFecha; }
void Cita::setDescripcion(const std::string& nuevaDescripcion) { descripcion = nuevaDescripcion; }

// Función para cargar las citas desde el archivo CSV
std::vector<Cita> cargarCitas(const std::string& filename) {
    std::vector<Cita> citas;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error al abrir el archivo: " + filename);
    }

    std::string linea;
    std::getline(file, linea);  // Saltar la primera línea (encabezado)

    while (std::getline(file, linea)) {
        std::stringstream ss(linea);
        std::string token;

        int id, idPaciente, idMedico;
        std::string fecha, descripcion, urgencia, estado;

        try {
            std::getline(ss, token, ','); // Leer ID
            id = std::stoi(token);

            std::getline(ss, token, ','); // Leer ID_Paciente
            idPaciente = std::stoi(token);

            std::getline(ss, token, ','); // Leer ID_Medico
            idMedico = std::stoi(token);

            std::getline(ss, fecha, ',');         // Leer fecha
            std::getline(ss, descripcion, ',');   // Leer descripcion
            std::getline(ss, urgencia, ',');      // Leer urgencia
            std::getline(ss, estado);             // Leer estado

            // Agregar la cita al vector
            citas.emplace_back(id, idPaciente, idMedico, fecha, descripcion, urgencia, estado);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error al procesar una línea: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Error de rango: " << e.what() << std::endl;
        }
    }

    return citas;
}

void mostrarCitas(const std::vector<Cita>& citas, const std::vector<Paciente>& pacientes, const std::vector<Medico>& medicos) {
    if (citas.empty()) {
        std::cout << bcolors::YELLOW << "No hay citas disponibles." << bcolors::RESET << std::endl;
        return;
    }

    for (const auto& cita : citas) {
        cita.mostrarInfo(pacientes, medicos); 
        std::cout << std::endl; 
    }
}

void guardarCitas(const std::string& filename, const std::vector<Cita>& citas) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error al abrir el archivo para guardar citas.");
    }

    file << "ID,ID_Paciente,ID_Medico,fecha,descripcion,urgencia,estado\n";

    for (const auto& cita : citas) {
        file << cita.getId() << ","
             << cita.getIdPaciente() << ","
             << cita.getIdMedico() << ","
             << cita.getFecha() << ","
             << cita.getDescripcion() << ","
             << cita.getUrgencia() << ","
             << cita.getEstado() << "\n";
    }
}

void eliminarCita(int id, std::vector<Cita>& citas) {
    auto it = std::remove_if(citas.begin(), citas.end(), [id](const Cita& c) {
        return c.getId() == id;
    });
    citas.erase(it, citas.end());
}