# Compiler
CXX = icpx
# Flags
CXXFLAGS = -std=c++20 -O3 -xHost
CXXFLAGS += -qopenmp

# Target and source files
TARGET = benchmark
SRC = src/main.cpp src/timing.c src/mmio.cpp

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean rule
clean:
	rm -f $(TARGET)
