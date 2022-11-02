#pragma once
#include "Colors.h"
#include "Graphics.h"
#include "Piece.h"
#include <vector>
#include <memory>
#include <stack>
#include <utility>
#include "PieceSquareTables.h";

//A struct to store all the possible moves of a piece
struct PossibleMoves {
	Piece* piece;
	std::vector<std::pair<int, int>> moves;
	PossibleMoves(Piece* _piece, std::vector<std::pair<int, int>> _moves) {
		piece = _piece;
		moves = _moves;
	}
};

//A struct to store the information of a move played, useful to undo move
struct MoveData {
	Piece* pieceMoved;
	Piece* pieceDisabled;
	std::pair<int, int> from;
	MoveData(Piece* _pieceMoved, Piece* _pieceDisabled, std::pair<int, int> _from) {
		pieceMoved = _pieceMoved;
		pieceDisabled = _pieceDisabled;
		from = _from;
	}
};

//A struct to store th information of a move to be played, useful to get the best move result of the AI
struct Move {
	Piece* piece = nullptr;
	std::pair<int, int> move;
	Move(Piece* _piece, std::pair<int, int> _move) {
		piece = _piece;
		move = _move;
	}
	Move(){}
};

class Board
{
public:
	Board();//Board class constructor
	Board(const Board& b);//Copy Constructor useful for the AI which will test all moves on another board
	~Board();//Destructor to delete pointers to pieces
	void DrawBoard(Graphics& gfx);//Draw the cells
	void DrawGrid(Graphics& gfx);//Draw the numbers and letters to indicate the position
	void DrawPieces(Graphics& gfx);//Draw the pieces
	void DrawDot(int x, int y, Graphics& gfx);//Draw a dot to indicate the legal moves
	void DrawDotAround(int x, int y, Graphics& gfx);//Draw a circle to indicate pieces that can be capture
	void HighlightCase(int x, int y, Graphics& gfx);//Highlight cases to indicate last move
	void Move(Piece* piece, int x, int y);//Move a piece to a (x,y) position, capture a piece if necessary and promote if pawn on the last row
	bool UndoMove();//Undo the last move, useful for the AI to test moves
	bool StillLegalMove(bool color);//Check if stalemate
	Piece* GetPieceAtPos(int _x, int _y);//Get a piece at a position (x,y)
	Piece* GetDisabledPieceAtPos(int _x, int _y);//Get the disabled piece at a position (x,y) (disabled = captured piece or pawn promoted) 
	void DeletePiece(Piece* p);//Delete a piece, useful to delete a queen when undo promotion
	bool isKingInCheck(bool _color);//Check if the king is attacked
	bool isCellAttackedBy(int _x, int _y, bool _color);//Check if a cell is attacked
	float whiteScore();//return the evaluation of a position for white
	float blackScore();//return the evaluation of a position for black
	bool GameIsOver();//Check if the game is ended by mate or stalemate
	std::vector<PossibleMoves> AllPossibleMove(bool _color);//return all the possible moves for a position
private:
	void DrawCase(int x, int y, Color c, Graphics& gfx);//Draw a cell
	void DrawNumber(int number, int x, int y, Color c, Graphics& gfx);//Draw a number
	void DrawAlpha(char alpha, int x, int y, Color c, Graphics& gfx);//Draw a letter

public:
	int static const width = 8;
	int static const height = 8;

	std::vector< Piece* > pieces;
	King* blackKing;
	King* whiteKing;

	std::stack<MoveData> moves;
public:
	int turnCounter = 0;
	
};

