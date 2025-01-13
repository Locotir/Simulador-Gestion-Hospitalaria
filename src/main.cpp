// --------------- Encabezados ---------------
#include "main.hpp" // Bcolors + todas las librerías .h
#include "pacientes.hpp"
#include "medicos.hpp"
#include "citas.hpp"
#include "reportes.hpp"


// Directorio de las bases de datos CSV
namespace fs = std::filesystem;
const std::string db_pacientes = "db/pacientes.csv";
const std::string db_medicos = "db/medicos.csv";
const std::string db_citas = "db/citas.csv";
const std::string db_historialClinico = "db/historial-clinico.csv";

// --------------- Bcolors---------------
const std::string bcolors::PURPLE = "\033[95m";
const std::string bcolors::BLUE = "\033[94m";
const std::string bcolors::BLUEL = "\033[96m";
const std::string bcolors::GREEN = "\033[92m";
const std::string bcolors::YELLOW = "\033[93m";
const std::string bcolors::RED = "\033[91m";
const std::string bcolors::UNDERLINE = "\033[4m";
const std::string bcolors::WHITE = "\033[37m";
const std::string bcolors::ORANGE = "\033[38;5;208m";
const std::string bcolors::VIOLET = "\033[38;5;135m";
const std::string bcolors::MAGENTA = "\033[35m";
const std::string bcolors::BLACK = "\033[30m";
const std::string bcolors::RESET = "\033[0m";

