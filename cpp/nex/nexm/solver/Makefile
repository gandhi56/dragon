
CC = g++
CCFLAGS = -Wall -std=c++17
SRC = ./src/
OBJ = ./obj/
PORT = 21299

$(OBJ)run: $(OBJ)run.o $(OBJ)player.o $(OBJ)game.o
	@$(CC) $(CCFLAGS) $(OBJ)run.o $(OBJ)player.o $(OBJ)game.o -o $(OBJ)run

$(OBJ)game.o: $(SRC)game.cpp
	@$(CC) $(CCFLAGS) $(SRC)game.cpp -c -o $(OBJ)game.o

$(OBJ)player.o: $(SRC)player.cpp
	@$(CC) $(CCFLAGS) $(SRC)player.cpp -c -o $(OBJ)player.o

$(OBJ)run.o: $(SRC)run.cpp
	@$(CC) $(CCFLAGS) $(SRC)run.cpp -c -o $(OBJ)run.o

compile:	$(OBJ)run

run: $(OBJ)run
	$(OBJ)run $(PORT)

clean:
	@rm -f $(OBJ)*.o
	@rm -f $(OBJ)player
	@rm -rf $(SRC).*.*.*
	@rm -rf ./include/.*.*.*

