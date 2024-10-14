CPP := g++
CPPFLAGS := -std=c++23 -Wall -Wextra -pedantic -g

INCLUDES_DIR := includes
SRC_DIR := src
OBJS_DIR := objs
TARGET := chess
OBJS := chess.o bitboard.o move.o lookup_table.o utils.o

$(TARGET): $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $^
	mkdir -p $(OBJS_DIR)
	mv $(OBJS) $(OBJS_DIR)

chess.o: $(SRC_DIR)/chess.cpp $(INCLUDES_DIR)/bitboard.h
	$(CPP) $(CPPFLAGS) -c $(SRC_DIR)/chess.cpp

bitboard.o: $(SRC_DIR)/bitboard.cpp $(INCLUDES_DIR)/bitboard.h $(INCLUDES_DIR)/lookup_table.h $(INCLUDES_DIR)/move.h $(INCLUDES_DIR)/utils.h
	$(CPP) $(CPPFLAGS) -c $(SRC_DIR)/bitboard.cpp

lookup_table.o: $(SRC_DIR)/lookup_table.cpp $(INCLUDES_DIR)/lookup_table.h
	$(CPP) $(CPPFLAGS) -c $(SRC_DIR)/lookup_table.cpp

move.o: $(SRC_DIR)/move.cpp $(INCLUDES_DIR)/move.h $(INCLUDES_DIR)/utils.h
	$(CPP) $(CPPFLAGS) -c $(SRC_DIR)/move.cpp

utils.o: $(SRC_DIR)/utils.cpp $(INCLUDES_DIR)/utils.h
	$(CPP) $(CPPFLAGS) -c $(SRC_DIR)/utils.cpp

clean:
	rm -rf $(OBJS_DIR) $(TARGET)
	clear

