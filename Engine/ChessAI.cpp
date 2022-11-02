#include "ChessAI.h"

//Minimax algorithm with alpha-beta pruning
std::pair<Move*, float> ChessAI::minimax(Board& board, int depth,float alpha, float beta, bool maximizingPlayer, bool maximizingColor)
{
    if(depth == 0 || board.GameIsOver())
        return std::make_pair(nullptr,evaluate(board, maximizingColor));

    auto possibleMoves = board.AllPossibleMove(maximizingPlayer);

    Move *bestMove;
    if (maximizingPlayer) {
        float maxEval = INFINITY;
        for (PossibleMoves& p : possibleMoves) {
            for (auto& move : p.moves) {
                board.Move(p.piece, move.first,move.second);
                float currentEval = minimax(board, depth - 1, alpha, beta, false, maximizingColor).second;
                board.UndoMove();
                if (currentEval < maxEval) {
                    maxEval = currentEval;
                    bestMove = new Move(p.piece, move);
                }
                beta = min(beta, currentEval);
                if (beta <= alpha)
                    break;
            }
        }
        return std::make_pair(bestMove,maxEval);
    }
    else {
        float maxEval = -INFINITY;
        for (PossibleMoves& p : possibleMoves) {
            for (auto& move : p.moves) {
                board.Move(p.piece, move.first, move.second);
                float currentEval = minimax(board, depth - 1, alpha, beta, true, maximizingColor).second;
                board.UndoMove();
                if (currentEval > maxEval) {
                    maxEval = currentEval;
                    bestMove = new Move(p.piece, move);
                }
                alpha = max(alpha, currentEval);
                if (beta <= alpha)
                    break;
            }
        }
        return std::make_pair(bestMove, maxEval);
    }
}

//Return the evaluation of a position
float ChessAI::evaluate(Board& board, bool maximizingColor)
{
    if (maximizingColor)
        return board.whiteScore() - board.blackScore();
    else
        return board.blackScore() - board.whiteScore();
}

