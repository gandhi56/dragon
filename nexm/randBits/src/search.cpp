#include "../include/search.h"

/*
	string move;
	MCTNode* parentNode;
	vector<MCTNode> childNodes;
	State state;
	int wins;
	int visits;
*/
	
MCTNode::MCTNode(){
	parentNode = NULL;
}

MCTNode::MCTNode(string _move, MCTNode* parent, State gameState, Valtype currPlayer){
	move = _move;	// move that reached this state
	parentNode = parent;
	state = gameState;
	ptm = currPlayer;
	expand_node();
}

void MCTNode::add_child(string move){
	vector<string>::iterator it = 
		find(untriedMoves.begin(), untriedMoves.end(), move);
	if (it != untriedMoves.end()){
		untriedMoves.erase(it);
	}
	else{
		cout << "Error: move " << move << "  not found." << endl;
	}
	State newState = state;
	newState.update(move);

	MCTNode node(move, this, newState, (ptm==BLACK?WHITE:BLACK));

	MCTNode* newNode = &node;
	childNodes.push_back(newNode);
}


void MCTNode::update(int result){
	visits++;
	wins += result;
}

void MCTNode::print(){
	cout << "[M:" << move << " W/V:" << wins << "/" << visits << "]" << endl;
}

void MCTNode::operator=(MCTNode node){
	move = node.move;
	parentNode = node.parentNode;
	state = node.state;
	wins = node.wins;
	visits = node.visits;
	ptm = node.ptm;

	for (string move : node.untriedMoves){
		untriedMoves.push_back(move);
	}
	for (auto child : node.childNodes){
		childNodes.push_back(child);
	}
}

/*
void MCTNode::uct_simulation(State state, int maxiter, Valtype currPlayer){
	MCTNode rootNode("None", NULL, state, ptm);
	MCTNode node;
	State currState;

	for (int iter = 1; iter <= maxiter; ++iter){
		node = rootNode;
		currState = state;

		// Selection --------------------------------------------------------
		while (node.untriedMoves.size() == 0 and node.childNodes.size() != 0){
			// find the best move
			node = node.childNodes[0];
			currState.update(node.move);
		}
		// ------------------------------------------------------------------
		
		// Expansion---------------------------------------------------------
		if (node.untriedMoves.size() != 0){
			string move;
			get_random_move(move);
			currState.update(move);
			node.add_child(move, currState);
			node = node.childNodes.back();
		}
		// ------------------------------------------------------------------
	
		// Rollout-----------------------------------------------------------
		//while not leaf node
		//	play random simulation
		// ------------------------------------------------------------------
		
		// Backpropagation---------------------------------------------------
		//while node is not root
		//	update node stats
		// ------------------------------------------------------------------
	}
}
*/
void MCTNode::expand_node(){
	string myStone = "B";
	if (ptm == WHITE)	myStone = "W";

	// store all cells where a stone may be placed
	vector<string> emptyPos;
	vector<string> stonePos;
	vector<string> neutralPos;
	for (uint row = 0; row < state.numRows; ++row){
		for (uint col = 0; col < state.numColumns; ++col){
			uint key = row * state.numColumns + col;
			if (state.graph[key].first == EMPTY){
				emptyPos.push_back(state.get_key(row, col));
			}
			else if((state.graph[key].first==BLACK and ptm==BLACK) or
				(state.graph[key].first == WHITE and ptm==WHITE)){
				stonePos.push_back(state.get_key(row, col));
			}
			else if (state.graph[key].first == NEUTRAL){
				neutralPos.push_back(state.get_key(row, col));
			}
		}
	}


	if (emptyPos.size() >= 2){
	
		for (uint i = 0; i < emptyPos.size(); ++i){
			for (uint j = i+1; j < emptyPos.size(); ++j){
				string key0 = myStone;
				key0 += emptyPos[i] + "?" + emptyPos[j];
				untriedMoves.push_back(key0);

				string key1 = "?";
				key1 += emptyPos[i] + myStone  + emptyPos[j];
				untriedMoves.push_back(key1);
			}
		}
	}

	if (neutralPos.size() >= 2 && stonePos.size() >= 1){
				
		for (uint i = 0; i < neutralPos.size(); ++i){
			for (uint j = i + 1; j < neutralPos.size(); ++j){
				for (uint k = 0; k < stonePos.size(); ++k){
					string key = myStone;
					key += neutralPos[i]+myStone+neutralPos[j];
					key += "?" + stonePos[k];
					untriedMoves.push_back(key);
				}
			}
		}
	}
}

