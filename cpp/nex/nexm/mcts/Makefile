
CC = g++
CCFLAGS = -Wall -std=c++17
SRC = ./src/
OBJ = ./obj/
PORT = 21299

$(OBJ)run: $(OBJ)run.o $(OBJ)player.o $(OBJ)board.o
	@$(CC) $(CCFLAGS) $(OBJ)run.o $(OBJ)player.o $(OBJ)board.o -o $(OBJ)run

$(OBJ)board.o: $(SRC)board.cpp
	@$(CC) $(CCFLAGS) $(SRC)board.cpp -c -o $(OBJ)board.o

$(OBJ)player.o: $(SRC)player.cpp
	@$(CC) $(CCFLAGS) $(SRC)player.cpp -c -o $(OBJ)player.o

$(OBJ)run.o: $(SRC)run.cpp
	@$(CC) $(CCFLAGS) $(SRC)run.cpp -c -o $(OBJ)run.o


run: $(OBJ)run
	$(OBJ)run $(PORT)

clean:
	@rm -f $(OBJ)*.o
	@rm -f $(OBJ)player
	@rm -rf $(SRC).*.*.*
	@rm -rf ./include/.*.*.*
