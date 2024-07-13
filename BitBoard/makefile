CPP := g++
CPPFLAGS := -std=c++23 -Wall -Wextra -pedantic -g

TARGET := chess
OBJS := chess.o bitboard.o lookup_table.o

$(TARGET): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $^

chess.o: chess.cpp bitboard.h
	$(CPP) $(CPPFLAGS) -c chess.cpp

bitboard.o: bitboard.cpp bitboard.h lookup_table.h
	$(CPP) $(CPPFLAGS) -c bitboard.cpp

lookup_table.o: lookup_table.cpp lookup_table.h
	$(CPP) $(CPPFLAGS) -c lookup_table.cpp

clean:
	rm -f $(OBJS) $(TARGET)

