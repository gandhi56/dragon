#ifndef _GAME_

#define _GAME_
#define BLACK   		1
#define WHITE   		(1<<1)
#define NEUTRAL			((1<<1)|1)
#define EMPTY   		0
#define VALBITS			2		// number of bits for value
#define DRAW			'#'
#define BLACK_STONE		'B'
#define WHITE_STONE		'W'
#define GAME_NOT_OVER	'.'

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#include <bitset>
#include <cstring>
using namespace std;
typedef bitset<VALBITS> Valtype;
typedef pair<Valtype, vector<uint16_t> > Cell;

#include "../../solver/solver.h"

class GoAction: public Action{
public:
	string move;
	GoAction()	:	move("")	{}
	GoAction(string m)	:	move(m)	{}
	GoAction& operator=(const GoAction& a){
		move = a.move;
		return *this;
	}
};

class GoState: public State{
public:
	uint16_t numRows;
	uint16_t numColumns;
	vector< Cell > graph;
	uint16_t emptyCount;
	uint16_t blackCount;
	uint16_t whiteCount;
	uint16_t playerJustMoved;
	char status;

	GoState();
	~GoState();
	GoState& operator=(GoState& s);

	// board construction
	void set_size(uint16_t rows, uint16_t cols);
	void create_graph();
	void set_nbrs(vector<uint16_t>& nbrs, uint16_t key);
	
	// display
	void show();

	// transition
	bool update(GoAction action);
	void revert(GoAction& action);
	int next();
	void switch_turns();
	
	// get actions
	void get_moves(vector<GoAction>& actions);
	void get_moves(vector<GoAction>& actions, bool isMax);
	
	// terminal test
	char check_win();
	bool connected(uint16_t key0, uint16_t end, bool blackConnect);
	
	// terminal test accessors
	char player1();
	char player2();
	char draw();
private:
	string get_key(uint16_t row, uint16_t col);
	string get_value(uint16_t row, uint16_t col);
	uint16_t num_nbrs(uint16_t row, uint16_t col);
	uint16_t get_row(string pos);
	uint16_t get_col(string pos);
	bool valid_pos(uint16_t key);
	bool valid_pos(uint16_t row, uint16_t col);
};



#endif	// _GAME_
