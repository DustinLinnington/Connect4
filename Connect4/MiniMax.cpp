#include "stdafx.h"
#include "MiniMax.h"


MiniMax::MiniMax(int _columns, int _rows)
{
	columns = _columns;
	rows = _rows;
}


MiniMax::~MiniMax()
{
}


int MiniMax::GetMove(int depth, vector<vector<int>> boardState, bool isMax)
{
	int highestScore = -10000;
	int lowestScore = 10000;

	int bestHighMove = 0;
	int bestLowMove = 0;

	if (depth == 0)
	{
		int boardValue = 0;
		for (int i = 0; i < boardState.size(); ++i)
		{
			bool columnFull = true;
			for (int j = 0; j < boardState[i].size(); ++j)
			{
				if (boardState[i][j] == 0)
				{
					columnFull = false;
					if (isMax)
					{
						boardState[i][j] = 1;
					}
					else
					{
						boardState[i][j] = 2;
					}
					break;
				}
			}

			if (!columnFull)
			{
				boardValue = EvaluateBoard(boardState);

				if (boardValue < lowestScore)
				{
					lowestScore = boardValue;
					bestLowMove = i;
				}
				if (boardValue > highestScore)
				{
					highestScore = boardValue;
					bestHighMove = i;
				}
			}
		}

		if (isMax)
		{
			return bestHighMove;
		}
		else
		{
			return bestLowMove;
		}
	}

	return GetMove(depth - 1, boardState, !isMax);
}

int MiniMax::EvaluateBoard(vector<vector<int>> boardState)
{
	int playerOnePotentialWins = 0;
	int playerTwoPotentialWins = 0;

	int pieceOwner = 0;
	int piecesInARow = 0;
	int potentialWinRowSize = 0;
	for (int i = 0; i < columns; ++i)
	{
		for (int j = 0; j < rows; ++j)
		{
			if (boardState[i][j] != 0)
			{
				pieceOwner = boardState[i][j];

				piecesInARow = 0;
				// Check for diagonal win (bottom left to top right)
				int y = j - 3;
				for (int x = i - 3; x < i + 3; ++x, ++y)
				{
					if (x < 0 || y < 0 || x > columns - 1 || y > rows - 1)
					{
						continue;
					}
					else if (piecesInARow == 4)
					{
						// We know that this way is a potential win
						break;
					}

					//if (piecesInARow == 3)
					//{
					//	// Either of these board positions would be winning moves!
					//	if ((y - 1 > 0 && boardState[x][y - 1] != 0) && boardState[x][y] == 0)
					//	{
					//		// TODO: Return a win value
					//	}
					//	else if (y - 1 < 0 && boardState[x][y] == 0)
					//	{
					//		// TODO: Return a win value
					//	}
					//}

					if (boardState[x][y] == pieceOwner)
					{
						piecesInARow++;
						potentialWinRowSize++;
					}
					else if (boardState[x][y] == 0)
					{
						potentialWinRowSize++;
					}
					else
					{
						if (potentialWinRowSize >= 4)
						{
							if (pieceOwner == 1)
							{
								playerOnePotentialWins++;
							}
							else
							{
								playerTwoPotentialWins++;
							}
						}

						piecesInARow = 0;
						potentialWinRowSize = 0;
					}
				}

				piecesInARow = 0;
				// Check for diagonal win (top left to bottom right)
				y = j + 3;
				for (int x = i - 3; x < i + 3; ++x, --y)
				{
					if (x < 0 || y < 0 || x > columns - 1 || y > rows - 1)
					{
						continue;
					}
					if (boardState[x][y] == pieceOwner)
					{
						piecesInARow++;
					}
					else
					{
						piecesInARow = 0;
					}

					if (piecesInARow == 4)
					{
						return pieceOwner;
					}
				}

				piecesInARow = 0;
				// Check for vertical win
				for (int x = j - 3; x < j + 3; ++x)
				{
					if (x < 0 || x > rows - 1)
					{
						continue;
					}
					if (boardState[i][x] == pieceOwner)
					{
						piecesInARow++;
					}
					else
					{
						piecesInARow = 0;
					}

					if (piecesInARow == 4)
					{
						//return pieceOwner;
					}
				}

				piecesInARow = 0;
				// Check for horizontal win
				for (int x = i - 3; x < i + 3; ++x)
				{
					if (x < 0 || x > columns - 1)
					{
						continue;
					}
					if (boardState[x][j] == pieceOwner)
					{
						piecesInARow++;
					}
					else
					{
						piecesInARow = 0;
					}

					if (piecesInARow == 4)
					{
						//return pieceOwner;
					}
				}
			}
		}
	}



	return rand() % 7;
}