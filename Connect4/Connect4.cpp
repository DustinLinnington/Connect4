// Connect4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Connect4.h"
#include <Query.h>
#include <sstream>
#include <iomanip>

//#define ROWS 6
//#define COLUMNS 7
//#define DEPTH 2

using namespace std;

int main(int ars, char* argv[])
{
	Connect4 newGame = Connect4();

	return 0;
}

Connect4::Connect4()
{
	StartGame();
}

Connect4::~Connect4()
{

}

void Connect4::InitiateBoard()
{
	currentBoardPosition = Position();
	boardSolver = Negamax();
}

//void Connect4::StartGame()
//{
//	Negamax boardSolver = Negamax();
//	string line;
//
//	for (int l = 1; getline(std::cin, line); l++) {
//		Position pos;
//		if (pos.PlacePiece(line) != line.size())
//		{
//			cerr << "Line << " << l << ": Invalid move " << (pos.GetTotalMoves() + 1) << " \"" << line << "\"" << endl;
//		}
//		else
//		{
//			LARGE_INTEGER frequency;
//			LARGE_INTEGER startTime, endTime;
//			double elapsedTime;
//
//			QueryPerformanceFrequency(&frequency);
//			QueryPerformanceCounter(&startTime);
//
//			int score = boardSolver.Solve(pos, -1, 1);
//			//int score = boardSolver.Solve(pos, -Position::WIDTH*Position::HEIGHT / 2, Position::WIDTH*Position::HEIGHT / 2);
//
//			
//			QueryPerformanceCounter(&endTime);
//			elapsedTime = ((endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart) / 1000.0;
//
//			cout << line << " " << score << " " << boardSolver.GetNodeCount() << " " << elapsedTime;
//		}
//		cout << endl;
//	}
//}

void Connect4::StartGame()
{
	InitiateBoard();

	cout << "Welcome to Connect4!" << endl;
	bool validInput = false;
	for (string line; cout << "Would you like to go first? (y/n)" << endl && getline(cin, line); )
	{
		if (line == "y" || line == "Y" )
		{ 
			validInput = true; 
			aiPlayer = 2;
			humanPlayer = 1;
			break; 
		}
		else if (line == "n" || line == "N")
		{
			validInput = true;
			aiPlayer = 1;
			humanPlayer = 2;
			break;
		}
		cerr << "I'm sorry, that was not an option." << endl;
	}
	if (!validInput) 
	{ 
		cerr << "Premature end of input.\n"; 
	}

	PrintBoard();
	TakeTurn();
}



void Connect4::PrintBoard()
{
	for (int i = Position::HEIGHT - 1; i >= 0; --i)
	{
		for (int j = 0; j < Position::WIDTH; ++j)
		{
			int playerPiece = currentBoardPosition.board[j][i];
			char* output = "|   ";
			if (playerPiece == 1)
			{
				output = "| X ";
			}
			else if (playerPiece == 2)
			{
				output = "| O ";
			}

			cout << output;
		}

		cout << "|" << endl;
	}

	cout << endl << endl;
}

void Connect4::TakeTurn()
{
	int columnChoice = 0;
	if (currentBoardPosition.GetCurrentPlayer() == humanPlayer)	
	{
		TakeAITurn();

		//string line;
		//for (int l = 1; cout << "Select which column to place your piece in: " << endl && getline(std::cin, line); l++) 
		//{
		//	line = line[0];
		//	if (!currentBoardPosition.PlacePiece(line))
		//	{
		//		cerr << "Invalid move, please select a column number from 1-7." << endl;
		//	}
		//	else
		//	{
		//		break;
		//	}
		//}
	}
	else
	{
		TakeAITurn();
		cout << "           AI Move" << endl;
	}

	if (currentBoardPosition.moves == Position::WIDTH * Position::HEIGHT)
	{
		cout << "Game was a draw!" << endl;
		getchar();
		exit(0);
	}

	EndTurn();
}

