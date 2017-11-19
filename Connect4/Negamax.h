#pragma once
#include "Position.h"

class Negamax
{
public:
	Negamax();
	~Negamax();

	int Solve(const Position &pos);

	unsigned long long GetNodeCount() { return nodeCount; };
	unsigned long long SetNodeCount(unsigned long long newCount) { nodeCount = newCount; };

private:
	unsigned long long nodeCount = 0; // Counts explored nodes.
};

