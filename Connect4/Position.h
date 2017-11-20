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

	int MoveScore(uint64_t move) const 
	{
		return PopCount(ComputeWinningPosition(currentPosition | move, mask));
	}

	static unsigned int PopCount(uint64_t m) 
	{
		unsigned int c = 0;
		for (c = 0; m; c++) 
			m &= m - 1;
		return c;
	}

	// Returns true if you can play a piece in the column
	bool IsColumnPlayable(int column) const;

	// Plays a piece in the given column
	void PlacePiece(uint64_t move);
	bool PlacePieceColumn(int column);
	bool PlacePiece(std::string seq);

	// Checks whether a given play would be a winning move if the current player played it
	//bool IsWinningMove(int column) const;

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


///*
//* This file is part of Connect4 Game Solver <http://connect4.gamesolver.org>
//* Copyright (C) 2007 Pascal Pons <contact@gamesolver.org>
//*
//* Connect4 Game Solver is free software: you can redistribute it and/or
//* modify it under the terms of the GNU Affero General Public License as
//* published by the Free Software Foundation, either version 3 of the
//* License, or (at your option) any later version.
//*
//* Connect4 Game Solver is distributed in the hope that it will be useful,
//* but WITHOUT ANY WARRANTY; without even the implied warranty of
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//* GNU Affero General Public License for more details.
//*
//* You should have received a copy of the GNU Affero General Public License
//* along with Connect4 Game Solver. If not, see <http://www.gnu.org/licenses/>.
//*/
//#include <string>
//#include <cstdint>
//
//	constexpr static uint64_t bottom(int width, int height) {
//		return width == 0 ? 0 : bottom(width - 1, height) | 1LL << (width - 1)*(height + 1);
//	}
//
//
//	class Position {
//	public:
//
//		static const int WIDTH = 7;  // width of the board
//		static const int HEIGHT = 6; // height of the board
//		static const int MIN_SCORE = -(WIDTH*HEIGHT) / 2 + 3;
//		static const int MAX_SCORE = (WIDTH*HEIGHT + 1) / 2 - 3;
//
//		static_assert(WIDTH < 10, "Board's width must be less than 10");
//		static_assert(WIDTH*(HEIGHT + 1) <= 64, "Board does not fit in 64bits bitboard");
//
//
//		/**
//		* Plays a possible move given by its bitmap representation
//		*
//		* @param move: a possible move given by its bitmap representation
//		*        only one bit of the bitmap should be set to 1
//		*        the move should be a valid possible move for the current player
//		*/
//		void play(uint64_t move)
//		{
//			current_position ^= mask;
//			mask |= move;
//			moves++;
//
//				board[column][height[column]] = 1 + moves % 2;
//				height[column]++;
//			////	moves++;
//		}
//
//		/*
//		* Plays a sequence of successive played columns, mainly used to initilize a board.
//		* @param seq: a sequence of digits corresponding to the 1-based index of the column played.
//		*
//		* @return number of played moves. Processing will stop at first invalid move that can be:
//		*           - invalid character (non digit, or digit >= WIDTH)
//		*           - playing a colum the is already full
//		*           - playing a column that makes an alignment (we only solve non).
//		*         Caller can check if the move sequence was valid by comparing the number of
//		*         processed moves to the length of the sequence.
//		*/
//		unsigned int play(std::string seq)
//		{
//			for (unsigned int i = 0; i < seq.size(); i++) {
//				int col = seq[i] - '1';
//				if (col < 0 || col >= Position::WIDTH || !canPlay(col) || isWinningMove(col)) return i; // invalid move
//				playCol(col);
//			}
//			return seq.size();
//		}
//
//		/*
//		* return true if current player can win next move
//		*/
//		bool canWinNext() const
//		{
//			return winning_position() & possible();
//		}
//
//
//		/**
//		* @return number of moves played from the beginning of the game.
//		*/
//		int nbMoves() const
//		{
//			return moves;
//		}
//
//		/**
//		* @return a compact representation of a position on WIDTH*(HEIGHT+1) bits.
//		*/
//		uint64_t key() const
//		{
//			return current_position + mask;
//		}
//
//		/*
//		* Return a bitmap of all the possible next moves the do not lose in one turn.
//		* A losing move is a move leaving the possibility for the opponent to win directly.
//		*
//		* Warning this function is intended to test position where you cannot win in one turn
//		* If you have a winning move, this function can miss it and prefer to prevent the opponent
//		* to make an alignment.
//		*/
//		uint64_t possibleNonLosingMoves() const {
//			assert(!canWinNext());
//			uint64_t possible_mask = possible();
//			uint64_t opponent_win = opponent_winning_position();
//			uint64_t forced_moves = possible_mask & opponent_win;
//			if (forced_moves) {
//				if (forced_moves & (forced_moves - 1)) // check if there is more than one forced move
//					return 0;                           // the opponnent has two winning moves and you cannot stop him
//				else possible_mask = forced_moves;    // enforce to play the single forced move
//			}
//			return possible_mask & ~(opponent_win >> 1);  // avoid to play below an opponent winning spot
//		}
//
//		/**
//		* Score a possible move.
//		*
//		* @param move, a possible move given in a bitmap format.
//		*
//		* The score we are using is the number of winning spots
//		* the current player has after playing the move.
//		*/
//		int moveScore(uint64_t move) const {
//			return popcount(compute_winning_position(current_position | move, mask));
//		}
//
//		/**
//		* Default constructor, build an empty position.
//		*/
//		Position() : current_position{ 0 }, mask{ 0 }, moves{ 0 } {}
//
//	private:
//		uint64_t current_position; // bitmap of the current_player stones
//		uint64_t mask;             // bitmap of all the already palyed spots
//		unsigned int moves;        // number of moves played since the beinning of the game.
//
//									/**
//									* Indicates whether a column is playable.
//									* @param col: 0-based index of column to play
//									* @return true if the column is playable, false if the column is already full.
//									*/
//		bool canPlay(int col) const
//		{
//			return (mask & top_mask_col(col)) == 0;
//		}
//
//
//		/**
//		* Plays a playable column.
//		* This function should not be called on a non-playable column or a column making an alignment.
//		*
//		* @param col: 0-based index of a playable column.
//		*/
//		void playCol(int col)
//		{
//			play((mask + bottom_mask_col(col)) & column_mask(col));
//		}
//
//
//		/**
//		* Indicates whether the current player wins by playing a given column.
//		* This function should never be called on a non-playable column.
//		* @param col: 0-based index of a playable column.
//		* @return true if current player makes an alignment by playing the corresponding column col.
//		*/
//		bool isWinningMove(int col) const
//		{
//			return winning_position() & possible() & column_mask(col);
//		}
//
//		/*
//		* Return a bitmask of the possible winning positions for the current player
//		*/
//		uint64_t winning_position() const {
//			return compute_winning_position(current_position, mask);
//		}
//
//		/*
//		* Return a bitmask of the possible winning positions for the opponent
//		*/
//		uint64_t opponent_winning_position() const {
//			return compute_winning_position(current_position ^ mask, mask);
//		}
//
//		/*
//		* Bitmap of the next possible valid moves for the current player
//		* Including losing moves.
//		*/
//		uint64_t possible() const {
//			return (mask + bottom_mask) & board_mask;
//		}
//
//		/*
//		* counts number of bit set to one in a 64bits integer
//		*/
//		static unsigned int popcount(uint64_t m) {
//			unsigned int c = 0;
//			for (c = 0; m; c++) m &= m - 1;
//			return c;
//		}
//
//		/*
//		* @parmam position, a bitmap of the player to evaluate the winning pos
//		* @param mask, a mask of the already played spots
//		*
//		* @return a bitmap of all the winning free spots making an alignment
//		*/
//		static uint64_t compute_winning_position(uint64_t position, uint64_t mask) {
//			// vertical;
//			uint64_t r = (position << 1) & (position << 2) & (position << 3);
//
//			//horizontal
//			uint64_t p = (position << (HEIGHT + 1)) & (position << 2 * (HEIGHT + 1));
//			r |= p & (position << 3 * (HEIGHT + 1));
//			r |= p & (position >> (HEIGHT + 1));
//			p = (position >> (HEIGHT + 1)) & (position >> 2 * (HEIGHT + 1));
//			r |= p & (position << (HEIGHT + 1));
//			r |= p & (position >> 3 * (HEIGHT + 1));
//
//			//diagonal 1
//			p = (position << HEIGHT) & (position << 2 * HEIGHT);
//			r |= p & (position << 3 * HEIGHT);
//			r |= p & (position >> HEIGHT);
//			p = (position >> HEIGHT) & (position >> 2 * HEIGHT);
//			r |= p & (position << HEIGHT);
//			r |= p & (position >> 3 * HEIGHT);
//
//			//diagonal 2
//			p = (position << (HEIGHT + 2)) & (position << 2 * (HEIGHT + 2));
//			r |= p & (position << 3 * (HEIGHT + 2));
//			r |= p & (position >> (HEIGHT + 2));
//			p = (position >> (HEIGHT + 2)) & (position >> 2 * (HEIGHT + 2));
//			r |= p & (position << (HEIGHT + 2));
//			r |= p & (position >> 3 * (HEIGHT + 2));
//
//			return r & (board_mask ^ mask);
//		}
//
//		// Static bitmaps
//
//		const static uint64_t bottom_mask = bottom(WIDTH, HEIGHT);
//		const static uint64_t board_mask = bottom_mask * ((1LL << HEIGHT) - 1);
//
//		// return a bitmask containg a single 1 corresponding to the top cel of a given column
//		static constexpr uint64_t top_mask_col(int col) {
//			return UINT64_C(1) << ((HEIGHT - 1) + col*(HEIGHT + 1));
//		}
//
//		// return a bitmask containg a single 1 corresponding to the bottom cell of a given column
//		static constexpr uint64_t bottom_mask_col(int col) {
//			return UINT64_C(1) << col*(HEIGHT + 1);
//		}
//
//	public:
//		// return a bitmask 1 on all the cells of a given column
//		static constexpr uint64_t column_mask(int col) {
//			return ((UINT64_C(1) << HEIGHT) - 1) << col*(HEIGHT + 1);
//		}
//	};