CXXFLAGS = -Wall -Wextra -std=c++11

Objects = 2048.o game.o grid.o io.o

.DEFAULT: 2048

2048: $(Objects)
	$(CXX) $(CXXFLAGS) $(Objects) -o 2048
