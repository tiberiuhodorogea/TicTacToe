#include"State.h"
#include<iostream>
#include<cstring>


State::State()
{
	_player = playerX;
	 memset(_board[0], NULL, 3 * sizeof(BoardCellValue));
	 memset(_board[1], NULL, 3 * sizeof(BoardCellValue));
	 memset(_board[2], NULL, 3 * sizeof(BoardCellValue));
}

State::State(const State& state)
{
	_player = state.getPlayer();
	memcpy(_board[0], state._board[0], 3 * sizeof(BoardCellValue));
	memcpy(_board[1], state._board[1], 3 * sizeof(BoardCellValue));
	memcpy(_board[2], state._board[2], 3 * sizeof(BoardCellValue));
}

BoardCellValue State::getBoardCellValue(char i, char j)
{
	return _board[i][j];
}

Player State::getOpponent() {
	Player ret = _player == playerX ? playerO : playerX;
	return ret;
}
void State::setPlayer(Player player)
{
	_player = player;
}

Player State::getPlayer() const
{
	return _player;
}

void State::setBoardCellValue(const BoardCellValue value,char i, char j)
{
	_board[i][j] = value;
}

static bool areElementsEqual(BoardCellValue* line) {
	if (line[0] != NONE &&
		line[0] == line[1] && 
		line[1] == line[2]  )
	{
		return true;
	}

	return false;
}

static bool checkLines(BoardCellValue board[3][3])
{
	if ( areElementsEqual(board[0]) ||
		 areElementsEqual(board[1]) || 
		 areElementsEqual(board[2]) )
	{
		return true;
	}

	return false;
}

static bool checkColumns(BoardCellValue board[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		if ( board[0][i] != NONE &&
			 board[0][i] == board[1][i] &&
			 board[1][i] == board[2][i] )
		{
			return true;
		}
	}

	return false;
}

static bool checkDiagonals(BoardCellValue board[3][3])
{
	//check first diagonal
	if (board[2][0] != NONE &&
		board[2][0] == board[1][1] &&
		board[1][1] == board[0][2] )
	{
		return true;
	}

	//check second diagonal
	if (board[0][0] != NONE &&
		board[0][0] == board[1][1] &&
		board[1][1] == board[2][2] 	)
	{
		return true;
	}

	return false;

}

bool State::isFinal()
{
	//check lines winner
	if (true == checkLines(_board))
	{
		return true;
	}

	//check columns winner

	if (true == checkColumns(_board))
	{
		return true;
	}

	//check diagonals

	if ( true == checkDiagonals(_board) )
	{
		return true;
	}


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (NONE == _board[i][j])
				return false;
		}
	}

	return true;
}

static Player whoWinsOnLines( BoardCellValue board[3][3] )
{
	if (areElementsEqual(board[0]) )
	{
		if (board[0][0] == X)
			return playerX;
		else
			return playerO;
	}

	else if (areElementsEqual(board[1]))
	{
		if (board[1][1] == X)
			return playerX;
		else
			return playerO;
	}

	else 
	{
		if (board[2][2] == X)
			return playerX;
		else
			return playerO;
	}

}

static Player whoWinsOnColumns(BoardCellValue board[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		if (board[0][i] == board[1][i] &&
			board[1][i] == board[2][i])
		{
			if (board[0][i] == X)
				return playerX;
			else
				return playerO;
		}
	}

	return playerX;
}

static Player whoWinsOnDiagonals(BoardCellValue board[3][3])
{
	//check first diagonal
	if (board[2][0] != NONE &&
		board[2][0] == board[1][1] &&
		board[1][1] == board[0][2])
	{
		if (X == board[2][0])
			return playerX;
		else
			return playerO;
	}

	//check second diagonal
	else if (board[0][0] != NONE &&
		board[0][0] == board[1][1] &&
		board[1][1] == board[2][2])
	{
		if (X == board[0][0])
			return playerX;
		else
			return playerO;
	}
}

char State::getScore(short depth)
{
	if (!isFinal())
		throw "getScore() called on non final state...";

	Player winner = playerX;
	bool isTie = true;

	if (true == checkLines(_board))
	{
		isTie = false;
		if (X == whoWinsOnLines(_board))
			winner = playerX;
		else
			winner = playerO;
			
	}
	else if (checkColumns(_board))
	{
		isTie = false;
		if (X == whoWinsOnColumns(_board))
			winner = playerX;
		else
			winner = playerO;
	}
	else if ( checkDiagonals(_board))
	{
		isTie = false;
		if (X == whoWinsOnDiagonals(_board))
			winner = playerX;
		else
			winner = playerO;
	}

	if (isTie)
		return 0;
	
	if (winner == playerX)
	{
		return (49 / depth) - 1;
	}
	else//playerO wins
	{
		return -(49 / depth) - 1;
	}

	return 0;
}

static std::vector<Position> getEmptyPositions(BoardCellValue board[3][3])
{
	std::vector<Position> ret;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (NONE == board[i][j])
			{
				Position pos;
				pos.i = i;
				pos.j = j;
				ret.push_back(pos);
			}
		}
	}

	return ret;
}

std::vector<State> State::generatePossibleNextStates()
{
	std::vector<State> ret;
	std::vector<Position> emptyPositions = getEmptyPositions(_board);

	for (int i = 0; i < emptyPositions.size(); i++)
	{
		State newState(*this);
		newState.setBoardCellValue(
			_player == playerX ? X : O, emptyPositions[i].i, emptyPositions[i].j);
		newState.setPlayer(_player == playerX ? playerO : playerX);
		ret.push_back(newState);
	}

	return ret;
}

static const char* getSymbolString(BoardCellValue what) {
	switch (what) {
	case X:
		return "X";
	case O:
		return "O";
	case NONE:
		return " ";
	default:
		return "ERROR";
	}
}

void State::printBoard()
{
	for (int i = 0; i < 3; i++)
	{
		std::cout << "| " << getSymbolString(_board[i][0]) << " | ";
		std::cout << "| " << getSymbolString(_board[i][1]) << " | ";
		std::cout << "| " << getSymbolString(_board[i][2]) << " | ";
		std::cout << std::endl;
		std::cout << std::endl;
	}
}
