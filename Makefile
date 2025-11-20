# Compiler
CXX = icpx
# Flags
CXXFLAGS = -O3 -xHost
CXXFLAGS += -qopenmp

# Target and source files
TARGET = benchmark
SRC = src/main.cpp src/timing.c

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean rule
clean:
	rm -f $(TARGET)
