# Versión mínima de CMake
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(SGH LANGUAGES CXX)

# Versión del estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags de compilación
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g")

# Directorios fuente y de cabecera
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB SOURCES ${SRC_DIR}/*.cpp)
file(GLOB HEADERS ${SRC_DIR}/*.hpp)

# Nombre del ejecutable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Crear una regla para copiar la carpeta db junto al ejecutable
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SRC_DIR}/db ${CMAKE_BINARY_DIR}/db
    COMMENT "Copiando carpeta db al directorio de salida..."
)


#       INSTRUCCIONES

# Construir el proyecto: cd build -> cmake .. 
# Luego con make compilarlo: make  
