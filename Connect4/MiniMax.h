#pragma once
#include <vector>

using namespace std;

class MiniMax
{
public:
	MiniMax(int _columns, int _rows);
	~MiniMax();
	int GetMove(int depth, vector<vector<int>> boardState, bool isMax);
private:
	int EvaluateBoard(vector<vector<int>> boardState);
	
	int columns;
	int rows;
};

