#include<vector>
#pragma once

enum BoardCellValue
{
	NONE,
	X,
	O
};

enum Player
{
	playerX,
	playerO
};

struct Position {
	int i;
	int j;
};

class State {

public:
	State();

	State(const State&);

private:

	friend class TreeNode;

	Player _player;

	BoardCellValue _board[3][3];
	
	Player getPlayer() const;
	
	Player getOpponent();
	
	void setPlayer(Player player);
	
	void setBoardCellValue(const BoardCellValue value, char i, char j);
	
	BoardCellValue getBoardCellValue(char i, char j);
	
	bool isFinal();

	char getScore(short);

	std::vector<State> generatePossibleNextStates();

	void printBoard();

};