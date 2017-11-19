#include "stdafx.h"
#include "Negamax.h"
#include <cassert>

Negamax::Negamax()
{
}


Negamax::~Negamax()
{
}

int Negamax::Solve(const Position &pos, int alpha, int beta)
{
	assert(alpha < beta);
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

	int maxScore = (Position::WIDTH * Position::HEIGHT + 1 - pos.GetTotalMoves()) / 2;

	if (beta > maxScore)
	{
		beta = maxScore;
		if (alpha >= beta)
		{
			return beta;
		}
	}

	for (int x = 0; x < Position::WIDTH; ++x) // Calculate the score of all moves possible next turn and save the highest
	{
		if (pos.IsColumnPlayable(x))
		{
			Position nextMove(pos);
			nextMove.PlacePiece(x);			
			int score = -Solve(nextMove, -beta, -alpha);
			if (score >= beta)
			{
				return score;
			}
			if (score > alpha)
			{
				alpha = score;
			}
		}
	}

	return alpha;
}