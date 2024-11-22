# Nombre del compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -g

# Archivos de cabecera y fuente
HEADERS = archivos.hpp citas.hpp medicos.hpp pacientes.hpp reportes.hpp
SOURCES = main.cpp archivos.cpp citas.cpp medicos.cpp pacientes.cpp reportes.cpp

# Nombre del ejecutable
EXEC = SGH

# Objetos intermedios
OBJECTS = $(SOURCES:.cpp=.o)

# Regla principal (default)
all: $(EXEC)

# Regla para compilar el ejecutable
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar cada archivo .cpp a .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJECTS) $(EXEC)

# Phony targets (no generan archivos)
.PHONY: all clean
