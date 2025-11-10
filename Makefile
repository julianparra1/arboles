# Compiler and flags
CXX = clang++
CC = clang

# Optimization flags
OPTFLAGS = -O3 -march=native -mtune=native -flto
# Warning flags
WARNFLAGS = -Wall -Wextra -Wpedantic -Wno-unused-parameter
# Include paths
INCLUDES = -Iexternal/glad/include -Iexternal/imgui -Isrc -Iinclude
# Feature flags
CXXFLAGS = -std=c++17 $(WARNFLAGS) $(OPTFLAGS) $(INCLUDES)
CFLAGS = $(WARNFLAGS) $(OPTFLAGS) $(INCLUDES)

# Dependency generation flags
DEPFLAGS = -MMD -MP

# Parallel compilation (use all available cores)
MAKEFLAGS += -j$(shell nproc)

# Build directory
BUILD_DIR = build

# Libraries
LIBS = -lglfw -lGL -ldl

# Output executable
TARGET = arboles

# Source files
SRC_DIR = src
CORE_SOURCES = $(SRC_DIR)/main.cpp
RENDERING_SOURCES = $(SRC_DIR)/rendering/Shader.cpp $(SRC_DIR)/rendering/Camera.cpp
LSYSTEM_SOURCES = $(SRC_DIR)/lsystem/LSystem.cpp $(SRC_DIR)/lsystem/TurtleRenderer.cpp
UI_SOURCES = $(SRC_DIR)/ui/UI.cpp
IMGUI_SOURCES = external/imgui/imgui.cpp external/imgui/imgui_draw.cpp \
                external/imgui/imgui_tables.cpp external/imgui/imgui_widgets.cpp \
                external/imgui/imgui_impl_glfw.cpp external/imgui/imgui_impl_opengl3.cpp
GLAD_SRC = external/glad/src/glad.c

# All C++ sources
CPP_SOURCES = $(CORE_SOURCES) $(RENDERING_SOURCES) $(LSYSTEM_SOURCES) $(UI_SOURCES) $(IMGUI_SOURCES)

# Object files in build directory
CPP_OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CPP_SOURCES))
OBJECTS = $(CPP_OBJECTS) $(BUILD_DIR)/glad.o

# Dependency files (auto-generated)
DEPS = $(CPP_OBJECTS:.o=.d)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory structure
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)/src/core
	@mkdir -p $(BUILD_DIR)/src/rendering
	@mkdir -p $(BUILD_DIR)/src/shapes
	@mkdir -p $(BUILD_DIR)/src/lsystem
	@mkdir -p $(BUILD_DIR)/src/ui
	@mkdir -p $(BUILD_DIR)/external/imgui

# Link the executable with LTO
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET) with optimizations..."
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "Build complete: $(TARGET)"

# Pattern rule for C++ source files
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# Compile glad.c
$(BUILD_DIR)/glad.o: $(GLAD_SRC) | $(BUILD_DIR)
	@echo "Compiling glad.c..."
	$(CC) $(CFLAGS) -c $(GLAD_SRC) -o $@

# Include auto-generated dependency files
-include $(DEPS)

# Run the application
run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR) $(TARGET)

# Clean everything
clean-all: clean

# Phony targets
.PHONY: all run clean clean-all help

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the application (default)"
	@echo "  run       - Build and run the application"
	@echo "  clean     - Remove object files and executable"
	@echo "  clean-all - Remove all build artifacts"
	@echo "  help      - Show this help message"
