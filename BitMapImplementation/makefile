CFLAGS := -Wall -Wextra -pedantic -g

TARGET := chess
OBJS := chess.o chess_board.o

$(TARGET): $(OBJS)
	g++ -o $@ $^

chess.o: chess.cpp chess_board.cpp
	g++ $(CFLAGS) -c chess.cpp chess_board.cpp

chess_board.o: chess_board.cpp
	g++ $(CFLAGS) -c chess_board.cpp

clean:
	rm $(OBJS) $(TARGET)
