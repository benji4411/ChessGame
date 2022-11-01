#pragma once
#include "Colors.h"
#include "Graphics.h"
#include "Piece.h"
#include <vector>
#include <memory>
#include <stack>
#include <utility>
#include "PieceSquareTables.h";

struct PossibleMoves {
	Piece* piece;
	std::vector<std::pair<int, int>> moves;
	PossibleMoves(Piece* _piece, std::vector<std::pair<int, int>> _moves) {
		piece = _piece;
		moves = _moves;
	}
};

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
	Board();
	Board(const Board& b);
	~Board();
	void DrawBoard(Graphics& gfx);
	void DrawGrid(Graphics& gfx);
	void DrawPieces(Graphics& gfx);
	void DrawDot(int x, int y, Graphics& gfx);
	void DrawDotAround(int x, int y, Graphics& gfx);
	void HighlightCase(int x, int y, Graphics& gfx);
	void Move(Piece* piece, int x, int y);
	bool UndoMove();
	bool StillLegalMove(bool color);
	Piece* GetPieceAtPos(int _x, int _y);
	Piece* GetDisabledPieceAtPos(int _x, int _y);
	void DeletePiece(Piece* p);
	bool isKingInCheck(bool _color);
	bool isCellAttackedBy(int _x, int _y, bool _color);
	float whiteScore();
	float blackScore();
	bool GameIsOver();
	std::vector<PossibleMoves> AllPossibleMove(bool _color);
private:
	void DrawCase(int x, int y, Color c, Graphics& gfx);
	void DrawNumber(int number, int x, int y, Color c, Graphics& gfx);
	void DrawAlpha(char alpha, int x, int y, Color c, Graphics& gfx);

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

