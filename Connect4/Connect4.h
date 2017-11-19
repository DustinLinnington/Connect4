#pragma once
#include <vector>
#include <iostream>
#include "MiniMax.h"

using namespace std;

class Connect4
{
public:
	Connect4();
	~Connect4();

	// Plays a piece in the given column
	void PlacePiece(int column);
	unsigned long long GetTime();

private:
	void CreateMap();
	void StartGame();

	void TakeTurn();
	int TakeAITurn();
	void EndTurn();
	int CheckIfGameOver();

	void PrintBoard();

	vector<vector<int>> gameBoard;
	int currentPlayer = 1;
	int aiPlayer;
	int humanPlayer;

	MiniMax minimaxLogic = MiniMax(7, 6);
};