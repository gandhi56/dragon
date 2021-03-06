#ifndef _PLAYER_
#define _PLAYER_

#include <stdio.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <fstream>

#include "../include/search.h"
// #include <mcts.h>

using namespace std;

// typedef MCTS<State, string, NexExpansionStrategy, NexPlayoutStrategy> nex_mcts;

struct ClientSocket{
	int port;
	struct hostent* host;
	sockaddr_in sendSockAddr;
	int clientSd;
};

class Player{
	State* state;
	// nex_mcts* mcts;
};

class Client{
private:
	ClientSocket _socket;
	uint movesCount;
	bool socketConnected;

	void init_vars();
	void attach_socket(char* servIp, int port);
	void read_settings(char* buff, uint& rows, uint& cols);
	void connect_server();
	
public:
	char myStone;
	State gameState;

	Client();
	Client(char* servIp, int port);
	~Client();

	void set_state(string moves);
	void run(bool disp);
	void solve(State state, bool isMax, bool disp);
	
	void get_moves(State state, vector<string>& moves, bool isMax);
	
	// negamax
	int evaluate(State state, bool isMax);
	string best_neg_move(State state, int depth, bool isMax, bool disp);
	int negamax(State state, int depth, bool isMax, int alpha, int beta, bool disp);
};

#endif // _PLAYER_
