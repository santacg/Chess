CPP := g++
CPPFLAGS := -std=c++20 -Wall -Wextra -pedantic -g

TARGET := chess
OBJS := chess.o chess_board.o lookup_table.o utils.o

$(TARGET): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $^

chess.o: chess.cpp chess_board.h
	$(CPP) $(CPPFLAGS) -c chess.cpp

chess_board.o: chess_board.cpp chess_board.h lookup_table.h utils.h
	$(CPP) $(CPPFLAGS) -c chess_board.cpp

lookup_table.o: lookup_table.cpp lookup_table.h
	$(CPP) $(CPPFLAGS) -c lookup_table.cpp

utils.o: utils.cpp utils.h
	$(CPP) $(CPPFLAGS) -c utils.cpp

clean:
	rm -f $(OBJS) $(TARGET)

