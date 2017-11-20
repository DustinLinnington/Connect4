#include "stdafx.h"
#include "Position.h"


Position::~Position()
{
}

bool Position::PlacePiece(std::string seq)
{
	for (unsigned int i = 0; i < seq.size(); i++) 
	{
		int column = seq[i] - '1';
		if (!PlacePiece(column))
		{
			return false;
		}
	}
	return true;
}

bool Position::PlacePiece(int column)
{
	if (column < 0 || column >= Position::WIDTH || !IsColumnPlayable(column)) // Invalid move
	{
		return false;
	}
	currentPosition ^= mask;
	mask |= mask + bottomMaskCol(column);
	moves++;

	// For printing the board state.
	board[column][height[column]] = 1 + moves % 2;
	height[column]++;
	return true;
}

bool Position::IsColumnPlayable(int column) const
{
	return (mask & topMaskCol(column)) == 0;
}

uint64_t Position::GetKey() const
{
	return currentPosition + mask;
}

//bool Position::IsWinningMove(int column) const
//{
//	// Check vertical wins.
//	if (height[column] >= 3
//		&& board[column][height[column] - 1] == GetCurrentPlayer()
//		&& board[column][height[column] - 2] == GetCurrentPlayer()
//		&& board[column][height[column] - 3] == GetCurrentPlayer())
//	{
//		return true;
//	}
//
//	for (int y = -1; y <= 1; ++y)
//	{
//		int friendlyPieces = 0;
//		for (int x = -1; x <= 1; x += 2)
//		{
//			for (int i = column + x, j = height[column] + x * y; i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT && board[i][j] == GetCurrentPlayer(); ++friendlyPieces)
//			{
//				i += x;
//				j += x * y;
//			}
//		}
//		if (friendlyPieces >= 3)
//		{
//			return true;
//		}
//	}
//	return false;
//}

//bool Position::PlacePiece(std::string seq)
//{
//	for (unsigned int i = 0; i < seq.size(); i++)
//	{
//		int column = seq[i] - '1';
//		if (!PlacePiece(column))
//		{
//			return false;
//		}
//	}
//	return true;
//}
//
//bool Position::PlacePiece(int column)
//{
//	if (column < 0 || column >= Position::WIDTH || !IsColumnPlayable(column)) // Invalid move
//	{
//		return false;
//	}
//	board[column][height[column]] = 1 + moves % 2;
//	height[column]++;
//	moves++;
//
//	return true;
//}

//bool Position::IsColumnPlayable(int column) const
//{
//	return height[column] < HEIGHT;
//}