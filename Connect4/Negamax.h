#pragma once
#include <Query.h>
#include "TranspositionTable.h"
#include "MoveSort.h"

class Negamax
{
public:
	Negamax();
	~Negamax();

	// Recursive function to score a board. Returns a number that corresponds to the amount of moves the current
	// player could win in. Negative means they are in a losing position.
	int Solve(const Position &pos, int alpha, int beta, const LARGE_INTEGER startTime, LARGE_INTEGER &currentTime, const LARGE_INTEGER frequency);
	int IterNullWindow(const Position &pos, bool weakSolve, const LARGE_INTEGER startTime, LARGE_INTEGER &currentTime, const LARGE_INTEGER frequency);

	unsigned long long GetNodeCount() { return nodeCount; };
	void SetNodeCount(unsigned long long newCount) { nodeCount = newCount; };
	int columnOrder[Position::WIDTH];

	TranspositionTable transTable;

private:
	unsigned long long nodeCount = 0; // Counts explored nodes.
};

