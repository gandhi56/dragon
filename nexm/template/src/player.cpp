
#include "../include/player.h"


Player::Player(char* servIp, int port){
	this->_socket.port = port;
	this->_socket.host = gethostbyname(servIp);
	this->_socket.clientSd = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char*)&this->_socket.sendSockAddr, 
		sizeof(this->_socket.sendSockAddr));
	this->_socket.sendSockAddr.sin_family = AF_INET;
	this->_socket.sendSockAddr.sin_addr.s_addr = 
		inet_addr(servIp);

	this->_socket.sendSockAddr.sin_port = htons(port);
	
	int status = connect(_socket.clientSd, (sockaddr*)&_socket.sendSockAddr, sizeof(_socket.sendSockAddr));
	if(status < 0){
		cout << "Error connecting to _socket!" << endl;
	}

	cout << "Connected to the server!" << endl;
	
	/* implement proper read settings function after including the game class */
	movesCount = 0;

	char buffer[40];
	memset(buffer, 0, sizeof(buffer));
	read(_socket.clientSd, (char *)&buffer, sizeof(buffer));
	cout << "rec: " << buffer << endl;
	if(send_data("Recieved game details") == -1){
		perror("Not able to send acknowledgement\n");
	}

	uint numRows;
	uint numColumns;
	read_settings(buffer, numRows, numColumns);

	state.set_size(numRows, numColumns);
	state.create_graph();

	memset(&buffer, 0, sizeof(buffer));
	recieve_data(buffer);
	this->set_state(string(buffer));
	send_data("Recieved game state");
}

int Player::recieve_data(char* data){
	return read(_socket.clientSd, (char *)&data, sizeof(data));
}
int Player::send_data(const char* msg){
	return send(this->_socket.clientSd, msg, strlen(msg), 0);
}
Player::~Player(){
	close(this->_socket.clientSd);
}

void Player::set_state(string moves){
    uint i = 0;
    uint j = 1;
    if(moves == "DEFAULT;") return;
    while (i < moves.length()){
        if (moves[i] == BLACK || moves[i] == WHITE || moves[i] == NEUTRAL){
            j = i + 1;
            while (moves[j] != ';') j++;
            state.update(moves.substr(i, j-i));
            i = j+1;

        }
        else{
            cout << "Error parsing start state\n";
            break;
        }
    }
}

void Player::read_settings(char* buff, uint& rows, uint& cols){
    /*
        read row and column dimensions from server.
    */

    string _buffer = string(buff);
    uint i = 0;
    while (_buffer[i] != 'r')   i++;
    i++;
    if (_buffer[i+1] == '-'){
        rows = stoi(_buffer.substr(i, 1));
    }
    else{
        rows = stoi(_buffer.substr(i, 2));
    }

    i = 0;
    while (_buffer[i] != 'c')   i++;
    i++;
    if (_buffer[i+1] == '-'){
        cols = stoi(_buffer.substr(i, 1));
    }
    else{
        cols = stoi(_buffer.substr(i, 2));
    }

    cout << "rows=" << rows << endl;
    cout << "cols=" << cols << endl;
}

void Player::run(){
	string data;
	char _data[64];
	while (1){
		memset(&_data, 0, sizeof(_data));
		read(this->_socket.clientSd, 
			(char*)&_data, sizeof(_data));
		cout << "rec:" << _data << endl;
				
		if (!strcmp(_data, "!")){
			exit(1);
		}
		else if (!strcmp(_data, "Move?")){

			// send move here
			cout << ">";
			getline(cin, data);

			send_data(data.c_str());
		}
		else if (!strcmp(_data, "Result+")){
			cout << "Yahoo, I won!!" << endl;
			break;
		}
		else if (!strcmp(_data, "Result-")){
			cout << "Ugh, I lost!" << endl;
			break;
		}
		else if (!strcmp(_data, "Result#")){
			cout << "Draw!" << endl;
			break;
		}
		else if(_data[0] == '>'){
			// to update state in memory
			data = string(_data);
			string move = data.substr(1, data.length() - 1);
			state.update(move);
			state.show();
			movesCount++;
			send_data("move recieved");
			continue;
		}
		else{
			break;
		}

	}
}


