#pragma once
#include "Board.h"
#include <utility>

class ChessAI
{
public:
	//Minimax algorithm with alpha-beta pruning
	static std::pair<Move*, float> minimax(Board& board, int depth, float alpha, float beta, bool maximizingPlayer, bool maximizingColor);
private:
	static float evaluate(Board& board, bool maximizingColor);//Return the evaluation of a position

};