// =========================================================================================================================================================================================
// =================================================================================== P A C I E N T E S ===================================================================================
// =========================================================================================================================================================================================
void gestionarPacientes(int operacion, int id, int edad, std::string nombre, int nuevaDisponibilidad, int aModificar, std::string modificado, std::string fechaIngreso) {
    try {
    std::vector<Paciente> pacientes = cargarPacientes(db_pacientes); // Cargar los pacientes
    std::vector<Medico> medicos = cargarMedicos(db_medicos);         // Cargar los médicos

    // ------------------------------------------ Buscar ID Paciente ------------------------------------------
    // Si no se tiene ID, pero se tiene nombre y edad, o si no se tiene ID y nombre/edad, pedimos esos datos
    if (id == -1 && operacion != 3) { // en la operación 3 queremos hacer una busqueda aislada
        // Si no se tienen nombre y edad, pedimos los datos al usuario
        if (nombre.empty() || edad == -1) {
            std::cout << bcolors::YELLOW << "\nIngrese la edad del paciente: " << bcolors::GREEN;
            std::cin >> edad;
            std::cin.ignore();  // Limpiar el buffer de entrada
            std::cout << bcolors::YELLOW << "\nIngrese el nombre del paciente: " << bcolors::GREEN;
            std::getline(std::cin, nombre);
        }

        // Buscar paciente por nombre y edad
        bool encontrado = false;
        for (const auto& paciente : pacientes) {
            if (paciente.getEdad() == edad && paciente.getNombre() == nombre) {
                id = paciente.getId();  // Asignar el id encontrado
                paciente.mostrarInfo();  // Mostrar la información del paciente
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            std::cout << "No se encontró ningún paciente con los datos proporcionados (edad: " << edad << ", nombre: " << nombre << ").\n";
            return;  // Si no se encuentra, terminamos la función
        }
    }
    // ---------------------------------------------------------------------------------------------------------

    // A partir de aquí, ya tenemos el id y no necesitamos pedirlo nuevamente
    switch (operacion) {
        case 1: {  // ||||||||||||||||||||||||||||||||||||||||||| Alta/Baja |||||||||||||||||||||||||||||||||||||||||||
            try {
                auto it = std::find_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) {
                    return p.getId() == id;
                });

                if (it != pacientes.end()) {
                    // Si se especifico nuevaDisponibilidad, se usa; de lo contrario, pedimos la entrada
                    if (nuevaDisponibilidad == -1) {
                        std::cout << "\n¿Desea dar de alta (1) o de baja (0) al paciente? ";
                        std::cin >> nuevaDisponibilidad;
                    }

                    // Validar si ya está en el estado solicitado
                    if (it->getDisponibilidad() == nuevaDisponibilidad) {
                        std::cout << it->getNombre() 
                                << " ya está " << (nuevaDisponibilidad == 1 ? "de alta" : "de baja") 
                                << ".\n";
                    } else {
                        it->setDisponibilidad(nuevaDisponibilidad);  // Actualizar disponibilidad
                        std::cout << "\n[" << bcolors::GREEN << "+" << bcolors::WHITE << "] " << it->getNombre() 
                                << " ha sido dado " << (nuevaDisponibilidad == 1 ? "de alta" : "de baja") 
                                << " correctamente.\n";

                        // Solo guardar los cambios una vez
                        guardarPacientes(db_pacientes, pacientes);
                    }
                } else {
                    throw std::runtime_error("Paciente no encontrado para realizar la operación.");
                }
            } catch (const std::runtime_error& e) {
                std::cerr << e.what() << std::endl;
            }
            break;
        }
        case 2: {  // ||||||||||||||||||||||||||||||||||||||||||| Modificar Datos |||||||||||||||||||||||||||||||||||||||||||
            int disponibilidad;
            auto it = std::find_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) {
                return p.getId() == id;
            });

            if (it != pacientes.end()) {
                disponibilidad = it->getDisponibilidad();  // Copia el valor de disponibilidad
            }

            if (aModificar == -1) {
                // Modo menú: Pedimos al usuario qué desea modificar
                std::cout << bcolors::YELLOW << "\n¿Qué desea modificar?\n";
                std::cout << "  1: Nombre\n";
                std::cout << "  2: Edad\n";
                std::cout << "  3: Disponibilidad\n";
                std::cout << "Operación ➟ " << bcolors::WHITE;
                std::cin >> aModificar;
                std::cin.ignore();  // Limpiar el buffer de entrada
            }
    
            if (aModificar == 1) {
                // Modificar Nombre
                if (modificado.empty()) {
                    std::cout << bcolors::YELLOW << "Nombre actual: " << bcolors::WHITE << nombre << "\n";
                    std::cout << bcolors::ORANGE << "Ingrese el nuevo nombre: " << bcolors::WHITE;
                    std::getline(std::cin, modificado);  // Nuevo nombre
                }
                modificarPaciente(id, pacientes, modificado, edad, disponibilidad);
            }else if (aModificar == 2) {
                // Modificar Edad
                if (modificado.empty()) {
                    std::cout << bcolors::YELLOW << "Edad actual: " << bcolors::WHITE << edad << "\n";
                    std::cout << bcolors::ORANGE << "Ingrese la nueva edad: " << bcolors::WHITE;
                    std::getline(std::cin, modificado);  // Leer la nueva edad como texto
                }
                // Convertir el texto a int
                int nuevaEdad;
                try {
                    nuevaEdad = std::stoi(modificado);  // Convertir la edad de texto a int
                } catch (const std::invalid_argument& e) {
                    std::cout << bcolors::RED << "La edad ingresada no es válida. Debe ser un número.\n";
                    return;  // Salir de la función si la conversión falla
                } catch (const std::out_of_range& e) {
                    std::cout << bcolors::RED << "El número ingresado está fuera del rango válido.\n";
                    return;  // Salir de la función si el número está fuera de rango
                }

                // Ahora pasamos la edad convertida a la función de modificar
                modificarPaciente(id, pacientes, nombre, nuevaEdad, disponibilidad);

            }else if (aModificar == 3) {
                // Modificar Disponibilidad
                if (modificado.empty()) {
                    std::cout << bcolors::YELLOW << "Disponibilidad actual: " << bcolors::WHITE << (disponibilidad == 1 ? "Alta" : "Baja") << "\n";
                    std::cout << bcolors::ORANGE << "Ingrese la nueva disponibilidad (1 para alta, 0 para baja): " << bcolors::WHITE;
                    std::cin >> modificado;
                    std::cin.ignore();  // Limpiar el buffer de entrada
                }

                // Validar la entrada antes de convertirla
                if (modificado != "0" && modificado != "1") {
                    std::cout << bcolors::RED << "Valor inválido. Solo se permite 1 (alta) o 0 (baja).\n";
                    return;
                }

                int nuevaDisponibilidad = std::stoi(modificado);  // Ahora es seguro convertir
                modificarPaciente(id, pacientes, nombre, edad, nuevaDisponibilidad);
            } else {
                std::cout << bcolors::RED << "Opción no válida.\n";
            }

            // Guardar cambios
            guardarPacientes(db_pacientes, pacientes);
            std::cout << bcolors::BLUEL << "\nNuevos Datos Modificados:↴";
            // Aquí mostramos la información actualizada del paciente
            auto itMod = std::find_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) {
                return p.getId() == id;
            });
            if (itMod != pacientes.end()) {
                itMod->mostrarInfo();  // Mostrar los datos del paciente modificado
            }
            break;
        }
        case 3: {  // ||||||||||||||||||||||||||||||||||||||||||| Realizar Búsqueda ||||||||||||||||||||||||||||||||||||||||||| 
            try {
                // Comprobar si todos los parámetros de búsqueda son vacíos o no válidos, modo menu/CLI
                if (fechaIngreso.empty() && id == -1 && nombre.empty() && edad == -1) {
                    int opcion;
                    std::cout << bcolors::YELLOW << "\nSeleccione el criterio por el que desea buscar:\n";
                    std::cout << "1. Buscar por fecha de ingreso\n";
                    std::cout << "2. Buscar por ID\n";
                    std::cout << "3. Buscar por nombre\n";
                    std::cout << "4. Buscar por edad\n";
                    std::cout << "\n\n ▶ Opción ➟ " << bcolors::RESET;
                    std::cin >> opcion;

                    // Configurar los parámetros según la selección del usuario
                    switch (opcion) {
                        case 1:
                            std::cout << "Ingrese la fecha de ingreso: ";
                            std::cin.ignore();
                            std::getline(std::cin, fechaIngreso);
                            break;
                        case 2:
                            std::cout << "Ingrese el ID: ";
                            std::cin >> id;
                            break;
                        case 3:
                            std::cout << "Ingrese el nombre: ";
                            std::cin.ignore();
                            std::getline(std::cin, nombre);
                            break;
                        case 4:
                            std::cout << "Ingrese la edad: ";
                            std::cin >> edad;
                            break;
                        default:
                            std::cout << bcolors::RED << "Opción no válida. Cancelando búsqueda.\n" << bcolors::RESET;
                            break;
                    }
                }                
                // Verificar si alguno de los parámetros de búsqueda es válido
                if (!fechaIngreso.empty()) {
                    // Buscar por fecha de ingreso
                    bool encontrado = false;
                    for (const auto& paciente : pacientes) {
                        if (paciente.getFechaIngreso() == fechaIngreso) {
                            paciente.mostrarInfo();
                            encontrado = true;
                        }
                    }
                    if (!encontrado) {
                        std::cout << bcolors::RED << "No se encontró un paciente con la fecha de ingreso proporcionada.\n" << bcolors::RESET;
                    }
                } else if (id != -1) {
                    // Buscar por ID
                    auto it = std::find_if(pacientes.begin(), pacientes.end(), [id](const Paciente& p) {
                        return p.getId() == id;
                    });
                    if (it != pacientes.end()) {
                        it->mostrarInfo();
                    } else {
                        std::cout << bcolors::RED << "No se encontró un paciente con el ID proporcionado.\n" << bcolors::RESET;
                    }
                } else if (!nombre.empty()) {
                    // Buscar por nombre
                    bool encontrado = false;
                    for (const auto& paciente : pacientes) {
                        if (paciente.getNombre() == nombre) {
                            paciente.mostrarInfo();
                            encontrado = true;
                        }
                    }
                    if (!encontrado) {
                        std::cout << bcolors::RED << "No se encontró un paciente con el nombre proporcionado.\n" << bcolors::RESET;
                    }
                } else if (edad != -1) {
                    // Buscar por edad
                    bool encontrado = false;
                    for (const auto& paciente : pacientes) {
                        if (paciente.getEdad() == edad) {
                            paciente.mostrarInfo();
                            encontrado = true;
                        }
                    }
                    if (!encontrado) {
                        std::cout << bcolors::RED << "No se encontró un paciente con la edad proporcionada.\n" << bcolors::RESET;
                    }
                } else {
                    std::cout << bcolors::YELLOW << "No se proporcionaron criterios de búsqueda válidos.\n" << bcolors::RESET;
                }
            } catch (const std::exception& e) {
                std::cerr << bcolors::RED << "Error durante la búsqueda: " << e.what() << "\n" << bcolors::RESET;
            }
            break;
        }

        case 4: {  // ||||||||||||||||||||||||||||||||||||||||||| Historial Clínico||||||||||||||||||||||||||||||||||||||||||| 
            cargarHistorialClinico(db_historialClinico, pacientes, medicos, id); // Pasamos el id del paciente encontrado
            break;
        }
        default:
            std::cout << "Opción no válida." << std::endl;
            break;
    }
    } catch (const std::exception& e) {
        std::cerr << bcolors::RED << "Se produjo un error: " << e.what() << bcolors::WHITE << std::endl;
    }
}
// -.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.

