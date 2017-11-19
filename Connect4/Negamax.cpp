#include "stdafx.h"
#include "Negamax.h"


Negamax::Negamax()
{
}


Negamax::~Negamax()
{
}

int Negamax::Solve(const Position &pos)
{
	nodeCount++;

	if (pos.GetTotalMoves() == Position::WIDTH * Position::HEIGHT) // The game is a draw
	{
		return 0;
	}

	for (int x = 0; x < Position::WIDTH; ++x)
	{
		if (pos.IsColumnPlayable(x) && pos.IsWinningMove(x)) // Current player can win next move
		{
			return (Position::WIDTH * Position::HEIGHT + 1 - pos.GetTotalMoves()) / 2;
		}
	}

	int bestScore = -Position::WIDTH * Position::HEIGHT; // Initialize with the lowest score possible

	for (int x = 0; x < Position::WIDTH; ++x) // Calculate the score of all moves possible next turn and save the highest
	{
		if (pos.IsColumnPlayable(x))
		{
			Position nextMove(pos);
			nextMove.PlacePiece(x);			
			int score = -Solve(nextMove);
			if (score > bestScore)
			{
				bestScore = score;
			}
		}
	}

	return bestScore;
}