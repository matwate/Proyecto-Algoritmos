# Detect operating system
OS := $(shell uname -s)

# Compiler and flags
ifeq ($(OS), Darwin)
    CXX = clang++
    CXXFLAGS = -I../raylib/src -I./src -I./src/helpers -I./src/visualizers -I./src/common
    FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    LDFLAGS = lib/libraylib.a
    TARGET_EXTENSION =
else ifeq ($(OS), Linux)
    CXX = g++
    RAYLIB = ./external/raylib-5.5/src
    CXXFLAGS = -I$(RAYLIB) -I./src -I./src/helpers -I./src/visualizers -I./src/common
    LDFLAGS = -L$(RAYLIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    FRAMEWORKS =
    TARGET_EXTENSION =
else
    CXX = g++
    CXXFLAGS = -I../raylib/src -I./src -I./src/helpers -I./src/visualizers -I./src/common
    FRAMEWORKS =
    LDFLAGS = -lraylib -lgdi32 -lwinmm
    TARGET_EXTENSION = .exe
endif

# Source files and output
SRC = src/main.cpp src/helpers/InputHelper.cpp src/visualizers/SortVisualizer.cpp src/visualizers/GraphVisualizer.cpp src/visualizers/TreeVisualizer.cpp
OBJ = $(SRC:src/%.cpp=build/%.o)
TARGET = build/my_app$(TARGET_EXTENSION)

# Default rule
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJ)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(OBJ) $(FRAMEWORKS) $(LDFLAGS) -o $@

# Compilation rule
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf build

# Run rule
run: $(TARGET)
	@cd build && ./$(notdir $(TARGET))

.PHONY: all clean run
