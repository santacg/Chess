CPP := g++
CPPFLAGS := -std=c++23 -Wall -Wextra -pedantic -O3 -g -I/usr/include/onnxruntime

LDFLAGS := -lonnxruntime

INCLUDES_DIR := includes
SRC_DIR := src
OBJS_DIR := objs
TARGET := chess
OBJS := chess.o bitboard.o move.o lookup_table.o utils.o model.o

$(TARGET): $(OBJS)
	$(CPP) $(CPPFLAGS) $(LDFLAGS) -o $@ $^
	mkdir -p $(OBJS_DIR)
	mv $(OBJS) $(OBJS_DIR)

%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR) $(TARGET)
	clear

