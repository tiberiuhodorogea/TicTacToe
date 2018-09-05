#include"TreeNode.h"

TreeNode::TreeNode(const State & state, short depth)
{
	_state = State(state);
	_depth = depth;
	generateChildren();
}

TreeNode::TreeNode()
{
	//initial state, before first move
	_depth = 0;
}

TreeNode::~TreeNode()
{
	deleteChildren();
}

State & TreeNode::getState()
{
	return _state;
}

void TreeNode::setState(const State & state)
{
	_state = state;
}

void TreeNode::addChild(const State & state)
{
	_children.push_back(
		new TreeNode(state,_depth + 1)
	);
}

TreeNode * TreeNode::getChild(const short int & index) const
{
	return _children[index];
}

int TreeNode::getChildrenCount() const
{
	return _children.size();
}

void TreeNode::generateChildren()
{
	deleteChildren();

	std::vector<State> possibleNextStates = _state.generatePossibleNextStates();
	
	for (int i = 0; i < possibleNextStates.size(); i++)
	{
		addChild(possibleNextStates[i]);
	}
}

void TreeNode::deleteChildren()
{
	for (int i = 0; i < _children.size(); i++)
	{
		delete _children[i];
	}
	_children.clear();
}

static Position boardFirstDifferencePosition(BoardCellValue board1[3][3], BoardCellValue board2[3][3]) {

	for(int i=0;i<3;i++)
		for (int j = 0; j < 3; j++) 
		{
			if (board1[i][j] != board2[i][j])
			{
				Position ret;
				ret.i = i;
				ret.j = j;
				return ret;
			}
		}
	throw "No difference in the boards...some problem";
	
}

Position TreeNode::getBestMove()
{
	TreeNode * bestChild = NULL;
	generateChildren();
	bestChild = getBestChild();
	//bestChild = minimaxBestChild();
	return boardFirstDifferencePosition(_state._board,bestChild->getState()._board);
}

//true == success
//false == move not possible
bool TreeNode::playMove(Position move)
{
	if (!isMoveValid(move))
	{
		return false;
	}

	_state.setBoardCellValue(_state.getPlayer() == playerX ? X : O, move.i, move.j);
	_state.setPlayer(_state.getOpponent());	

	return true;
}

bool TreeNode::isMoveValid(Position move) 
{
	if (NONE != _state.getBoardCellValue(move.i, move.j))
	{
		return false;
	}

	return true;
}

bool TreeNode::isFinalState()
{
	return _state.isFinal();
}

static bool isIndexInVector(int index, std::vector<short> vector)
{
	bool ret = false;
	for (int i = 0; i < vector.size(); i++)
	{
		if (index == vector[i]);
		ret = true;
	}
	return ret;
}

static short getIndexForMaxValue(int* vector, int dim, std::vector<short> excludedIndexes) {
	short ret = 0;
	int maxValue = vector[0];

	for (int i = 1; i < dim; i++)
	{
		if (maxValue < vector[i] &&
			!isIndexInVector(i,excludedIndexes))
		{
			maxValue = vector[i];
			ret = i;
		}
	}
	return ret;
}

static short getIndexForMinValue(int* vector, int dim) {
	short ret = 0;
	int minValue = vector[0];

	for (int i = 1; i < dim; i++)
	{
		if (minValue > vector[i])
		{
			minValue = vector[i];
			ret = i;
		}
	}
	return ret;
}

bool TreeNode::isOpponentPossiblyWinnerNext()
{
	bool ret = false;
	for (int i = 0; i < _children.size(); i++)
	{
		if (_children[i]->getState().isFinal() &&
			_children[i]->getState().getScore(1) > 0)
		{
			ret = true;
		}
	}
	
	return ret;
}

TreeNode * TreeNode::getBestChild()
{
	int bestChildIndex = 0;
	int * valuesOfChildren = new int[_children.size()];
	if (playerX == _state.getPlayer())//computer is playerX
	{
		
		for (int i = 0; i < _children.size(); i++)
		{
			valuesOfChildren[i] = _children[i]->getValue();
		}

		bestChildIndex = getIndexForMinValue(valuesOfChildren, _children.size());
	}
	else//playerO is computer
	{
		std::vector<short> excludedIndexes;
		for (int i = 0; i < _children.size(); i++)
		{
			if (_children[i]->isOpponentPossiblyWinnerNext())
			{
				excludedIndexes.push_back(i);
			}
		}

		for (int i = 0; i < _children.size(); i++)
		{
			valuesOfChildren[i] = _children[i]->getValue();
		}

		bestChildIndex = getIndexForMaxValue(valuesOfChildren, _children.size(),excludedIndexes);
	}

	delete[] valuesOfChildren;
	return _children[bestChildIndex];
}


int TreeNode::getValue()
{
	if (isFinalState())
	{
		return getState().getScore(_depth);
	}
	//else return sumscore of chidren
	int sumScoreOfChildren = 0;
	for (int i = 0; i < _children.size(); i++)
	{
		sumScoreOfChildren += _children[i]->getValue();
	}

	return sumScoreOfChildren;
}

void TreeNode::printBoard()
{
	return _state.printBoard();
}
