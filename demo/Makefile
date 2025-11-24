CXX = g++
CXXFLAGS = -std=c++20 -Wall -O2
LIBS = -lSDL2 -lGL -lm
TARGET = FlarkVizDemo

all: $(TARGET)

$(TARGET): FlarkVizDemo.cpp
	@echo "Building FlarkViz Demo..."
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)
	@echo "Build complete! Run with: ./$(TARGET)"

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
