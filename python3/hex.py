from copy import deepcopy
import random
from math import sqrt, log

BLACK_WON = 0
WHITE_WON = 1
DRAW = 2
NOT_DONE = 3


class HexState:
	def __init__(self, nrows=3, ncols=3):
		self.playerJustMoved = 2
		self.board = { chr(j+ord('a')) : {str(i+1) : '.' for i in range(ncols)} for j in range(nrows)}
		self.count = {'B':0, 'W':0, '?':0}
		self.status = NOT_DONE
		self.nrows = nrows
		self.ncols = ncols

	def get_size(self):
		return (len(self.board), len(self.board['a']))

	def next(self):
		return 3 - self.playerJustMoved

	def play(self, move, used_by_undo = False):
		def submove(board, count, m):
			stone, row, col = m[0], m[1], m[2:]
			board[row][col] = stone
			if not used_by_undo:
				count[stone] += 1

		stone, row, col = move[0], move[1], move[2:]
		self.board[row][col] = stone
		self.count[stone] += 1

		self.playerJustMoved = self.next()

		if self.connected(('B', '0'), ('B', '1')):
			self.status = BLACK_WON
			print('black won')
		elif self.connected(('W', '0'), ('W', '1')):
			self.status = WHITE_WON
			print('white won')

	def get_moves(self):
		assert(self.status == NOT_DONE)

		moves = list()
		stone = 'B'
		if self.playerJustMoved == 2:
			stone = 'B'
		else:
			stone = 'W'

		for r in self.board.keys():
			for c in self.board[r].keys():
				if self.board[r][c] == '.':
					moves.append(stone+r+c)
		return moves

	def gameover(self):
		return self.status == BLACK_WON or self.status == WHITE_WON

	def evaluate(self, player):
		# assume terminal state has been reached
		if (player == 1 and self.status == BLACK_WON) or (player == 2 and self.status == WHITE_WON):
			return 1.0
		return 0.0
	
	def show(self):
		print('  ', end='')
		for c in range(1, self.ncols+1):
			if c < 10:
				print('  {}'.format(c), end='')
			else:
				print(' {}'.format(c), end='')
		print()
		for r in self.board.keys():
			print(' '*(ord(r)-ord('a')), end='')
			print(r, end=' \\')
			for c in self.board[r].keys():
				print('  {}'.format(self.board[r][c]), end='')
			print('  \\', r)

		print(' ', end='')
		for r in range(self.nrows+2):
			print(' ', end='')
		for c in range(1, self.ncols+1):
			if c < 10:
				print('  {}'.format(c), end='')
			else:
				print(' {}'.format(c), end='')
		print('\n')

	def nbrs(self, key):
		if key[0] == 'B':
			r = 'a'
			if key[1] == '1':
				r = chr(ord('a')+self.nrows-1)
			return [(r, str(c)) for c in range(1, self.ncols+1)]
		elif key[0] == 'W':
			c = '1'
			if key[1] == '1':
				c = chr(ord('1')+self.ncols-1)
			return [(r,c) for r in self.board.keys()]

		nbrs = []
		if self.valid_pos(chr(ord(key[0])+1), key[1]):
			nbrs.append((chr(ord(key[0])+1), key[1]))
		if self.valid_pos(chr(ord(key[0])-1), key[1]):
			nbrs.append((chr(ord(key[0])-1), key[1]))
		if self.valid_pos(key[0], chr(ord(key[1])+1)):
			nbrs.append((key[0], chr(ord(key[1])+1)))
		if self.valid_pos(key[0], chr(ord(key[1])-1)):
			nbrs.append((key[0], chr(ord(key[1])-1)))
		if self.valid_pos(chr(ord(key[0])-1), chr(ord(key[1])+1)):
			nbrs.append((chr(ord(key[0])-1), chr(ord(key[1])+1)))
		if self.valid_pos(chr(ord(key[0])+1), chr(ord(key[1])-1)):
			nbrs.append((chr(ord(key[0])+1), chr(ord(key[1])-1)))

		if key[0] == 'a':
			nbrs.append(('B', '0'))
		if ord(key[0])-ord('a') == self.nrows-1:
			nbrs.append(('B', '1'))
		if key[1] == '1':
			nbrs.append(('W', '0'))
		if ord(key[1])-ord('1') == self.nrows-1:
			nbrs.append(('W', '1'))

		return nbrs

	def valid_pos(self, r,c):
		return (r >= 'a' and r < chr(ord('a')+self.nrows) and int(c) >= 1 and int(c) <= self.ncols)

	def connected(self, start, end):
		isConnected = False
		visited = {('B','0'):False, ('B','1'):False,('W','0'):False, ('W','1'):False}
		for r in self.board.keys():
			for c in self.board[r].keys():
				visited[(r,c)] = False

		stack = list()
		stack.append(start)
		visited[start] = True
		while len(stack) > 0 and not isConnected:
			curr = stack.pop()
			if curr == end:
				isConnected = True
				break
			for adj in self.nbrs(curr):
				if visited[adj]:
					continue
				if adj == end:
					stack.append(adj)
					visited[adj] = True
					isConnected = True
					break
				if adj[0] == 'B' or adj[0] == 'W':
					continue
				if self.board[adj[0]][adj[1]] == 'B' and curr[0] != 'W':
					if curr[0] == 'B' or self.board[curr[0]][curr[1]] == 'B':
						stack.append(adj)
						visited[adj] = True
				elif self.board[adj[0]][adj[1]] == 'W' and curr[0] != 'B':
					if curr[0] == 'W' or self.board[curr[0]][curr[1]] == 'W':
						stack.append(adj)
						visited[adj] = True


		return isConnected