void Connect4::TakeAITurn()
{
	//Position nextPos(currentBoardPosition);
	//nextPos.board[0][0] = 2;
	//nextPos.board[0][1] = 2;
	//nextPos.board[0][2] = 2;
	//nextPos.board[1][0] = 1;
	//nextPos.board[1][1] = 1;
	//nextPos.board[1][2] = 1;
	//nextPos.height[0] = 3;
	//nextPos.height[1] = 3;
	//nextPos.moves = 6;

	//for (int i = 0; i < Position::WIDTH; ++i)
	//{
	//	Position nextNextPos(nextPos);
	//	nextNextPos.PlacePiece(i);
	//	int score = -boardSolver.Solve(nextPos, -Position::WIDTH * Position::HEIGHT / 2, Position::WIDTH * Position::HEIGHT / 2);
	//}

	LARGE_INTEGER frequency;
	LARGE_INTEGER startTime, endTime;
	double elapsedTime;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);

	int bestScore = -1000;
	int bestMove = 0;
	bool outOfTime = false;
	for (int i = 0; i < Position::WIDTH; ++i)
	{
		boardSolver.SetNodeCount(0);
		//boardSolver.transTable.Reset();

		Position nextPosition(currentBoardPosition);
		if (!nextPosition.PlacePiece(i))
		{
			continue;
		} 
		int score = -boardSolver.IterNullWindow(nextPosition, false, startTime, endTime, frequency);
		//int score = -boardSolver.IterNullWindow(nextPosition, -Position::WIDTH * Position::HEIGHT / 2, Position::WIDTH * Position::HEIGHT / 2, startTime, endTime, frequency);
		//int score = -boardSolver.IterNullWindow(nextPosition, -6, 6, startTime, endTime, frequency);

		if (score == -10000 || score == 10000)
		{
			outOfTime = true;
			break;
		}
		if (score > bestScore)
		{
			bestScore = score;
			bestMove = i;
		}
	}
	QueryPerformanceCounter(&endTime);
	elapsedTime = ((endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart) / 1000.0;

	if (outOfTime)
	{
		for (int i = 0; i < Position::WIDTH; ++i)
		{
			Position newPosition(currentBoardPosition);

			if (currentBoardPosition.IsColumnPlayable(boardSolver.columnOrder[i]))
			{
				bestMove = boardSolver.columnOrder[i];
				break;
			}
		}
	}

	//if (currentBoardPosition.IsWinningMove(bestMove))
	//{
	//	cout << "Game over, AI wins!" << endl;
	//	getchar();
	//	exit(0);
	//}

	currentBoardPosition.PlacePiece(bestMove);
	elapsedTime = floor(elapsedTime * 1000) / 1000;
	cout << "AI move took " <<  elapsedTime << " seconds." << endl;
}

void Connect4::EndTurn()
{
	PrintBoard();
	int winner = CheckIfGameOver(currentBoardPosition);

	if (winner != 0)
	{
		cout << "Game over! Player ";
		cout << winner;
		cout << " won!" << endl;
		cout << "Enter character to quit.";
		getchar();
		exit(0);
	}

	TakeTurn();
}

int Connect4::CheckIfGameOver(Position pos)
{
	int pieceOwner = 0;
	int piecesInARow = 0;
	for (int i = 0; i < Position::WIDTH; ++i)
	{
		for (int j = 0; j < Position::HEIGHT; ++j)
		{
			if (pos.board[i][j] != 0)
			{
				pieceOwner = pos.board[i][j];

				piecesInARow = 0;
				// Check for diagonal win (bottom left to top right)
				int y = j - 3;
				for (int x = i-3; x < i+3; ++x, ++y)
				{
					if (x < 0 || y < 0 || x > Position::WIDTH - 1 || y > Position::HEIGHT - 1)
					{
						continue;
					}
					if (pos.board[x][y] == pieceOwner)
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
				// Check for diagonal win (top left to bottom right)
				y = j + 3;
				for (int x = i - 3; x < i + 3; ++x, --y)
				{
					if (x < 0 || y < 0 || x > Position::WIDTH - 1 || y > Position::HEIGHT - 1)
					{
						continue;
					}
					if (pos.board[x][y] == pieceOwner)
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
					if (x < 0 || x > Position::HEIGHT - 1)
					{
						continue;
					}
					if (pos.board[i][x] == pieceOwner)
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
				// Check for horizontal win
				for (int x = i - 3; x < i + 3; ++x)
				{
					if (x < 0  || x > Position::WIDTH - 1)
					{
						continue;
					}
					if (pos.board[x][j] == pieceOwner)
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
			}
		}
	}

	return 0;
}