// =========================================================================================================================================================================================
// ===================================================================================== M E D I C O S =====================================================================================
// =========================================================================================================================================================================================
void gestionarMedicos(int operacion, int id, std::string nombre, int nuevaDisponibilidad, std::string especialidad, std::string nuevaEspecialidad) {
    try {
        std::vector<Medico> medicos = cargarMedicos(db_medicos); // Cargar los médicos desde el archivo
        
        // --------------------------------------- Buscar ID Médico ---------------------------------------
        // Si no se tiene ID, se pide al usuario ingresar datos para buscar al médico
        if (id == -1 && operacion != 3) { // En la operación 3 no buscamos médico específico
            if (especialidad.empty() || nombre.empty()) {
                std::cout << bcolors::YELLOW << "\nIngrese el nombre completo del médico: " << bcolors::GREEN;
                std::cin.ignore();
                std::getline(std::cin, nombre);

                std::cout << bcolors::YELLOW << "\nIngrese la especialidad del médico: " << bcolors::GREEN;
                std::getline(std::cin, especialidad);
            }

            // Buscar médico por nombre completo y especialidad
            bool encontrado = false;
            for (const auto& medico : medicos) {
                if (medico.getNombre() == nombre && medico.getEspecialidad() == especialidad) {
                    id = medico.getId(); // Asignar el ID encontrado
                    medico.mostrarInfo(); // Mostrar la información del médico
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado) {
                std::cout << "No se encontró ningún médico con los datos proporcionados: " 
                          << nombre << " (especialidad: " << especialidad << ").\n";
                return; // Terminar la función si no se encuentra
            }
        }
        // ------------------------------------------------------------------------------------------------------

        // A partir de aquí, ya tenemos el ID
        switch (operacion) {
            case 1: { // ||||||||||||||||||||||||||||||||||||||||||| Alta/Baja |||||||||||||||||||||||||||||||||||||||||||
                auto it = std::find_if(medicos.begin(), medicos.end(), [id](const Medico& m) {
                    return m.getId() == id;
                });

                if (it != medicos.end()) {
                    if (nuevaDisponibilidad == -1) {
                        std::cout << "\n¿Desea activar (1) o desactivar (0) la disponibilidad del médico? ";
                        std::cin >> nuevaDisponibilidad;
                    }

                    if (it->getDisponibilidad() == nuevaDisponibilidad) {
                        std::cout << it->getNombre() 
                                  << " ya está " << (nuevaDisponibilidad == 1 ? "activo" : "inactivo") 
                                  << ".\n";
                    } else {
                        it->setDisponibilidad(nuevaDisponibilidad); // Actualizar disponibilidad
                        std::cout << "\n[" << bcolors::GREEN << "+" << bcolors::WHITE << "] " << it->getNombre() 
                                  << " ha sido " << (nuevaDisponibilidad == 1 ? "activado" : "desactivado") 
                                  << " correctamente.\n";

                        guardarMedicos(db_medicos, medicos);
                    }
                } else {
                    throw std::runtime_error("Médico no encontrado para realizar la operación.");
                }
                break;
            }

            case 2: { // ||||||||||||||||||||||||||||||||||||||||||| Asignar Especialidad |||||||||||||||||||||||||||||||||||||||||||
                auto it = std::find_if(medicos.begin(), medicos.end(), [id](const Medico& m) {
                    return m.getId() == id;
                });

                if (it != medicos.end()) {
                    if (nuevaEspecialidad.empty()) {
                        std::cout << "\nIngrese la nueva especialidad: ";
                        std::getline(std::cin, nuevaEspecialidad);
                    }

                    it->setEspecialidad(nuevaEspecialidad); // Actualizar la especialidad
                    std::cout << "\n[" << bcolors::GREEN << "+" << bcolors::WHITE << "] " << it->getNombre() 
                                << " ahora es especialista en " 
                                << nuevaEspecialidad << ".\n";

                    guardarMedicos(db_medicos, medicos);

                } else {
                    throw std::runtime_error("Médico no encontrado para realizar la operación.");
                }
                break;
            }

            case 3: { // ||||||||||||||||||||||||||||||||||||||||||| Listar por Especialidad/Disponibilidad |||||||||||||||||||||||||||||||||||||||||||
                if (especialidad.empty()) {
                    std::cout << "\nIngrese la especialidad o disponibilidad para listar: ";
                    std::cin.ignore();
                    std::getline(std::cin, especialidad);
                }

                for (const auto& medico : medicos) {
                    if (medico.getEspecialidad() == especialidad || 
                        (especialidad == "1" && medico.getDisponibilidad() == 1) ||
                        (especialidad == "0" && medico.getDisponibilidad() == 0)) {
                        medico.mostrarInfo();
                    }
                }
                break;
            }
            // |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
            default:
                std::cout << "Operación no válida.\n";
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}
// -.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.

// =========================================================================================================================================================================================
// ======================================================================================= C I T A S =======================================================================================
// =========================================================================================================================================================================================
void gestionarCitas(int operacion, int idCita, std::string fecha, std::string nuevaFecha, std::string descripcion, std::string nombrePaciente, std::string nombreMedico, int aModificar, std::string nuevoEstado, std::string urgencia, int criterio) {
    try {
        std::vector<Cita> citas = cargarCitas(db_citas);  // Cargar las citas desde el archivo
        std::vector<Medico> medicos = cargarMedicos(db_medicos); // Cargar los médicos desde el archivo
        std::vector<Paciente> pacientes = cargarPacientes(db_pacientes); // Cargar los pacientes
        std::vector<HistorialClinico> historialClinico = cargarHistorialClinico(db_historialClinico); // Cargar el historial clínico


        switch (operacion) {
            case 1: { // ||||||||||||||||||||||||||||||||||||||||||| Asignar Cita |||||||||||||||||||||||||||||||||||||||||||

                // ::::::::::::::::::::::::::::::::::::::::::::::::: Paciente :::::::::::::::::::::::::::::::::::::::::::::::::
                if (nombrePaciente.empty()) {
                    std::cin.ignore(); // Evitar salto de línea
                    std::cout << bcolors::YELLOW << "\nIngrese el nombre del paciente: " << bcolors::RESET;
                    std::getline(std::cin, nombrePaciente);
                }

                // Buscar paciente por nombre
                auto itPaciente = std::find_if(pacientes.begin(), pacientes.end(),
                                            [&nombrePaciente](const Paciente& p) {
                                                return p.getNombre() == nombrePaciente;
                                            });

                // ---------------------------------- Registrar Paciente si no Existe ----------------------------------
                if (itPaciente == pacientes.end()) {
                    // Si no se encuentra el paciente, solicitamos los datos
                    std::cout << bcolors::RED << "\nEl paciente no está disponible o no existe, procediendo al registro.\n";
                    std::cout << bcolors::YELLOW << "\nPor favor ingrese los datos del paciente:\n";

                    std::string edadStr, fechaIngresoStr;
                    int edad;

                    std::cout << bcolors::GREEN << "\n--== Nuevo Registro de Paciente ==--\n";
                    std::cout << bcolors::ORANGE << "Edad: " << bcolors::WHITE;
                    std::getline(std::cin, edadStr);
                    try {
                        edad = std::stoi(edadStr);
                    } catch (const std::invalid_argument& e) {
                        std::cout << bcolors::RED << "La edad ingresada no es válida. Debe ser un número.\n";
                        break;
                    }

                    std::cout << bcolors::ORANGE << "Fecha de ingreso (dd-mm-yyyy): " << bcolors::WHITE;
                    std::getline(std::cin, fechaIngresoStr);

                    // Asignar ID y disponibilidad (nuevo paciente será no disponible)
                    int nuevoId = pacientes.empty() ? 0 : pacientes.back().getId() + 1;
                    int disponibilidad = 1;  // Al registrar, se establece como disponible

                    // Crear el nuevo paciente
                    Paciente nuevoPaciente(nuevoId, nombrePaciente, edad, fechaIngresoStr, disponibilidad);
                    pacientes.push_back(nuevoPaciente);

                    // Guardar al paciente en el archivo CSV
                    guardarPacientes(db_pacientes, pacientes); // Implementar esta función para guardar en CSV
                    std::cout << "\n[" << bcolors::GREEN << "+" << bcolors::WHITE << "] " << bcolors::GREEN << " Paciente registrado exitosamente.\n";

                    itPaciente = std::find_if(pacientes.begin(), pacientes.end(),
                                            [&nombrePaciente](const Paciente& p) {
                                                return p.getNombre() == nombrePaciente;
                                            });
                }
                // -------------------------------------------------------------------------------------------------------

                // Continuar con la asignación de la cita después de asegurar que el paciente existe
                int idPaciente = itPaciente->getId();
                std::cout << bcolors::GREEN << "\nPaciente " << nombrePaciente << " encontrado.\n";
                
                // ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

                // ::::::::::::::::::::::::::::::::::::::::::::::::: Médico :::::::::::::::::::::::::::::::::::::::::::::::::
                if (nombreMedico.empty()) {
                    std::cout << bcolors::YELLOW << "\nIngrese el nombre del médico: " << bcolors::RESET;
                    std::getline(std::cin, nombreMedico);
                }
                // Buscar médico por nombre
                auto itMedico = std::find_if(medicos.begin(), medicos.end(),
                                            [&nombreMedico](const Medico& m) {
                                                return m.getNombre() == nombreMedico;
                                            });
                // ---------------------------------- Registrar Médico si no Existe ----------------------------------
                if (itMedico == medicos.end()) {
                    // Si no se encuentra el médico, solicitamos los datos
                    std::cout << bcolors::RED << "\nEl médico no existe, procediendo al registro.\n";
                    std::cout << bcolors::YELLOW << "\nPor favor ingrese los datos del médico:\n";

                    std::string especialidad;

                    std::cout << bcolors::GREEN << "\n--== Nuevo Registro de Médico ==--\n";
                    std::cout << bcolors::ORANGE << "Especialidad: " << bcolors::WHITE;
                    std::getline(std::cin, especialidad);

                    // Asignar ID y disponibilidad (nuevo médico será disponible)
                    int nuevoIdMedico = medicos.empty() ? 0 : medicos.back().getId() + 1; // Último id +1
                    int disponibilidadMedico = 1;  // Al registrar, se establece como disponible

                    // Crear el nuevo médico
                    Medico nuevoMedico(nuevoIdMedico, nombreMedico, especialidad, disponibilidadMedico);
                    medicos.push_back(nuevoMedico);

                    // Guardar al médico en el archivo CSV
                    guardarMedicos(db_medicos, medicos); // Implementar esta función para guardar en CSV
                    std::cout << "\n[" << bcolors::GREEN << "+" << bcolors::WHITE << "] " << bcolors::GREEN << " Médico registrado exitosamente.\n";

                    itMedico = std::find_if(medicos.begin(), medicos.end(),
                                            [&nombreMedico](const Medico& m) {
                                                return m.getNombre() == nombreMedico;
                                            });
                }
                // -------------------------------------------------------------------------------------------------------

                // Continuar con la asignación de la cita después de asegurar que el médico existe
                int idMedico = itMedico->getId();
                if (itMedico->getDisponibilidad() == 0) {
                        std::cout << bcolors::RED << "\nMédico encontrado pero no disponible.\n";
                        exit(1); // Salir del programa
                    }
                std::cout << bcolors::GREEN << "\nMédico " << nombreMedico << " encontrado & disponibilidad verificada.\n";

                // :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

                // ...................... Solicitar datos si no se ejecuta por argumentos CLI ......................
                if (nuevaFecha.empty()) {
                    std::cout << bcolors::YELLOW << "\nIngrese la fecha de la cita (dd-mm-yyyy): " << bcolors::RESET;
                    std::getline(std::cin, nuevaFecha);
                }
            
                if (nuevaFecha.empty() || nuevaFecha.length() != 10 || nuevaFecha[2] != '-' || nuevaFecha[5] != '-') {
                    std::cout << bcolors::RED << "\nFecha inválida. Asegúrese de seguir el formato (dd-mm-yyyy).\n";
                    break;
                }

                if (descripcion.empty()) {
                    std::cout << bcolors::YELLOW << "\nIngrese la descripción de la cita: " << bcolors::RESET;
                    std::getline(std::cin, descripcion);
                }
                
                if (urgencia.empty()){
                    std::cout << bcolors::YELLOW << "\nIngrese el nivel de urgencia (alta, media, baja): " << bcolors::RESET;
                    std::getline(std::cin, urgencia);
                }
                
                if (nuevoEstado.empty()) {
                    std::cout << bcolors::YELLOW << "\nIngrese el estado de la cita (Pendiente, Completada, Cancelada): " << bcolors::RESET;
                    std::getline(std::cin, nuevoEstado);
                }
                

                // +++++++++++++++++++++++++++++++++++ Crear y guardar +++++++++++++++++++++++++++++++++++
                int nuevoIdCita = citas.empty() ? 0 : citas.back().getId() + 1;
                Cita nuevaCita(nuevoIdCita, idPaciente, idMedico, nuevaFecha, descripcion, urgencia, nuevoEstado);
                citas.push_back(nuevaCita);

                guardarCitas(db_citas, citas); // Guardar las citas actualizadas
                guardarPacientes(db_pacientes, pacientes); // Guardar los cambios de disponibilidad en los pacientes
                guardarMedicos(db_medicos, medicos); // Guardar los cambios de disponibilidad en los médicos

                std::cout << bcolors::GREEN << "\nCita asignada exitosamente.\n";
                break;
            }
            // |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

            case 2: { // ||||||||||||||||||||||||||||||||||||||||||| Ordenar Citas por Fecha o Urgencia |||||||||||||||||||||||||||||||||||||||||||

                if (criterio == -1) { // Si no se ejecuta por CLI
                    std::cin.ignore(); // Evitar salto de línea
                    std::cout << bcolors::YELLOW << "\nSeleccione el criterio de ordenación: ";
                    std::cout << "\n1. Ordenar por Fecha";
                    std::cout << "\n2. Ordenar por Urgencia";
                    std::cout << "\n ▶ ";
                    std::cin >> criterio;
                }
                
                // Por fecha
                if (criterio == 1) {
                    std::sort(citas.begin(), citas.end(), [](const Cita& a, const Cita& b) {
                        return a.getFecha() < b.getFecha();  // Ordenar por fecha
                    });
                    std::cout << bcolors::GREEN << "\nCitas ordenadas por fecha.\n";

                // Por Urgencia
                } else if (criterio == 2) {
                    std::cout << "Ordenando por urgencia...\n"; // Mensaje de depuración
                    std::sort(citas.begin(), citas.end(), [](const Cita& a, const Cita& b) {
                        // Mapa de urgencia con las prioridades correctas
                        static const std::unordered_map<std::string, int> urgenciaMap = {
                            {"alta", 1},    // Alta tiene prioridad 1 (más urgente)
                            {"media", 2},   // Media tiene prioridad 2
                            {"baja", 3}     // Baja tiene prioridad 3 (menos urgente)
                        };

                        // Función para convertir las cadenas a minúsculas
                        auto toLower = [](std::string str) {
                            std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
                                return std::tolower(c);
                            });
                            return str;
                        };

                        // Obtener el valor de urgencia de cada cita, normalizado a minúsculas
                        std::string urgenciaA = toLower(a.getUrgencia());
                        std::string urgenciaB = toLower(b.getUrgencia());

                        int urgenciaA_val = urgenciaMap.count(urgenciaA) ? urgenciaMap.at(urgenciaA) : 0;
                        int urgenciaB_val = urgenciaMap.count(urgenciaB) ? urgenciaMap.at(urgenciaB) : 0;


                        return urgenciaA_val < urgenciaB_val; // Orden ascendente: Alta primero
                    });
                    std::cout << bcolors::GREEN << "\nCitas ordenadas por urgencia.\n";
                }
                mostrarCitas(citas, pacientes, medicos); // Mostrar citas ordenadas
                break;
            }
            // |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

            case 3: { // ||||||||||||||||||||||||||||||||||||||||||| Registro Clínico (listar todos) |||||||||||||||||||||||||||||||||||||||||||
                if (aModificar == -1) {
                    std::cout << bcolors::YELLOW << "\n1. Citas pendientes por médico o especialidad\n";
                    std::cout << "2. Listado de pacientes atendidos en un rango de fechas\n";
                    std::cout << "3. Reporte de pacientes con enfermedades crónicas\n";
                    std::cout << "\n ▶ " << bcolors::RESET;
                    std::cin >> aModificar;
                }

                switch (aModificar) {
                    case 1: {
                        if (nombreMedico.empty()) {
                            std::cin.ignore();
                            std::cout << bcolors::YELLOW << "Ingrese el nombre del médico o especialidad (o deje vacío para todos): " << bcolors::RESET;
                            std::getline(std::cin, nombreMedico);
                        }

                        citasPendientesPorMedicoOEspecialidad(citas, medicos, pacientes, nombreMedico);
                        break;
                    }
                    case 2: {
                        if (fecha.empty() || nuevaFecha.empty()) {
                            std::cout << bcolors::YELLOW << "Ingrese la fecha de inicio (dd-mm-yyyy): " << bcolors::RESET;
                            std::cin >> fecha;
                            std::cout << bcolors::YELLOW << "Ingrese la fecha de fin (dd-mm-yyyy): " << bcolors::RESET;
                            std::cin >> nuevaFecha;
                        }

                        listarPacientesPorFecha(historialClinico, pacientes, fecha, nuevaFecha);
                        break;
                    }
                    case 3:
                        reportePacientesCronicos(historialClinico, pacientes);
                        break;

                    default:
                        std::cout << "Opción no válida.\n";
                }
                break;
            }

            case 4: { // ||||||||||||||||||||||||||||||||||||||||||| Cancelar, Modificar o Confirmar Cita |||||||||||||||||||||||||||||||||||||||||||

                if (idCita == -1) { // Si no se pasa el ID de la cita, solicitamos el nombre del paciente y la fecha
                     if (nombrePaciente.empty() && fecha.empty()) {
                        std::cin.ignore(); // Evitar salto de línea
                        std::cout << bcolors::YELLOW << "\nIngrese el nombre del paciente: " << bcolors::RESET;
                        std::getline(std::cin, nombrePaciente);

                        std::cout << bcolors::YELLOW << "\nIngrese la fecha de la cita (dd-mm-yyyy): " << bcolors::RESET;
                        std::getline(std::cin, fecha);
                     }
                    
                    auto itPaciente = std::find_if(pacientes.begin(), pacientes.end(),
                                            [&nombrePaciente](const Paciente& p) {
                                                return p.getNombre() == nombrePaciente;
                                            });
                    int idPaciente = itPaciente->getId();

                    if (idPaciente == -1) {
                        std::cout << bcolors::RED << "\nNo se encontró un paciente con ese nombre.\n";
                        break;
                    }

                    // Buscar la cita con el ID del paciente y la fecha proporcionada
                    auto it = std::find_if(citas.begin(), citas.end(), [&idPaciente, &fecha](const Cita& cita) {
                        // Verificamos si el ID del paciente y la fecha coinciden
                        return cita.getFecha() == fecha && cita.getIdPaciente() == idPaciente;

                    });

                    if (it != citas.end()) {
                        idCita = it->getId(); // Si se encuentra, asignamos el ID de la cita
                    } else {
                        std::cout << bcolors::RED << "\nNo se encontró ninguna cita para el paciente con esa fecha.\n";
                        break;
                    }
                }

                // Ahora que tenemos el idCita, procedemos como en el código original
                auto it = std::find_if(citas.begin(), citas.end(), [idCita](const Cita& cita) {
                    return cita.getId() == idCita;
                });

                if (it != citas.end()) {

                    if (aModificar == -1) { // Si no se ejecuta por CLI
                        std::cout << bcolors::YELLOW << "\n¿Qué desea realizar? ";
                        std::cout << "\n1. Cambiar el estado de la cita (Confirmada/Cancelada/Pendiente)";
                        std::cout << "\n2. Modificar la fecha";
                        std::cout << "\n3. Modificar la descripción de la cita";
                        std::cout << "\n4. Cambiar nivel de urgencia";
                        std::cout << "\n ▶ "  << bcolors::RESET;
                        std::cin >> aModificar;
                        std::cin.ignore(); // Limpiar el buffer de entrada

                    }

                    switch (aModificar) {
                        case 1: {
                        // ----------------------------------------------------- Cambiar estado de la cita -----------------------------------------------------
                        // Pedir el nuevo estado si no se pasa como parámetro
                        if (nuevoEstado.empty()) {
                            std::cout << bcolors::YELLOW << "\nIngrese el nuevo estado de la cita (Confirmada/Cancelada/Pendiente): " << bcolors::RESET;
                            std::getline(std::cin, nuevoEstado);
                        }

                        // Validar el estado ingresado
                        if (nuevoEstado == "Confirmada" || nuevoEstado == "Cancelada" || nuevoEstado == "Pendiente") {
                            it->setEstado(nuevoEstado);
                            guardarCitas(db_citas, citas);  // Guardar cambios
                            std::cout << bcolors::GREEN << "\nEstado de la cita actualizado exitosamente a " << nuevoEstado << ".\n";
                        } else {
                            std::cout << bcolors::RED << "\nEstado inválido. Debe ser Confirmada / Cancelada / Pendiente.\n" << bcolors::RESET;
                        }
                        break;
                        }

                        case 2: {
                            // ----------------------------------------------------- Cambiar la fecha -----------------------------------------------------
                            // Pedir la nueva fecha si no se pasa como parámetro
                            if (nuevaFecha.empty()) {
                                std::cout << bcolors::YELLOW << "\nIngrese la nueva fecha (dd-mm-yyyy): " << bcolors::RESET;
                                std::getline(std::cin, nuevaFecha);
                            }
                            // Actualizar la cita
                            it->setFecha(nuevaFecha);

                            guardarCitas(db_citas, citas);  // Guardar cambios
                            std::cout << bcolors::GREEN << "\nCita modificada exitosamente.\n";
                            break;
                            }
                        case 3: {
                            // ----------------------------------------------------- Cambiar la descripción -----------------------------------------------------
                            // Pedir la nueva descripción si no se pasa como parámetro
                            if (descripcion.empty()) {
                                std::cout << bcolors::YELLOW << "\nIngrese la nueva descripción: "<< bcolors::RESET;
                                std::getline(std::cin, descripcion);
                            }

                            it->setDescripcion(descripcion);
                            guardarCitas(db_citas, citas);  // Guardar cambios
                            std::cout << bcolors::GREEN << "\nCita modificada exitosamente.\n";
                            break;

                        }
                        case 4: {
                            // ----------------------------------------------------- Cambiar la urgencia -----------------------------------------------------
                            // Pedir el nuevo nivel de urgencia si no se pasa como parámetro
                            if (urgencia.empty()) {
                                std::cout << bcolors::YELLOW << "\nIngrese el nuevo nivel de urgencia: " << bcolors::RESET;
                                std::getline(std::cin, urgencia);
                            }

                            it->setUrgencia(urgencia);
                            guardarCitas(db_citas, citas);  // Guardar cambios
                            std::cout << bcolors::GREEN << "\nCita modificada exitosamente.\n";
                            break;
                        }
                        default: {
                        std::cout << bcolors::RED << "\nOpción no válida.\n" << bcolors::RESET;
                    }
                    }
                    

                } else {
                    std::cout << bcolors::RED << "\nCita no encontrada.\n";
                }
                break;
            }
            // |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

            default:
                std::cout << "Operación no válida.\n"; 
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}
// -.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.


// =========================================================================================================================================================================================
// ====================================================================================== B A C K U P ======================================================================================
// =========================================================================================================================================================================================
void realizarBackup() {
    // Directorio donde se encuentran los archivos CSV
    fs::path dir = "./db";  

    // Iterar sobre los archivos en el directorio
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            std::string originalPath = entry.path().string();
            std::string backupPath = originalPath + ".BACKUP";

            // Abrir archivo original
            std::ifstream src(originalPath, std::ios::binary);

            // Crear archivo de backup
            std::ofstream dst(backupPath, std::ios::binary);

            // Copiar contenido del archivo original al de backup
            dst << src.rdbuf();

            std::cout << "\n[" << bcolors::GREEN << "+" << bcolors::WHITE << "] " << "Backup creado para: " << originalPath << " -> " << backupPath << std::endl;
        }
    }
}
// -.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Manegar CTL + C Exit ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void signalHandler([[maybe_unused]] int signum) {
    std::cout << bcolors::WHITE << "\n\n[" << bcolors::RED << "!" << bcolors::WHITE << "] ""Saliendo...\n";
    exit(0); // Exit program
}
// -.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Mostrar Mensaje de Ayuda ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void showHelp() {
    std::cout << "\nUso: ./SGH [opciones]\n"
              << "Opciones:\n"
              << "  --gestionar     <pacientes,medicos,citas>       Area de enfoque.\n"
              << "      -id           <id>                            Identificador del individuo.\n"
              << "      -N            <'Nombre Apellido1 Apellido2'>  Nombre del paciente.\n"
              << "      -NM           <'Nombre Apellido1 Apellido2'>  Nombre del medico.\n"
              << "      -E            <edad>                          Edad del individuo.\n"
              << "      -X            <1:0/especialidad>              Especialidad o disponibilidad del médico.\n"
              << "      -NX           <especialidad>                  Nueva especialidad.\n"
              << "      -F            <dd-mm-yyyy>                    Fecha de Ingreso/Cita del individuo\n"
              << "      -NF           <dd-mm-yyyy>                    Nueva fecha de Ingreso/Cita del individuo\n"
              << "      -alta         <dar de alta>                  Dar de alta el individuo.\n"
              << "      -baja         <dar de baja>                  Dar de baja el individuo.\n"
              << "      --reporte    <F,NF|NM|cronico>               Lista todos los registros clínicos por filtros.\n"
              << "      -cronico      <aplicar filtro>               Aplica el filtro de crónicos para el registro.\n"
              << "      -modificar    <nombre,edad,disponibilidad>   Modificar un dato del individuo.\n"
              << "      -NC           <Nueva Cita>                   Crear nueva cita.\n"
              << "      -MC           <Modificar Cita>               Modificar cita existente.\n"
              << "      -confirmar    <Confirma cita>                Cambia el estado de la cita a Confirmada.\n"
              << "      -cancelar     <Cancela cita>                 Cambia el estado de la cita a Cancelada.\n"
              << "      -pendiente    <Cita pendiente>               Cambia el estado de la cita a Pendiente.\n"
              << "      -descripcion  <'descripción'>                Cambia la descripción de la cita.\n"
              << "      -urgencia     <alta/media/baja>              Nivel de urgencia de la cita.\n"
              << "      --listar       <urgencia/fecha>               Listar citas por fecha o nivel de urgencia.\n"
              << "      -especialidad <nueva especialidad>           Cambiar especialidad.\n"
              << "      -buscar       <+-N/-E/-F/-id>                Buscar individuo mediante un único parámetro.\n"
              << "      -historial    <++-N/-E/-id>                  Buscar historial del individuo mediante un único parámetro.\n"
              << "  --backup        <realizar backup>               Realizar una copia de seguridad de los datos.\n"
              << "  -h               Mostrar este mensaje\n"
              << "Ejemplos:\n"
              << "  ./SGH --gestionar pacientes -N 'Marcos Garcia Zorin' -E 19 -alta\n" 
              << "  ./SGH --gestionar pacientes -id 123 -modificar nombre 'Nuevo Nombre'\n"
              << "  ./SGH --gestionar pacientes -buscar -F 05-08-2015\n"
              << "  ./SGH --gestionar pacientes -historial -N 'Marcos Garcia Zorin' -E 19\n"
              << "  ./SGH --gestionar medicos -N 'Luis Torres Martín' -X Cardiología -alta\n"
              << "  ./SGH --gestionar medicos -N 'Luis Torres Martín' -X Cardiología -NX Pediatría -especialidad\n" 
              << "  ./SGH --gestionar medicos -N 'Luis Torres Martín' -X 1 -buscar\n"
              << "  ./SGH --gestionar citas -NC -N 'Marcos Garcia Zorin' -NM 'Luis Torres Martín' -NF '29-12-2024' -descripcion 'Revisión de Oído' -urgencia alta -confirmar\n"
              << "  ./SGH --gestionar citas --listar urgencia\n"
              << "  ./SGH --gestionar citas --reporte\n"
              << "  ./SGH --gestionar citas -MC -F '16-11-2024' -N 'Marcos Garcia Zorin' -NF '17-11-2024'\n"
              << "  ./SGH --gestionar citas -MC -F '16-11-2024' -N 'Marcos Garcia Zorin' -cancelar\n"
              << "  ./SGH --gestionar citas --reporte -F '15-11-2024' -NF '10-12-2024'\n"
              << "  ./SGH --gestionar citas --reporte -NM 'Luis Torres Martín'\n"
              << "  ./SGH --gestionar citas --reporte -NM 'Cardiología'\n"
              << "  ./SGH --gestionar citas --reporte -cronico\n"
              << "  ./SGH --backup\n\n"
              << bcolors::BLUEL << "[" << bcolors::PURPLE << "?" << bcolors::BLUEL << "]"
              << bcolors::GREEN << " Si se ejecuta sin argumentos, el modo interactivo empezará." << std::endl;
}
// -.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.--.

