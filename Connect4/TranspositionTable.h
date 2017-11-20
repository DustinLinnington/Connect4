#pragma once
#include<vector>
#include<cstring>
#include<cassert>

class TranspositionTable
{
public:
	TranspositionTable();
	~TranspositionTable();
public:

	TranspositionTable(unsigned int size) : T(size) 
	{
		assert(size > 0);
	}

	// Fill table with 0's as that means it's empty.
	void Reset()
	{
		memset(&T[0], 0, T.size() * sizeof(Entry));
	}

	void put(uint64_t key, uint8_t val) 
	{
		assert(key < (1LL << 56));
		unsigned int i = index(key); // Compute the index position.
		T[i].key = key;              // Overide any existing value.
		T[i].val = val;
	}

	uint8_t get(uint64_t key) const 
	{
		assert(key < (1LL << 56));
		unsigned int i = index(key);  // Compute the index position
		if (T[i].key == key)
			return T[i].val;            // Return the value if the key matches
		else
			return 0;                   // Return 0 if missing entry
	}

private:
	struct Entry 
	{
		uint64_t key : 56;
		uint8_t val;
	};                  

	std::vector<Entry> T;

	unsigned int index(uint64_t key) const 
	{
		return key%T.size();
	}

};

