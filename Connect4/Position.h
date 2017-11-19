#pragma once
#include <string>

class Position
{
public:
	Position() : board { 0 }, height{ 0 }, moves{ 0 } {};
	~Position();

	static const int WIDTH = 5;  // Width of the board
	static const int HEIGHT = 4; // Height of the board

	// Returns true if you can play a piece in the column
	bool IsColumnPlayable(int column) const;

	// Plays a piece in the given column
	void PlacePiece(int column);
	unsigned int PlacePiece(std::string seq);

	// Checks whether a given play would be a winning move if the current player played it
	bool IsWinningMove(int column) const;

	// Gives the number of moves played since the beginning of the game.
	unsigned GetTotalMoves() const { return moves; };

private:
	int board[WIDTH][HEIGHT]; // 0 if a cell is empty, 1 for the first player and 2 for the second player.
	int height[WIDTH];        // Number of pieces in each column.
	unsigned int moves;       // Number of moves played since the beinning of the game.
};

