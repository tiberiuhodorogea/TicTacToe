#include"TreeNode.h"
#include<iostream>



void main()
{
	TreeNode * gameNode = new TreeNode();
	Position humanPlayerMove;

	std::cout << "Input your moves as \"'i j\"" << std::endl;
    gameNode->playMove(gameNode->getBestMove());
	std::cout << "Current game state: " << std::endl << std::endl;
	gameNode->printBoard();

	while (!gameNode->isFinalState())
	{
		std::cout << "Yout move: " << std::endl;
		std::cin >> humanPlayerMove.i >> humanPlayerMove.j;
		//adjust input( zero based )
		humanPlayerMove.i -= 1;
		humanPlayerMove.j -= 1;

		if (humanPlayerMove.i < 0 ||
			humanPlayerMove.i>2 || 
			humanPlayerMove.j < 0 || 
			humanPlayerMove.j>2 ||
			!gameNode->isMoveValid(humanPlayerMove) )
		{
			std::cout << "Invalid input, try again: "<<std::endl;
		}
		else//input ok
		{
			gameNode->playMove(humanPlayerMove);
			gameNode->playMove(gameNode->getBestMove());
			std::cout << "Current game state: " << std::endl;
			gameNode->printBoard();
		}
	}


	
}