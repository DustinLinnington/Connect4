#include "stdafx.h"
#include "Negamax.h"
#include <cassert>

Negamax::Negamax()
{
	for (int i = 0; i < Position::WIDTH; i++)
	{
		// Initialize the column exploration order, starting with center column.
		columnOrder[i] = Position::WIDTH / 2 + (1 - 2 * (i % 2))*(i + 1) / 2; 
	}
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
		// Current player can win next move. Return how many moves they can win in, counting from the last move they could make.
		if (pos.IsColumnPlayable(x) && pos.IsWinningMove(x))
		{
			return (Position::WIDTH * Position::HEIGHT + 1 - pos.GetTotalMoves()) / 2;
		}
	}

	int maxScore = (Position::WIDTH * Position::HEIGHT - 1 - pos.GetTotalMoves()) / 2;

	if (beta > maxScore)
	{
		beta = maxScore;
		if (alpha >= beta)
		{
			return beta; // Prune this branch
		}
	}

	for (int x = 0; x < Position::WIDTH; ++x) // Calculate the score of all moves possible next turn and save the highest.
	{
		if (pos.IsColumnPlayable(columnOrder[x]))
		{
			Position nextMove(pos);
			nextMove.PlacePiece(columnOrder[x]);
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