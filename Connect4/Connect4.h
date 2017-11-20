#pragma once
#include <vector>
#include <iostream>
#include "MiniMax.h"
#include "Negamax.h"

using namespace std;

class Connect4
{
public:
	Connect4();
	~Connect4();

	// Plays a piece in the given column
	void PlacePiece(int column);
	Position currentBoardPosition;

private:
	void InitiateBoard();
	void StartGame();

	void TakeTurn();
	void TakeAITurn();
	void EndTurn();
	int CheckIfGameOver();

	void PrintBoard();

	vector<vector<int>> gameBoard;
	int currentPlayer = 1;
	int aiPlayer;
	int humanPlayer;

	MiniMax minimaxLogic = MiniMax(7, 6);
	Negamax boardSolver;
};