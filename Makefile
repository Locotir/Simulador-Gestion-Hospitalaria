# Nombre del compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -g

# Directorio fuente
SRC_DIR = src

# Archivos de cabecera y fuente
HEADERS = $(wildcard $(SRC_DIR)/*.hpp)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Directorio de objetos intermedios
BUILD_DIR = build

# Archivos de objetos intermedios
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Nombre del ejecutable
EXEC = SGH

# Regla principal (default)
all: $(EXEC)

# Regla para compilar el ejecutable
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar cada archivo .cpp a .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(BUILD_DIR)  # Crear el directorio build si no existe
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

# Phony targets (no generan archivos)
.PHONY: all clean
