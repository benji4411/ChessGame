#pragma once
#include "Graphics.h"
#include "Colors.h"
#include "Sprite.h"
#include <utility>
#include <vector>

#define KING 0
#define QUEEN 1
#define KNIGHT 2
#define ROOK 3
#define BISHOP 4
#define PAWN 5


class Board;

class Piece
{
public:
	Piece() = default;
	Piece(int _x, int _y, bool c, Board* b);
	Piece(int _x, int _y, bool c, Board* b, bool d);
	int GetX();
	int GetY();
	bool GetColor();
	virtual void DrawPiece(Graphics& gfx, int _x, int _y) = 0;
	virtual void DrawPiece(Graphics& gfx) = 0;
	virtual void showLegalMove(Graphics& gfx) = 0;
	virtual bool hasLegalMove() = 0;
	virtual bool isLegalMove(int _x, int _y, bool check) = 0;
	virtual std::vector<std::pair<int, int>>  PossibleMove() = 0;
	virtual int GetType() = 0;
	void MoveTo(int xBoard, int yBoard);
	void Draw(bool state);
protected:
	bool willKingBeInCheck(int xChecked, int yChecked);
protected:
	int x;
	int y;
	Board* board;
	bool color; //Black = false, White = true
	bool draw = true;
public:
	bool disabled = false;

};

class King : public Piece
{
public:
	King(int _x, int _y, bool c, Board* b);
	King(int _x, int _y, bool c, Board* b, bool d);
public:
	void DrawPiece(Graphics& gfx) override;
	void DrawPiece(Graphics& gfx, int _x, int _y) override;
	void showLegalMove(Graphics& gfx) override;
	bool hasLegalMove();
	std::vector<std::pair<int, int>>  PossibleMove() override;
	int GetType() override;
	bool isLegalMove(int _x, int _y, bool check) override;
	void IncreaseMoveCounter();
	void DecreaseMoveCounter();
private:
	int moveCounter = 0;
};
class Queen : public Piece
{
public:
	Queen(int _x, int _y, bool c, Board* b);
	Queen(int _x, int _y, bool c, Board* b, bool d);
public:
	void DrawPiece(Graphics& gfx) override;
	void DrawPiece(Graphics& gfx, int _x, int _y) override;
	void showLegalMove(Graphics& gfx) override;
	std::vector<std::pair<int, int>>  PossibleMove() override;
	int GetType() override;
	bool hasLegalMove();
	bool isLegalMove(int _x, int _y, bool check) override;
};
class Bishop : public Piece
{
public:
	Bishop(int _x, int _y, bool c, Board* b);
	Bishop(int _x, int _y, bool c, Board* b, bool d);
public:
	void DrawPiece(Graphics& gfx) override;
	void DrawPiece(Graphics& gfx, int _x, int _y) override;
	void showLegalMove(Graphics& gfx) override;
	std::vector<std::pair<int, int>>  PossibleMove() override;
	int GetType() override;
	bool hasLegalMove();
	bool isLegalMove(int _x, int _y, bool check) override;
};
class Rook : public Piece
{
public:
	Rook(int _x, int _y, bool c, Board* b);
	Rook(int _x, int _y, bool c, Board* b, bool d);
public:
	void DrawPiece(Graphics& gfx) override;
	void DrawPiece(Graphics& gfx, int _x, int _y) override;
	void showLegalMove(Graphics& gfx) override;
	std::vector<std::pair<int, int>>  PossibleMove() override;
	int GetType() override;
	bool hasLegalMove();
	void IncreaseMoveCounter();
	void DecreaseMoveCounter();
	int GetMoveCounter();
	bool isLegalMove(int _x, int _y, bool check) override;
private:
	int moveCounter = 0;
};
class Knight : public Piece
{
public:
	Knight(int _x, int _y, bool c, Board* b);
	Knight(int _x, int _y, bool c, Board* b, bool d);
public:
	void DrawPiece(Graphics& gfx) override;
	void DrawPiece(Graphics& gfx, int _x, int _y) override;
	void showLegalMove(Graphics& gfx) override;
	std::vector<std::pair<int, int>>  PossibleMove() override;
	int GetType() override;
	bool hasLegalMove();
	bool isLegalMove(int _x, int _y, bool check) override;
};
class Pawn : public Piece
{
public:
	Pawn(int _x, int _y, bool c, Board* b);
	Pawn(int _x, int _y, bool c, Board* b, bool d);
public:
	void DrawPiece(Graphics& gfx) override;
	void DrawPiece(Graphics& gfx, int _x, int _y) override;
	void showLegalMove(Graphics& gfx) override;
	std::vector<std::pair<int, int>>  PossibleMove() override;
	int GetType() override;
	bool hasLegalMove();
	bool isLegalMove(int _x, int _y, bool check) override;
public:
	int enPassantTurn = -1;
};
