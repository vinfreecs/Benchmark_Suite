# Compiler
CXX = g++
# Flags
CXXFLAGS = -std=c++17 -O3 
CXXFLAGS += -fopenmp

# Target and source files
TARGET = benchmark
SRC = main.cpp

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean rule
clean:
	rm -f $(TARGET)
