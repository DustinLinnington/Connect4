#pragma once
#include "Position.h"

class Negamax
{
public:
	Negamax();
	~Negamax();

	// Recursive function to score a board. Returns a number that corresponds to the amount of moves the current
	// player could win in. Negative means they are in a losing position.
	int Solve(const Position &pos, int alpha, int beta);

	unsigned long long GetNodeCount() { return nodeCount; };
	void SetNodeCount(unsigned long long newCount) { nodeCount = newCount; };

private:
	unsigned long long nodeCount = 0; // Counts explored nodes.
	int columnOrder[Position::WIDTH];
};