// =========================================================================================================================================================================================
// ======================================================================================== M A I N ========================================================================================
// =========================================================================================================================================================================================
int main(int argc, char *argv[]) { // Coger argumentos de ejecuccion
    std::signal(SIGINT, signalHandler); // Interceptar CTL + C
    setlocale(LC_ALL, "en_US.UTF-8");

    if (argc == 1) { // Modo interactivo :> sin argumentos
    int opcion;
    int operacion;
    std::cout << bcolors::YELLOW << R"(
       ______          __  _                __  __                 _ __        __           _      
      / ____/__  _____/ /_(_)___  ____     / / / /___  _________  (_) /_____ _/ /___ ______(_)___ _
     / / __/ _ \/ ___/ __/ / __ \/ __ \   / /_/ / __ \/ ___/ __ \/ / __/ __ `/ / __ `/ ___/ / __ `/
    / /_/ /  __(__  ) /_/ / /_/ / / / /  / __  / /_/ (__  ) /_/ / / /_/ /_/ / / /_/ / /  / / /_/ / 
    \____/\___/____/\__/_/\____/_/ /_/  /_/ /_/\____/____/ .___/_/\__/\__,_/_/\__,_/_/  /_/\__,_/  
                                                        /_/                                        
    )" << bcolors::RESET << std::endl;

    // Mostrar Opciones
    std::cout << bcolors::GREEN <<  "\n1. 🧑 Gestionar Pacientes" << bcolors::RESET << std::endl;
    std::cout << bcolors::BLUEL <<  "\n2. 👨‍⚕️Gestionar Médicos" << bcolors::RESET << std::endl;
    std::cout << bcolors::RED <<    "\n3. 🗎  Gestionar Citas Médicas" << bcolors::RESET << std::endl;
    std::cout << bcolors::PURPLE << "\n4. ⎗  Realizar Backup" << bcolors::RESET << std::endl;
    std::cout << bcolors::YELLOW << "\n\n ▶ Opción ➟ " << bcolors::WHITE;
    std::cin >> opcion;

    switch (opcion) {
        case 1:
            std::cout << bcolors::GREEN << "\n1. ⭥  Manejar el Alta/Baja";
            std::cout << bcolors::GREEN << "\n2. 📝 Modificar Datos";
            std::cout << bcolors::GREEN << "\n3. 🔎 Realizar Busqueda";
            std::cout << bcolors::GREEN << "\n4. 🗎  Historial Clínico";
            std::cout << bcolors::YELLOW << "\n\n ▶ Operación ➟ " << bcolors::WHITE;
            std::cin >> operacion;

            gestionarPacientes(operacion, -1, -1, "", -1, -1, "", "");  // Función para gestionar pacientes
            break;

        case 2:
            std::cout << bcolors::BLUEL << "\n1. ⭥  Manejar el Alta/Baja";
            std::cout << bcolors::BLUEL << "\n2. ⚕️ Asignar Especialidad";
            std::cout << bcolors::BLUEL << "\n3. 🗎  Listar * Especialidad/Disponibilidad";
            std::cout << bcolors::YELLOW << "\n\n ▶ Operación ➟ " << bcolors::WHITE;
            std::cin >> operacion;
            gestionarMedicos(operacion, -1, "", -1, "", "");    // Función para gestionar medicos
            break;
        case 3:
            std::cout << bcolors::RED << "\n1. 🗓️ Asignar Cita";
            std::cout << bcolors::RED << "\n2. 📅 Ordenar Citas * Fecha/Urgencia";
            std::cout << bcolors::RED << "\n3. 🗎  Registros";
            std::cout << bcolors::RED << "\n4. 📝 Cancelar/Modificar cita";
            std::cout << bcolors::YELLOW << "\n\n ▶ Operación ➟ " << bcolors::WHITE;
            std::cin >> operacion;
            gestionarCitas(operacion, -1, "", "", "", "", "", -1, "", "", -1);      // Funcion para gestionar citas
            break;
        case 4:
            realizarBackup();   // Función para realizar backup
            break;
        default:
            std::cout << bcolors::RED << "Opción inválida!" << std::endl << std::endl;
            showHelp();
        
            return 1;
    }

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||| CLI FLAG ARGUMENTS ||||||||||||||||||||||||||||||||||||||||||||||||||||
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    } else { // Ejecutar argumentos en CLI

//  Definimos ciertos valores -1 y otros vacíos para gestionar correctamente como operar con los datos especificos proporcionados por el usuario, los datos esenciales no introducidos serán pedidos a introducir más adelante.
        int gestion; // {pacientes, medicos, citas}
        int operacion; // 
        int id = -1;
        std::string nombre;
        std::string nombreMedico;
        std::string especialidad; // de médico
        std::string nuevaEspecialidad; // de médico
        int edad = -1; // paciente
        int nuevaDisponibilidad = -1; // pacientes y medicos
        int aModificar = -1; // que modificar; nombre,edad,disponibilidad
        std::string modificacion; // Que categoria modificar
        std::string modificado; // nuevo valor dentro de <aModificar>
        std::string fecha; 
        std::string nuevaFecha; //  modificar la fecha
        std::string descripcion; // para gestion de citas
        std::string nuevoEstado; // citas -> modificarlo
        std::string urgencia;  // crear / ordenar citas
        int criterio = -1; // para --listar

        std::unordered_map<std::string, std::string> estadoMap = {
                {"-confirmar", "Confirmada"},
                {"-cancelar", "Cancelada"},
                {"-pendiente", "Pendiente"}
            };

        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--backup") == 0) {
                realizarBackup();
                return 0;  // Salimos del programa después de hacer el backup
            }
            if (strcmp(argv[i], "--gestionar") == 0) {
                if (i + 1 < argc) {
                    if (strcmp(argv[i + 1], "pacientes") == 0) {
                        gestion = 1;  // Gestionar pacientes
                    } else if (strcmp(argv[i + 1], "medicos") == 0) {
                        gestion = 2;  // Gestionar médicos
                    } else if (strcmp(argv[i + 1], "citas") == 0) {
                        gestion = 3;  // Gestionar citas
                    } else {
                        std::cout << bcolors::RED << "\n!!!Argumento desconocido para --gestionar: " << argv[i + 1] << bcolors::YELLOW << std::endl;
                        showHelp();
                        return 1;
                    }
                    i++; // Avanzar el índice para saltar el argumento procesado
                    continue; // Saltar a la siguiente iteración
                } else {
                    std::cout << bcolors::RED << "\n!!!Falta el tipo de gestión después de --gestionar" << bcolors::YELLOW << std::endl;
                    showHelp();
                    return 1;
                }
            }


            else if (strcmp(argv[i], "-id") == 0 && i + 1 < argc) { // ID de individuo
                id = std::atoi(argv[i + 1]); 
                i++;
            }  
            else if (strcmp(argv[i], "-N") == 0 && i + 1 < argc) { // Nombre
                nombre = argv[i + 1]; 
                i++;
            }
            else if (strcmp(argv[i], "-NM") == 0 && i + 1 < argc) { // Nombre Médico
                nombreMedico = argv[i + 1]; 
                i++;
            }
            else if (strcmp(argv[i], "-X") == 0 && i + 1 < argc) { // Especialidad
                especialidad = argv[i + 1]; 
                i++;
            }
            else if (strcmp(argv[i], "-NX") == 0 && i + 1 < argc) { // Nueva Especialidad
                nuevaEspecialidad = argv[i + 1]; 
                i++;
            }
            else if (strcmp(argv[i], "-E") == 0 && i + 1 < argc) { // Edad
                edad = std::atoi(argv[i + 1]); 
                i++;
            }
            else if (strcmp(argv[i], "-F") == 0 && i + 1 < argc) { // Fecha
                fecha = argv[i + 1]; 
                i++;
            }
            else if (strcmp(argv[i], "-NF") == 0 && i + 1 < argc) { // Nueva Fecha
                aModificar = 2;
                nuevaFecha = argv[i + 1]; 
                i++;
            }
                       
            else if (strcmp(argv[i], "-alta") == 0) { // Alta para pacientes y medicos
                operacion = 1;
                nuevaDisponibilidad = 1;  // Alta
            }
            else if (strcmp(argv[i], "-baja") == 0) { // Baja para pacientes y médicos
                operacion = 1;
                nuevaDisponibilidad = 0;  // Baja
            }

            else if (strcmp(argv[i], "-modificar") == 0 && i + 1 < argc) { // pacientes
                operacion = 2;
                if (i + 2 < argc) {

                    modificacion = argv[i + 1]; // Primer argumento del modificar (nombre, edad, disponibilidad)
                    modificado = argv[i + 2];  // Segundo argumento es el nuevo valor

                    // Determinar qué modificar y asignar los valores
                    if (modificacion == "nombre") {
                        aModificar = 1;  
                    } else if (modificacion == "edad") {
                        aModificar = 2;  
                    } else if (modificacion == "disponibilidad") {
                        aModificar = 3;  
                    }

                    i += 2;  // Avanzar dos posiciones
                }
            }
            else if (strcmp(argv[i], "-buscar") == 0) { // pacientes
                operacion = 3;
            }

            else if (strcmp(argv[i], "-historial") == 0) { // pacientes
                operacion = 4;
            }

            else if (strcmp(argv[i], "-especialidad") == 0) {
                operacion = 2;
            }
            
            else if (strcmp(argv[i], "-NC") == 0) { // Nueva Cita
                operacion = 1;
            }

            else if (strcmp(argv[i], "-MC") == 0) { // Modificar Cita
                operacion = 4;
            }


            else if (estadoMap.find(argv[i]) != estadoMap.end()) { // {confirmar, cancelar, pendiente} : Citas
                aModificar = 1;
                nuevoEstado = estadoMap[argv[i]];
                i++;
            }


            else if (strcmp(argv[i], "-descripcion") == 0 && i + 1 < argc) { // Descripcion de la cita
                aModificar = 3;
                descripcion = argv[i + 1]; 
                i++;
            }

            else if (strcmp(argv[i], "-urgencia") == 0 && i + 1 < argc) { // Urgencia de cita
                aModificar = 4;
                urgencia = argv[i + 1]; 
                i++;
            }

            else if (strcmp(argv[i], "--listar") == 0 && i + 1 < argc) { // Listar por fecha/urgencia las citas
                operacion = 2;
                if (strcmp(argv[i + 1], "fecha") == 0) {
                    criterio = 1;
                } else if (strcmp(argv[i + 1], "urgencia") == 0) {
                    criterio = 2;
                } else {
                    std::cout << bcolors::RED 
                            << "Parámetro no admitido, solamente -> fecha / urgencia"
                            << bcolors::YELLOW << std::endl;
                }
                i++; 
            }

            else if (strcmp(argv[i], "--reporte") == 0) { // Listar los registros clínicos
                operacion = 3;
                aModificar = 1;
            }
            else if (strcmp(argv[i], "-cronico") == 0) { // Listar los registros clínicos crónicos
                aModificar = 3;
            }

            else if (strcmp(argv[i], "-h") == 0) { // Menú de Ayuda
                showHelp(); 
                return 0;
            }

            else {
                std::cout << bcolors::RED << "\n!!!Argumento desconocido: " << argv[i] << bcolors::YELLOW << std::endl;
                showHelp();
                return 1;
            }
        }

// ------------------------------------------- Procesar Argumentos del CLI -------------------------------------------
        switch (gestion) {
            case 1: {
                // Operaciónes de pacientes
                gestionarPacientes(operacion, id, edad, nombre, nuevaDisponibilidad, aModificar, modificado, fecha); 
                break;
            }
                
            case 2: {
                // Operaciónes de médicos
                gestionarMedicos(operacion, id, nombre, nuevaDisponibilidad, especialidad, nuevaEspecialidad);
                break;
            }

            case 3: {
                // Operaciónes de Citas
                gestionarCitas(operacion, id, fecha, nuevaFecha, descripcion, nombre, nombreMedico, aModificar, nuevoEstado, urgencia, criterio);
                break;
            }
                
        }

    }

    return 0; // Salida sin errores
}

// ##################################################################################################################################################################
// ########################################################################## E N D #################################################################################
// ##################################################################################################################################################################