# Versión mínima de CMake
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(SGH LANGUAGES CXX)

# Versión del estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags de compilación
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g")

# Asegurarse de que la entrada y salida estén en UTF-8
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8 -fexec-charset=UTF-8")

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

# Asegurarse de que el entorno de ejecución use UTF-8 (opcional)
# Esto configura la localización para que el programa maneje UTF-8 correctamente
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUTF8_SUPPORT")


#       INSTRUCCIONES
# Crear el directorio build: mkdir build
# Construir el proyecto: cd build -> cmake .. 
# Luego con make compilarlo: make  
