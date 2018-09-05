#pragma once
#include<vector>
#include"State.h"

class TreeNode {
private:
	//state
	State _state;

	std::vector<TreeNode*> _children;

	short _depth;

	TreeNode(const State&,short);

	void generateChildren();

	void deleteChildren();

	State& getState();

	void setState(const State&state);

	void addChild(const State& state);

	TreeNode* getChild(const short int& index) const;

	int getChildrenCount() const;
	
	int getValue();

	bool isOpponentPossiblyWinnerNext();

	TreeNode* getBestChild();

public:

	TreeNode();

	~TreeNode();

	Position getBestMove();

	bool isMoveValid(Position move);

	bool playMove(Position move);

	bool isFinalState();

	void printBoard();
};
