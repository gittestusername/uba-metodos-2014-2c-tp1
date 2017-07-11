CXXFLAGS= -Wall -Wextra -pedantic -Werror -std=c++0x -O2 -ggdb -masm=intel
OBJS=Matrix.o WithFifteenThetas.o

.PHONY: test all clean

main: main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o tp $< $(OBJS) -DRELEASE
	
test: main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o main $< $(OBJS) -DTEST
	
all: main

clean:
	rm -f tp main *.o *~
