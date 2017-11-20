#include "stdafx.h"
#include "Negamax.h"
#include <cassert>

Negamax::Negamax() : transTable(8388593)
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

int Negamax::Solve(const Position &pos, int alpha, int beta, const LARGE_INTEGER startTime, LARGE_INTEGER &currentTime, LARGE_INTEGER frequency)
{
	//QueryPerformanceCounter(&currentTime);
	//double elapsedTime = ((currentTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart) / 1000.0;
	//if (elapsedTime >= 4.9)
	//{
	//	return -10000;
	//}

	assert(alpha < beta);
	assert(!pos.CanWinNextTurn());

	nodeCount++;

	uint64_t next = pos.PossibleNonLoosingMoves();
	if (next == 0)     // Opponent wins next move.
	{
		return -(Position::WIDTH * Position::HEIGHT - pos.GetTotalMoves()) / 2;
	}

	if (pos.GetTotalMoves() >= Position::WIDTH * Position::HEIGHT) // The game is a draw
	{
		return 0;
	}

	int minScore = -(Position::WIDTH * Position::HEIGHT - 2 - pos.GetTotalMoves()) / 2;
	if (alpha < minScore)
	{
		alpha = minScore;
		if (alpha >= beta)
		{
			return alpha;  // Prune!
		}
	}

	int maxScore = (Position::WIDTH * Position::HEIGHT - 1 - pos.GetTotalMoves()) / 2;
	if (int storedValue = transTable.get(pos.GetKey()))
	{
		maxScore = storedValue + Position::MIN_SCORE - 1;
	}

	if (beta > maxScore)
	{
		beta = maxScore;
		if (alpha >= beta)
		{
			return beta; // Prune this branch
		}
	}


	MoveSort moves;
	for (int i = Position::WIDTH; --i;)
	{
		if (uint64_t move = next & Position::columnMask(columnOrder[i]))
		{
			moves.Add(move, pos.MoveScore(move));
		}
	}
	while (uint64_t next = moves.GetNext()) // Calculate the score of all moves possible next turn and save the highest.
	{
		Position nextMove(pos);
		nextMove.PlacePiece(next);
		int score = -Solve(nextMove, -beta, -alpha, startTime, currentTime, frequency);
		if (score == -10000 || score == 10000) // Time has run out
		{
			return score;
		}

		if (score >= beta)
		{
			return score;
		}
		if (score > alpha)
		{
			alpha = score;
		}
	}
	transTable.put(pos.GetKey(), alpha - Position::MIN_SCORE + 1); // save the upper bound of the position
	return alpha;
}

int Negamax::IterNullWindow(const Position &pos, bool weakSolve, const LARGE_INTEGER startTime, LARGE_INTEGER &currentTime, const LARGE_INTEGER frequency)
{
	if (pos.CanWinNextTurn())
		return (Position::WIDTH*Position::HEIGHT + 1 - pos.GetTotalMoves()) / 2;

	int min = -(Position::WIDTH * Position::HEIGHT - pos.GetTotalMoves()) / 2;
	int max = (Position::WIDTH * Position::HEIGHT + 1 - pos.GetTotalMoves()) / 2;
	if (weakSolve)
	{
		min = -1;
		max = 1;
	}

	while (min < max) // Iterative deepening
	{
		int med = min + (max - min) / 2;
		if (med <= 0 && min / 2 < med) 
			med = min / 2;
		else if (med >= 0 && max / 2 > med) 
			med = max / 2;
		int r = Solve(pos, med, med + 1, startTime, currentTime, frequency);   // Null depth window;
		if (r <= -10000 || r >= 10000)
		{
			return r;
		}

		if (r <= med)
		{
			max = r;
		}
		else min = r;
	}
	return min;
}

//int Negamax::Solve(const Position &pos, int alpha, int beta, const LARGE_INTEGER startTime, LARGE_INTEGER &currentTime, LARGE_INTEGER frequency)
//{
//	//QueryPerformanceCounter(&currentTime);
//	//double elapsedTime = ((currentTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart) / 1000.0;
//	//if (elapsedTime >= 4.9)
//	//{
//	//	return -10000;
//	//}
//
//	assert(alpha < beta);
//	assert(!pos.CanWinNextTurn());
//
//	nodeCount++;
//
//	uint64_t next = pos.PossibleNonLoosingMoves();
//	if (next == 0)     // Opponent wins next move.
//	{
//		return -(Position::WIDTH * Position::HEIGHT - pos.GetTotalMoves()) / 2;
//	}
//
//	if (pos.GetTotalMoves() >= Position::WIDTH * Position::HEIGHT) // The game is a draw
//	{
//		return 0;
//	}
//
//	int minScore = -(Position::WIDTH * Position::HEIGHT - 2 - pos.GetTotalMoves()) / 2;
//	if (alpha < minScore)
//	{
//		alpha = minScore;
//		if (alpha >= beta)
//		{
//			return alpha;  // Prune!
//		}
//	}
//
//	int maxScore = (Position::WIDTH * Position::HEIGHT - 1 - pos.GetTotalMoves()) / 2;
//	if (int storedValue = transTable.get(pos.GetKey()))
//	{
//		maxScore = storedValue + Position::MIN_SCORE - 1;
//	}
//
//	if (beta > maxScore)
//	{
//		beta = maxScore;
//		if (alpha >= beta)
//		{
//			return beta; // Prune this branch
//		}
//	}
//
//
//	MoveSort moves;
//	for (int i = Position::WIDTH; --i;)
//	{
//		if (uint64_t move = next & Position::columnMask(columnOrder[i]))
//		{
//			moves.Add(move, pos.MoveScore(move));
//		}
//	}
//	while (uint64_t next = moves.GetNext()) // Calculate the score of all moves possible next turn and save the highest.
//	{
//		Position nextMove(pos);
//		nextMove.PlacePiece(next);
//		int score = -Solve(nextMove, -beta, -alpha, startTime, currentTime, frequency);
//		if (score == -10000 || score == 10000) // Time has run out
//		{
//			return score;
//		}
//
//		if (score >= beta)
//		{
//			return score;
//		}
//		if (score > alpha)
//		{
//			alpha = score;
//		}
//	}
//	transTable.put(pos.GetKey(), alpha - Position::MIN_SCORE + 1); // save the upper bound of the position
//	return alpha;
//}
//
//int Negamax::IterNullWindow(const Position &pos, bool weakSolve, const LARGE_INTEGER startTime, LARGE_INTEGER &currentTime, const LARGE_INTEGER frequency)
//{
//	if (pos.CanWinNextTurn())
//		return (Position::WIDTH*Position::HEIGHT + 1 - pos.GetTotalMoves()) / 2;
//
//	int min = -(Position::WIDTH * Position::HEIGHT - pos.GetTotalMoves()) / 2;
//	int max = (Position::WIDTH * Position::HEIGHT + 1 - pos.GetTotalMoves()) / 2;
//	if (weakSolve)
//	{
//		min = -1;
//		max = 1;
//	}
//
//
//	while (min < max) // Iterative deepening
//	{
//		int med = min + (max - min) / 2;
//		if (med <= 0 && min / 2 < med) med = min / 2;
//		else if (med >= 0 && max / 2 > med) med = max / 2;
//		int r = Solve(pos, med, med + 1, startTime, currentTime, frequency);   // Null depth window;
//		if (r <= -10000 || r >= 10000)
//		{
//			return r;
//		}
//
//		if (r <= med)
//		{
//			max = r;
//		}
//		else min = r;
//	}
//	return min;
//}