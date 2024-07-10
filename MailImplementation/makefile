CFLAGS := -Wall -pedantic -Wextra $(CFLAGS)

TARGET := chess

OBJS := main.o piece.o square.o move.o chess.o

main: $(OBJS) 
	g++ -o main $(OBJS)

main.o: main.cpp chess.cpp
	g++ $(CFLAGS) -c main.cpp chess.cpp

piece.o: piece.cpp
	g++ $(CFLAGS) -c piece.cpp 

square.o: square.cpp
	g++ $(CFLAGS) -c square.cpp

move.o: move.cpp
	g++ $(CFLAGS) -c move.cpp

chess.o: chess.cpp piece.cpp square.cpp
	g++ $(CFLAGS) -c chess.cpp piece.cpp square.cpp

clean: 
	rm *.o
