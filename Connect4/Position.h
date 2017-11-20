#pragma once
#include <string>
#include <assert.h>

constexpr static uint64_t Bottom(int width, int height)
{
	return width == 0 ? 0 : Bottom(width - 1, height) | 1LL << (width - 1) * (height + 1);
}

class Position
{
public:
	Position() : board { 0 }, height{ 0 }, moves{ 0 } {};
	~Position();

	static const int WIDTH = 7;  // Width of the board
	static const int HEIGHT = 6; // Height of the board
	static const int MIN_SCORE = -(WIDTH * HEIGHT) / 2 + 3;
	static const int MAX_SCORE = (WIDTH * HEIGHT + 1) / 2 - 3;


	static_assert(WIDTH < 10, "Board's width must be less than 10.");
	static_assert(WIDTH*(HEIGHT + 1) <= 64, "Board does not fit in a 64bit bitboard.");

	static uint64_t ComputeWinningPosition(uint64_t position, uint64_t mask)
	{
		// Vertical.
		uint64_t r = (position << 1) & (position << 2) & (position << 3);

		// Horizontal.
		uint64_t p = (position << (HEIGHT + 1)) & (position << 2 * (HEIGHT + 1));
		r |= p & (position << 3 * (HEIGHT + 1));
		r |= p & (position >> (HEIGHT + 1));
		p = (position >> (HEIGHT + 1)) & (position >> 2 * (HEIGHT + 1));
		r |= p & (position << (HEIGHT + 1));
		r |= p & (position >> 3 * (HEIGHT + 1));

		// Diagonal 1.
		p = (position << HEIGHT) & (position << 2 * HEIGHT);
		r |= p & (position << 3 * HEIGHT);
		r |= p & (position >> HEIGHT);
		p = (position >> HEIGHT) & (position >> 2 * HEIGHT);
		r |= p & (position << HEIGHT);
		r |= p & (position >> 3 * HEIGHT);

		// Diagonal 2.
		p = (position << (HEIGHT + 2)) & (position << 2 * (HEIGHT + 2));
		r |= p & (position << 3 * (HEIGHT + 2));
		r |= p & (position >> (HEIGHT + 2));
		p = (position >> (HEIGHT + 2)) & (position >> 2 * (HEIGHT + 2));
		r |= p & (position << (HEIGHT + 2));
		r |= p & (position >> 3 * (HEIGHT + 2));

		return r & (boardMask ^ mask);
	}

	// Returns true if you can play a piece in the column
	bool IsColumnPlayable(int column) const;

	// Plays a piece in the given column
	bool PlacePiece(int column);
	bool PlacePiece(std::string seq);

	// Checks whether a given play would be a winning move if the current player played it
	bool IsWinningMove(int column) const;

	bool CanWinNextTurn() const
	{
		return WinningPosition() & Possible();
	}

	uint64_t PossibleNonLoosingMoves() const 
	{
		uint64_t possibleMask = Possible();
		uint64_t opponentWin = OpponentWinningPosition();
		uint64_t forcedMoves = possibleMask & opponentWin;
		if (forcedMoves) {
			if (forcedMoves & (forcedMoves - 1))
				return 0; 
			else possibleMask = forcedMoves;
		}
		return possibleMask & ~(opponentWin >> 1);
	}

	bool isWinningMove(int col) const{ return WinningPosition() & Possible() & columnMask(col);}  

	uint64_t WinningPosition() const { return ComputeWinningPosition(currentPosition, mask); }

	uint64_t OpponentWinningPosition() const { return ComputeWinningPosition(currentPosition ^ mask, mask); }

	uint64_t Possible() const { return (mask + bottomMask) & boardMask; }

	// Gives the number of moves played since the beginning of the game.
	int GetTotalMoves() const { return moves; };

	// Returns an int corresponding to the current player's turn
	unsigned GetCurrentPlayer() const { return 1 + moves % 2; };

	uint64_t GetKey() const;

	int board[WIDTH][HEIGHT]; // 0 if a cell is empty, 1 for the first player and 2 for the second player.

	int height[WIDTH];        // Number of pieces in each column.
	int moves = 0;       // Number of moves played since the beinning of the game.
private:
	uint64_t currentPosition = 0;
	uint64_t mask = 0;

	static bool Alignment(uint64_t position) 
	{
		// Horizontal.
		uint64_t m = position & (position >> (HEIGHT + 1));
		if (m & (m >> (2 * (HEIGHT + 1)))) 
			return true;

		// Diagonal 1.
		m = position & (position >> HEIGHT);
		if (m & (m >> (2 * HEIGHT))) 
			return true;

		// Diagonal 2. 
		m = position & (position >> (HEIGHT + 2));
		if (m & (m >> (2 * (HEIGHT + 2)))) 
			return true;

		// Vertical.
		m = position & (position >> 1);
		if (m & (m >> 2)) 
			return true;

		return false;
	}

	// Return a bitmask containg a single 1 corresponding to the bottom cell of a given column.
	const static uint64_t bottomMask = Bottom(WIDTH, HEIGHT);
	const static uint64_t boardMask = bottomMask * ((1LL << HEIGHT) - 1);


	// return a bitmask containg a single 1 corresponding to the bottom cell of a given column
	static constexpr uint64_t bottomMaskCol(int column) {
		return UINT64_C(1) << column * (HEIGHT + 1);
	}

	// Return a bitmask containg a single 1 corresponding to the top cell of a given column.
	static uint64_t topMaskCol(int column)
	{
		return (UINT64_C(1) << (HEIGHT - 1)) << column * (HEIGHT + 1);
	}

	// Return a bitmask 1 on all the cells of a given column.
	static uint64_t columnMask(int column) 
	{
		return ((UINT64_C(1) << HEIGHT) - 1) << column * (HEIGHT + 1);
	}
};

