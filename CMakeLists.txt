# Versión mínima requerida de CMake
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto y estándar de C++
project(SGH LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilitar warnings adicionales en el compilador
add_compile_options(-Wall -Wextra -pedantic -g)

# Directorio fuente y archivos
set(SRC_DIR src)
file(GLOB HEADERS "${SRC_DIR}/*.hpp")
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# Directorio de salida para archivos binarios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Agregar el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Directorio de compilación intermedia (objetos)
set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)


#       INSTRUCCIONES

# Compilar el proyecto: cmake build/ .

