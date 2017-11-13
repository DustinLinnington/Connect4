#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Connect4
{
public:
	Connect4();
	~Connect4();

private:
	void CreateMap();
	void StartGame();

	void TakeTurn();
	void PlacePiece(int column);
	int TakeAITurn();
	void EndTurn();
	int CheckIfGameOver();

	void PrintBoard();

	vector<vector<int>> gameBoard;
	int currentPlayer = 1;
	int aiPlayer;
	int humanPlayer;
};