def play_game(state):
	from dragon.mcts import MCTNode, mcts
	while True:
		#print(state.status)
		#state.show()
		state.get_result(state.playerJustMoved)

		if state.status != NOT_DONE:
			break

		if state.playerJustMoved == 1:
			move, rate = mcts(rootState = state, itermax = 1000, verbose = False)
		else:
			move, rate = mcts(rootState = state, itermax = 1000, verbose = False)

		#print('Best move estimate:', move)
		state.do_move(move)

	if state.status == BLACK_WON:
		#print('Black')
		black += 1
	elif state.status == WHITE_WON:
		#print('White')
		white += 1
	elif state.status == DRAW:
		#print('Draw')
		draw += 1
	#else:
		#print('Error')

	print()
	print('black wins', black)
	print('white wins', white)
	print('draw', draw)

if __name__ == "__main__":
	import cProfile
	from fire.mcts import *
	
	player1 = 'uct'
	player2 = 'uct'

	state = HexState(3, 3)

	mcts(state, 5.0)

	'''
	while True:
		cmd = input('? ').strip().split()
		if len(cmd) == 0:
			continue
		if cmd[0] == 'quit':
			break
		elif cmd[0] == 'show':

			print('############################################')
			print('#  {} vs {} #'.format(player1, player2))
			print('############################################')
			state.show()
			print('Current player: {}'.format('1' if state.playerJustMoved == 2 else '2'))
			print()

		elif cmd[0] == 'new':
			state = HexState(int(cmd[1]), int(cmd[2]))

		elif cmd[0] == 'run':
			showState = False
			showEstimate = False
			numGames = int(cmd[1])

			for flag in cmd[2:]:
				if flag == '-state':
					showState = True
				elif flag == '-estimate':
					showEstimate = True

			_state = state.clone()

			blackWins = 0
			whiteWins = 0
			draws = 0

			print('{} vs {}'.format(player1, player2))
			

			for it in range(numGames):
				state = _state.clone()
				while True:
					if showState:
						state.show()
					state.get_result(state.playerJustMoved)
					if state.status != NOT_DONE:
						break
					if state.playerJustMoved == 1:
						if player1 == 'human':
							move = input('Enter move:')
						else:
							move, rate = mcts(rootState=state.clone(), itermax=10000, verbose=False, select_policy=player1)
					else:
						if player2 == 'human':
							move = input('Enter move:')
						else:
							move, rate = mcts(rootState=state.clone(), itermax=10000, verbose=False, select_policy=player2)
					
					if showEstimate:
						print('Best move estimate:', move)
					
					state.do_move(move)

				print('Game #{} '.format(it), end='')
				if state.status == BLACK_WON:
					print('*** Black wins! ***')
					blackWins += 1
				elif state.status == WHITE_WON:
					print('*** White wins! ***')
					whiteWins += 1
				elif state.status == DRAW:
					print('*** Draw ***')
					draws += 1

			print('\nNumber of games = {}'.format(numGames))
			print('Black won {} games'.format(blackWins))
			print('White won {} games'.format(whiteWins))
			print('{} games ended in a draw\n'.format(draws))

			state = _state.clone()
			del _state

		elif cmd[0] == 'search':
			numSimulations = 1000
			for flag in cmd[1:]:
				if flag[:len('-sim=')] == '-sim=':
					numSimulations = int(flag[5:].strip())

			state.show()

			move, winRate = mcts(rootState = state.clone(), itermax = numSimulations, verbose = False, select_policy='uct')
			print('Best move found: %s\tWin ratio: %.3f' %(move, winRate))

		elif cmd[0] == 'play':
			# TODO use regex to check if valid move was written in input
			move = cmd[1]
			state.make_move(move)

		elif cmd[0] == 'switch':
			state.playerJustMoved = 3 - state.playerJustMoved

		elif cmd[0] == 'config':
			if cmd[1] == 'black' and (cmd[2] in AI or cmd[2] == 'human'):
				player1 = cmd[2]
			elif cmd[1] == 'white' and (cmd[2] in AI or cmd[2] == 'human'):
				player2 = cmd[2]
			else:
				continue

		else:
			print('\t\t\tWelcome to dragon 1.0 manual')
			print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
			print('show\t\t\t\tprints the current state of the board\n')
			print('new <R> <C>\t\t\t\tcreates an empty state of size R x C\n')
			print('run <N> [-state] [-estimate]\t\truns N games between the players')
			print('\t\t\t\t\tshowing the state and the estimate if the flags are provided\n')
			print('search [N]\t\t\t\tsearching for the best move in N simulations, 1000 simulations\n')
			print('play <M>\t\t\t\tplays the given move M over the current state\n')
			print('switch\t\t\t\t\tswitches the player to move\n')
			print('config <black/white> <AI>\t\t\tconigure player modes\n')
			print('quit\t\t\t\t\tquit the program\n')


	'''


