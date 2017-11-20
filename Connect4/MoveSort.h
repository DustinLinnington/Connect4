#pragma once
#include "Position.h"

class MoveSort
{
public:
	void Add(uint64_t move, int score)
	{
		int pos = size++;
		for (; pos && entries[pos - 1].score > score; --pos) 
			entries[pos] = entries[pos - 1];
		entries[pos].move = move;
		entries[pos].score = score;
	}

	// Get next move
	uint64_t GetNext()
	{
		if (size)
			return entries[--size].move;
		else
			return 0;
	}

	void Reset()
	{
		size = 0;
	}

	MoveSort() : size{ 0 }
	{
	}

private:
	// Number of stored moves
	unsigned int size;

	struct { uint64_t move; int score; } entries[Position::WIDTH];
};
