CFLAGS :=  

TARGET := chess

OBJS := main.o piece.o square.o move.o chess.o

main: $(OBJS) 
	g++ -g -o main $(OBJS)

main.o: main.cpp chess.cpp
	g++ -g -c main.cpp chess.cpp

piece.o: piece.cpp
	g++ -c piece.cpp 

square.o: square.cpp
	g++ -c square.cpp

move.o: move.cpp
	g++ -c move.cpp

chess.o: chess.cpp piece.cpp square.cpp
	g++ -c chess.cpp piece.cpp square.cpp

clean: 
	rm *.o
