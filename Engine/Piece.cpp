#include "Piece.h"
#include "Board.h"
#include <math.h>
#include <assert.h>


Piece::Piece(int _x, int _y, bool c, Board* b)
	:
	x(_x),
	y(_y),
	color(c),
	board(b)
{
}

Piece::Piece(int _x, int _y, bool c, Board* b, bool d)
	:
	x(_x),
	y(_y),
	color(c),
	board(b),
	disabled(d)
{
}

int Piece::GetX()
{
	return x;
}

int Piece::GetY()
{
	return y;
}

bool Piece::GetColor() {
	return color;
}

//Check if his king will be attacked after one of his piece move to (xChecked,yChecked)
bool Piece::willKingBeInCheck(int xChecked, int yChecked)
{
	int xTemp = this->x;
	int yTemp = this->y;

	Piece* p = board->GetPieceAtPos(xChecked, yChecked);
	if(p != nullptr)
		p->disabled = true;

	this->x = xChecked;
	this->y = yChecked;

	bool result = board->isKingInCheck(color);

	this->x = xTemp;
	this->y = yTemp;

	if (p != nullptr)
		p->disabled = false;

	return result;
}

void Piece::MoveTo(int xBoard, int yBoard)
{
	assert(xBoard >= 0);
	assert(xBoard <= 7);
	assert(yBoard >= 0);
	assert(yBoard <= 7);
	x = xBoard;
	y = yBoard;
}

void Piece::Draw(bool state)
{
	draw = state;
}

King::King(int _x, int _y, bool c, Board* b)
	:
	Piece(_x, _y,c,b)
{
}

King::King(int _x, int _y, bool c, Board* b, bool d)
	:
	Piece(_x, _y, c, b, d)
{
}

void King::DrawPiece(Graphics& gfx)
{
	if (draw == true) {
		char const* king = color ? &whiteKing[0][0] : &blackKing[0][0];
		int const squareSize = 90;
		int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
		int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
		const Color black = { 0,0,0 };
		const Color white = { 250,250,250 };
		for (int i = 0; i < 90; i++)
			for (int j = 0; j < 90; j++) {
				if (king[i + j*90] == -1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, black);
				else if (king[i + j * 90] == 1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, white);
			}
	}
}

void King::DrawPiece(Graphics& gfx, int _x, int _y)
{
	int const squareSize = 90;


	const int left = _x - squareSize / 2;
	const int right = _x + squareSize / 2;
	const int top = _y - squareSize / 2;
	const int bottom = _y + squareSize / 2;

	if (right >= Graphics::ScreenWidth)
		_x = Graphics::ScreenWidth - squareSize / 2;
	else if (left < 0)
		_x = squareSize / 2;
	if (bottom >= Graphics::ScreenHeight)
		_y = Graphics::ScreenHeight - squareSize / 2;
	else if (top < 0)
		_y = squareSize / 2;

	const char* king = color ? &whiteKing[0][0] : &blackKing[0][0];

	int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
	int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
	const Color black = { 0,0,0 };
	const Color white = { 250,250,250 };
	for (int i = 0; i < 90; i++)
		for (int j = 0; j < 90; j++) {
			if (king[i + j * 90] == -1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, black);
			else if (king[i + j * 90] == 1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, white);
		}
}

void King::showLegalMove(Graphics& gfx)
{
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if ((dx != 0 || dy != 0) && dx + x <= 7 && dx + x >= 0 && dy + y <= 7 && dy + y >= 0) {
				if (isLegalMove(x + dx, y + dy, true) && board->GetPieceAtPos(x + dx, y + dy) == nullptr)
					board->DrawDot(x + dx, y + dy, gfx);
				else if (isLegalMove(x + dx, y + dy, true)) {
					board->DrawDotAround(x + dx, y + dy, gfx);
				}
			}
		}
	if (moveCounter == 0) {
		if (isLegalMove(6, y,true)) 
			board->DrawDot(6, y, gfx);
		if(isLegalMove(2, y, true))
			board->DrawDot(2, y, gfx);
	}
}

bool King::hasLegalMove()
{
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if ((dx != 0 || dy != 0) && dx + x <= 7 && dx + x >= 0 && dy + y <= 7 && dy + y >= 0) {
				if (isLegalMove(x + dx, y + dy, true))
					return true;
			}
		}
	return false;
}

std::vector<std::pair<int, int>> King::PossibleMove()
{
	std::vector<std::pair<int, int>> result;

	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if ((dx != 0 || dy != 0) && dx + x <= 7 && dx + x >= 0 && dy + y <= 7 && dy + y >= 0) {
				if (isLegalMove(x + dx, y + dy, true))
					result.push_back(std::make_pair(x + dx, y + dy));
			}
		}
	return result;
}

int King::GetType()
{
	return KING;
}

bool King::isLegalMove(int _x, int _y, bool check)
{
	if (_x < 0 || _x > 7 || _y < 0 || _y > 7 || (_x == x && _y == y))
		return false;

	if (moveCounter == 0) {
		if (_x == 6 && _y == 7 * color && board->GetPieceAtPos(6, _y) == nullptr && board->GetPieceAtPos(5, _y) == nullptr
			&& board->GetPieceAtPos(7, _y) != nullptr && board->GetPieceAtPos(7, _y)->GetType() == ROOK && ((Rook*)board->GetPieceAtPos(7, _y))->GetMoveCounter() == 0
			&& !board->isCellAttackedBy(6, _y, !color) && !board->isCellAttackedBy(5, _y, !color) && !board->isKingInCheck(color))
			return true;
		else if (_x == 2 && _y == 7 * color && board->GetPieceAtPos(2, _y) == nullptr && board->GetPieceAtPos(3, _y) == nullptr && board->GetPieceAtPos(1, _y) == nullptr
			&& board->GetPieceAtPos(0, _y) != nullptr && board->GetPieceAtPos(0, _y)->GetType() == ROOK && ((Rook*)board->GetPieceAtPos(0, _y))->GetMoveCounter() == 0
			&& !board->isCellAttackedBy(2, _y, !color) && !board->isCellAttackedBy(3, _y, !color) && !board->isKingInCheck(color))
			return true;
	}
	if (!((board->GetPieceAtPos(_x, _y) == nullptr || board->GetPieceAtPos(_x, _y)->GetColor() != color) && abs(x - _x) <= 1 && abs(y - _y) <= 1))
		return false;
	if (check && willKingBeInCheck(_x, _y))
		return false;

		return true;
}

void King::IncreaseMoveCounter()
{
	++moveCounter;
}

void King::DecreaseMoveCounter()
{
	if(moveCounter > 0)
		--moveCounter;
}


Queen::Queen(int _x, int _y, bool c, Board* b)
	:
	Piece(_x, _y, c,b)
{
}

Queen::Queen(int _x, int _y, bool c, Board* b, bool d)
	:
	Piece(_x, _y, c, b, d)
{
}

void Queen::DrawPiece(Graphics& gfx)
{
	if (draw == true) {
		char const* queen = color ? &whiteQueen[0][0] : &blackQueen[0][0];
		int const squareSize = 90;
		int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
		int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
		const Color black = { 0,0,0 };
		const Color white = { 250,250,250 };
		for (int i = 0; i < 90; i++)
			for (int j = 0; j < 90; j++) {
				if (queen[i + j * 90] == -1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, black);
				else if (queen[i + j * 90] == 1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, white);
			}
	}
}

void Queen::DrawPiece(Graphics& gfx, int _x, int _y)
{
	int const squareSize = 90;


	const int left = _x - squareSize / 2;
	const int right = _x + squareSize / 2;
	const int top = _y - squareSize / 2;
	const int bottom = _y + squareSize / 2;

	if (right >= Graphics::ScreenWidth)
		_x = Graphics::ScreenWidth - squareSize / 2;
	else if (left < 0)
		_x = squareSize / 2;
	if (bottom >= Graphics::ScreenHeight)
		_y = Graphics::ScreenHeight - squareSize / 2;
	else if (top < 0)
		_y = squareSize / 2;

	const char* queen = color ? &whiteQueen[0][0] : &blackQueen[0][0];

	int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
	int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
	const Color black = { 0,0,0 };
	const Color white = { 250,250,250 };
	for (int i = 0; i < 90; i++)
		for (int j = 0; j < 90; j++) {
			if (queen[i + j * 90] == -1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, black);
			else if (queen[i + j * 90] == 1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, white);
		}
}
void Queen::showLegalMove(Graphics& gfx)
{
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if((dx != 0 || dy != 0)){
				int i = dx;
				int j = dy;
				while (i + x <= 7 && i + x >= 0 && j + y <= 7 && j + y >= 0) {
					if (isLegalMove(x + i, y + j, true) && board->GetPieceAtPos(x + i, y + j) == nullptr)
						board->DrawDot(x + i, y + j, gfx);
					else if (isLegalMove(x + i, y + j, true) && board->GetPieceAtPos(x + i, y + j)->GetColor() != color) {
						board->DrawDotAround(x + i, y + j, gfx);
						break;
					}

					j += dy;
					i += dx;
				}
			}
		}
}

std::vector<std::pair<int, int>> Queen::PossibleMove()
{
	std::vector<std::pair<int, int>> result;

	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if ((dx != 0 || dy != 0)) {
				int i = dx;
				int j = dy;
				while (i + x <= 7 && i + x >= 0 && j + y <= 7 && j + y >= 0) {
					if (isLegalMove(x + i, y + j, true))
						result.push_back(std::make_pair(x + i, y + j));

					j += dy;
					i += dx;
				}
			}
		}
	return result;
}

int Queen::GetType()
{
	return QUEEN;
}

bool Queen::hasLegalMove()
{
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if ((dx != 0 || dy != 0)) {
				int i = dx;
				int j = dy;
				while (i + x <= 7 && i + x >= 0 && j + y <= 7 && j + y >= 0) {
					if (isLegalMove(x + i, y + j, true))
						return true;

					j += dy;
					i += dx;
				}
			}
		}
	return false;
}

bool Queen::isLegalMove(int _x, int _y,bool check)
{
	if (_x < 0 || _x > 7 || _y < 0 || _y > 7 || (board->GetPieceAtPos(_x, _y) != nullptr && board->GetPieceAtPos(_x, _y)->GetColor() == color) || (_x == x && _y == y))
		return false;

	

	if ((x - _x) == 0 || (y - _y) == 0 || abs(x - _x) == abs(y - _y)) {
		int dx;
		int dy;
		int nbLoop;

		if ((_y - y) == 0) {
			dx = (_x - x) / abs(x - _x);
			dy = 0;
			nbLoop = abs(x - _x) - 1;
		}
		else if((_x - x) == 0){
			dy = (_y - y) / abs(y - _y);
			dx = 0;
			nbLoop = abs(_y - y) - 1;
		}
		else {
			dx = (_x - x) / abs(x - _x);
			dy = (_y - y) / abs(y - _y);
			nbLoop = abs(x - _x) - 1;
		}


		_x = x + dx;
		_y = y + dy;

		for (int i = 0; i < nbLoop; i++) {

			if (board->GetPieceAtPos(_x, _y) != nullptr)
				return false;
			_x += dx;
			_y += dy;
		}
		if (check && willKingBeInCheck(_x, _y))
			return false;
		return true;
	}
	else
		return false;
		


}

Bishop::Bishop(int _x, int _y, bool c, Board* b)
	:
	Piece(_x, _y, c,b)
{
}

Bishop::Bishop(int _x, int _y, bool c, Board* b, bool d)
	:
	Piece(_x, _y, c, b, d)
{
}

void Bishop::DrawPiece(Graphics& gfx)
{
	if (draw == true) {
		char const* bishop = color ? &whiteBishop[0][0] : &blackBishop[0][0];
		int const squareSize = 90;
		int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
		int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
		const Color black = { 0,0,0 };
		const Color white = { 250,250,250 };
		for (int i = 0; i < 90; i++)
			for (int j = 0; j < 90; j++) {
				if (bishop[i + j * 90] == -1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, black);
				else if (bishop[i + j * 90] == 1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, white);
			}
	}
}

void Bishop::DrawPiece(Graphics& gfx, int _x, int _y)
{
	int const squareSize = 90;


	const int left = _x - squareSize / 2;
	const int right = _x + squareSize / 2;
	const int top = _y - squareSize / 2;
	const int bottom = _y + squareSize / 2;

	if (right >= Graphics::ScreenWidth)
		_x = Graphics::ScreenWidth - squareSize / 2;
	else if (left < 0)
		_x = squareSize / 2;
	if (bottom >= Graphics::ScreenHeight)
		_y = Graphics::ScreenHeight - squareSize / 2;
	else if (top < 0)
		_y = squareSize / 2;

	const char* bishop = color ? &whiteBishop[0][0] : &blackBishop[0][0];

	int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
	int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
	const Color black = { 0,0,0 };
	const Color white = { 250,250,250 };
	for (int i = 0; i < 90; i++)
		for (int j = 0; j < 90; j++) {
			if (bishop[i + j * 90] == -1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, black);
			else if (bishop[i + j * 90] == 1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, white);
		}
}
void Bishop::showLegalMove(Graphics& gfx)
{
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if (abs(dx) == abs(dy) && (dx != 0 || dy != 0)) {
				int i = dx;
				int j = dy;
				while (i + x <= 7 && i + x >= 0 && j + y <= 7 && j + y >= 0) {
					if (isLegalMove(x + i, y + j, true) && board->GetPieceAtPos(x + i, y + j) == nullptr)
						board->DrawDot(x + i, y + j, gfx);
					else if (isLegalMove(x + i, y + j, true) && board->GetPieceAtPos(x + i, y + j)->GetColor() != color) {
						board->DrawDotAround(x + i, y + j, gfx);
						break;
					}

					j += dy;
					i += dx;
				}
			}
		}
}

std::vector<std::pair<int, int>> Bishop::PossibleMove()
{
	std::vector<std::pair<int, int>> result;

	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if (abs(dx) == abs(dy) && (dx != 0 || dy != 0)) {
				int i = dx;
				int j = dy;
				while (i + x <= 7 && i + x >= 0 && j + y <= 7 && j + y >= 0) {
					if (isLegalMove(x + i, y + j, true))
						result.push_back(std::make_pair(x + i, y + j));

					j += dy;
					i += dx;
				}
			}
		}
	return result;
}

int Bishop::GetType()
{
	return BISHOP;
}

bool Bishop::hasLegalMove()
{
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if (abs(dx) == abs(dy) && (dx != 0 || dy != 0)) {
				int i = dx;
				int j = dy;
				while (i + x <= 7 && i + x >= 0 && j + y <= 7 && j + y >= 0) {
					if (isLegalMove(x + i, y + j, true))
						return true;

					j += dy;
					i += dx;
				}
			}
		}
	return false;
}

bool Bishop::isLegalMove(int _x, int _y, bool check)
{
	if (_x < 0 || _x > 7 || _y < 0 || _y > 7 || (_x == x && _y == y))
		return false;

	if (abs(x - _x) != abs(y - _y) || (board->GetPieceAtPos(_x, _y) != nullptr && board->GetPieceAtPos(_x, _y)->GetColor() == color))
		return false;

	int const dx = (_x - x) / abs(x - _x);
	int const dy = (_y - y)/ abs(y - _y);
	int const nbLoop = abs(x - _x) - 1;

	_x = x + dx;
	_y = y + dy;

	for (int i = 0; i < nbLoop; i++) {
		
		if (board->GetPieceAtPos(_x, _y) != nullptr)
			return false;
		_x += dx;
		_y += dy;
	}
	if (check && willKingBeInCheck(_x, _y))
		return false;
	return true;
}

Rook::Rook(int _x, int _y, bool c, Board* b)
	:
	Piece(_x, _y, c,b)
{
}

Rook::Rook(int _x, int _y, bool c, Board* b, bool d)
	:
	Piece(_x, _y, c, b, d)
{
}

void Rook::DrawPiece(Graphics& gfx)
{
	if (draw == true) {
		char const* rook = color ? &whiteRook[0][0] : &blackRook[0][0];
		int const squareSize = 90;
		int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
		int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
		const Color black = { 0,0,0 };
		const Color white = { 250,250,250 };
		for (int i = 0; i < 90; i++)
			for (int j = 0; j < 90; j++) {
				if (rook[i + j * 90] == -1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, black);
				else if (rook[i + j * 90] == 1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, white);
			}
	}
}

void Rook::DrawPiece(Graphics& gfx, int _x, int _y)
{
	int const squareSize = 90;


	const int left = _x - squareSize / 2;
	const int right = _x + squareSize / 2;
	const int top = _y - squareSize / 2;
	const int bottom = _y + squareSize / 2;

	if (right >= Graphics::ScreenWidth)
		_x = Graphics::ScreenWidth - squareSize / 2;
	else if (left < 0)
		_x = squareSize / 2;
	if (bottom >= Graphics::ScreenHeight)
		_y = Graphics::ScreenHeight - squareSize / 2;
	else if (top < 0)
		_y = squareSize / 2;

	const char* rook = color ? &whiteRook[0][0] : &blackRook[0][0];

	int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
	int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
	const Color black = { 0,0,0 };
	const Color white = { 250,250,250 };
	for (int i = 0; i < 90; i++)
		for (int j = 0; j < 90; j++) {
			if (rook[i + j * 90] == -1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, black);
			else if (rook[i + j * 90] == 1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, white);
		}
}
void Rook::showLegalMove(Graphics& gfx)
{
	for(int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if (abs(dx) != abs(dy)) {
				int i = dx;
				int j = dy;
				while (i + x <= 7 && i + x >= 0 && j + y <= 7 && j + y >= 0) {
					if (isLegalMove(x + i, y + j, true) && board->GetPieceAtPos(x + i, y + j) == nullptr)
						board->DrawDot(x + i, y + j, gfx);
					else if (isLegalMove(x + i, y + j, true)) {
						board->DrawDotAround(x + i, y + j, gfx);
						break;
					}
					j += dy;
					i += dx;
				}
			}
		}
}

std::vector<std::pair<int, int>> Rook::PossibleMove()
{
	std::vector<std::pair<int, int>> result;

	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if (abs(dx) != abs(dy)) {
				int i = dx;
				int j = dy;
				while (i + x <= 7 && i + x >= 0 && j + y <= 7 && j + y >= 0) {
					if (isLegalMove(x + i, y + j, true))
						result.push_back(std::make_pair(x + i, y + j));
					j += dy;
					i += dx;
				}
			}
		}
	return result;
}

int Rook::GetType()
{
	return ROOK;
}

bool Rook::hasLegalMove()
{
	for (int dx = -1; dx <= 1; dx++)
		for (int dy = -1; dy <= 1; dy++) {
			if (abs(dx) != abs(dy)) {
				int i = dx;
				int j = dy;
				while (i + x <= 7 && i + x >= 0 && j + y <= 7 && j + y >= 0) {
					if (isLegalMove(x + i, y + j, true))
						return true;
					j += dy;
					i += dx;
				}
			}
		}
	return false;
}

void Rook::IncreaseMoveCounter()
{
	++moveCounter;
}

void Rook::DecreaseMoveCounter()
{
	if(moveCounter > 0)
		--moveCounter;
}

int Rook::GetMoveCounter()
{
	return moveCounter;
}

bool Rook::isLegalMove(int _x, int _y, bool check)
{
	if (_x < 0 || _x > 7 || _y < 0 || _y > 7 || (_x == x && _y == y))
		return false;

	if (((x - _x) != 0 && (y - _y) != 0) || (board->GetPieceAtPos(_x, _y) != nullptr && board->GetPieceAtPos(_x, _y)->GetColor() == color))
		return false;

	int dx;
	int dy;
	int nbLoop;

	if ((_x - x) != 0) {
		dx = (_x - x) / abs(x - _x);
		dy = 0;
		nbLoop = abs(x - _x) - 1;
	}
	else {
		dy = (_y - y) / abs(y - _y);
		dx = 0;
		nbLoop = abs(_y - y) - 1;
	}

	_x = x + dx;
	_y = y + dy;

	for (int i = 0; i < nbLoop; i++) {

		if (board->GetPieceAtPos(_x, _y) != nullptr)
			return false;
		_x += dx;
		_y += dy;
	}
	if (check && willKingBeInCheck(_x, _y))
		return false;
	return true;
}

Knight::Knight(int _x, int _y, bool c, Board* b)
	:
	Piece(_x, _y, c,b)
{
}

Knight::Knight(int _x, int _y, bool c, Board* b, bool d)
	:
	Piece(_x, _y, c, b, d)
{
}

void Knight::DrawPiece(Graphics& gfx)
{
	if (draw == true) {
		char const* knight = color ? &whiteKnight[0][0] : &blackKnight[0][0];
		int const squareSize = 90;
		int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
		int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
		const Color black = { 0,0,0 };
		const Color white = { 250,250,250 };
		for (int i = 0; i < 90; i++)
			for (int j = 0; j < 90; j++) {
				if (knight[i + j * 90] == -1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, black);
				else if (knight[i + j * 90] == 1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, white);
			}
	}
}

void Knight::DrawPiece(Graphics& gfx, int _x, int _y)
{
	int const squareSize = 90;


	const int left = _x - squareSize / 2;
	const int right = _x + squareSize / 2;
	const int top = _y - squareSize / 2;
	const int bottom = _y + squareSize / 2;

	if (right >= Graphics::ScreenWidth)
		_x = Graphics::ScreenWidth - squareSize / 2;
	else if (left < 0)
		_x = squareSize / 2;
	if (bottom >= Graphics::ScreenHeight)
		_y = Graphics::ScreenHeight - squareSize / 2;
	else if (top < 0)
		_y = squareSize / 2;

	const char* knight = color ? &whiteKnight[0][0] : &blackKnight[0][0];

	int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
	int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
	const Color black = { 0,0,0 };
	const Color white = { 250,250,250 };
	for (int i = 0; i < 90; i++)
		for (int j = 0; j < 90; j++) {
			if (knight[i + j * 90] == -1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, black);
			else if (knight[i + j * 90] == 1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, white);
		}
}
void Knight::showLegalMove(Graphics& gfx)
{
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			if (!(i == 0 || j == 0 || abs(i) == abs(j)) && x + i < 8 && x + i >= 0 && y + j < 8 && y + j >= 0) {
				if(isLegalMove(x + i, y + j, true) && board->GetPieceAtPos(x + i, y + j) == nullptr)
					board->DrawDot(x + i, y + j, gfx);
				else if(isLegalMove(x + i, y + j, true))
					board->DrawDotAround(x + i, y + j, gfx);
			}
		}
	}
}

std::vector<std::pair<int, int>> Knight::PossibleMove()
{
	std::vector<std::pair<int, int>> result;

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			if (!(i == 0 || j == 0 || abs(i) == abs(j)) && x + i < 8 && x + i >= 0 && y + j < 8 && y + j >= 0) {
				if (isLegalMove(x + i, y + j, true))
					result.push_back(std::make_pair(x + i, y + j));
			}
		}
	}
	return result;
}

int Knight::GetType()
{
	return KNIGHT;
}

bool Knight::hasLegalMove()
{
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			if (!(i == 0 || j == 0 || abs(i) == abs(j)) && x + i < 8 && x + i >= 0 && y + j < 8 && y + j >= 0) {
				if (isLegalMove(x + i, y + j, true))
					return true;
			}
		}
	}
	return false;
}

bool Knight::isLegalMove(int _x, int _y, bool check)
{
	if (_x < 0 || _x > 7 || _y < 0 || _y > 7 || (board->GetPieceAtPos(_x, _y) != nullptr && board->GetPieceAtPos(_x, _y)->GetColor() == color) || (_x == x && _y == y))
		return false;
	if (!((abs(x - _x) == 1 && abs(y - _y) == 2) || (abs(x - _x) == 2 && abs(y - _y) == 1)))
		return false;
	
	if (check && willKingBeInCheck(_x, _y))
		return false;

	return true;
}

Pawn::Pawn(int _x, int _y, bool c, Board* b)
	:
	Piece(_x, _y, c,b)
{
}

Pawn::Pawn(int _x, int _y, bool c, Board* b, bool d)
	:
	Piece(_x, _y, c, b, d)
{
}

void Pawn::DrawPiece(Graphics& gfx)
{
	if (draw == true) {
		char const* pawn = color ? &whitePawn[0][0] : &blackPawn[0][0];
		int const squareSize = 90;
		int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
		int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
		const Color black = { 0,0,0 };
		const Color white = { 250,250,250 };
		for (int i = 0; i < 90; i++)
			for (int j = 0; j < 90; j++) {
				if (pawn[i + j * 90] == -1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, black);
				else if (pawn[i + j * 90] == 1)
					gfx.PutPixel(x * Graphics::ScreenWidth / 8 + dx + i, y * Graphics::ScreenHeight / 8 + dy + j, white);
			}
	}
}

void Pawn::DrawPiece(Graphics& gfx, int _x, int _y)
{
	int const squareSize = 90;


	const int left = _x - squareSize / 2;
	const int right = _x + squareSize / 2;
	const int top = _y - squareSize / 2;
	const int bottom = _y + squareSize / 2;

	if (right >= Graphics::ScreenWidth)
		_x = Graphics::ScreenWidth - squareSize / 2;
	else if (left < 0)
		_x = squareSize / 2;
	if (bottom >= Graphics::ScreenHeight)
		_y = Graphics::ScreenHeight - squareSize / 2;
	else if (top < 0)
		_y = squareSize / 2;

	const char* pawn = color ? &whitePawn[0][0] : &blackPawn[0][0];

	int const dx = (Graphics::ScreenWidth / 8 - squareSize) / 2;
	int const dy = (Graphics::ScreenHeight / 8 - squareSize) / 2;
	const Color black = { 0,0,0 };
	const Color white = { 250,250,250 };
	for (int i = 0; i < 90; i++)
		for (int j = 0; j < 90; j++) {
			if (pawn[i + j * 90] == -1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, black);
			else if (pawn[i + j * 90] == 1)
				gfx.PutPixel(_x - squareSize / 2 + i, _y - squareSize / 2 + j, white);
		}
}

void Pawn::showLegalMove(Graphics& gfx)
{
	int const direction = color ? -1 : 1;

	//move
	if (color && y == 6 || !color && y == 1) {
		for (int dy = direction; abs(dy) <= 2; dy += direction) {
			if (isLegalMove(x, y + dy,true))
				board->DrawDot(x, y + dy, gfx);
		}
	}
	else {
		if (isLegalMove(x, y + direction, true))
			board->DrawDot(x, y + direction, gfx);
	}
	//capture

	if (isLegalMove(x - 1, y + direction, true)) {
		if (board->GetPieceAtPos(x - 1, y + direction) != nullptr)
			board->DrawDotAround(x - 1, y + direction, gfx);
		else
			board->DrawDot(x - 1, y + direction, gfx);
	}
	if (isLegalMove(x + 1, y + direction, true)) {
		if(board->GetPieceAtPos(x + 1, y + direction) != nullptr)
			board->DrawDotAround(x + 1, y + direction, gfx);
		else
			board->DrawDot(x + 1, y + direction, gfx);
	}
}

std::vector<std::pair<int, int>> Pawn::PossibleMove()
{
	std::vector<std::pair<int, int>> result;
	int const direction = color ? -1 : 1;

	//move
	if (color && y == 6 || !color && y == 1) {
		for (int dy = direction; abs(dy) <= 2; dy += direction) {
			if (isLegalMove(x, y + dy, true))
				result.push_back(std::make_pair(x, y + dy));
		}
	}
	else {
		if (isLegalMove(x, y + direction, true))
			result.push_back(std::make_pair(x, y + direction));
	}
	//capture

	if (isLegalMove(x - 1, y + direction, true))
		result.push_back(std::make_pair(x - 1, y + direction));
	if (isLegalMove(x + 1, y + direction, true))
		result.push_back(std::make_pair(x + 1, y + direction));


	return result;
}

int Pawn::GetType()
{
	return PAWN;
}

bool Pawn::hasLegalMove()
{
	int const direction = color ? -1 : 1;

	//move
	if (color && y == 6 || !color && y == 1) {
		for (int dy = direction; abs(dy) <= 2; dy += direction) {
			if (isLegalMove(x, y + dy, true))
				return true;
		}
	}
	else {
		if (isLegalMove(x, y + direction, true))
			return true;
	}
	//capture

	if (isLegalMove(x - 1, y + direction, true))
		return true;
	if (isLegalMove(x + 1, y + direction, true))
		return true;
	return false;
}

bool Pawn::isLegalMove(int _x, int _y, bool check)
{
	if (_x < 0 || _x > 7 || _y < 0 || _y > 7 || (board->GetPieceAtPos(_x, _y) != nullptr && board->GetPieceAtPos(_x, _y)->GetColor() == color) || (_x == x && _y == y))
		return false;

	int const direction = color ? -1 : 1;

	if (check && willKingBeInCheck(_x, _y))
		return false;

	if ((_x - x) == 0 && (_y - y)/abs(_y - y) == direction){
		//move
		if ((color && y == 6 || !color && y == 1) && abs(_y - y) == 2 ) {
			if (board->GetPieceAtPos(_x, _y) == nullptr && board->GetPieceAtPos(x, y + direction) == nullptr)
				return true;
		}
		else if(abs(_y - y) == 1 && board->GetPieceAtPos(x, y + direction) == nullptr ){
			return true;
		}
	}
	else if (abs(_x - x) == 1 && (_y - y) == direction) {
		//capture
		if (board->GetPieceAtPos(_x, _y) != nullptr && board->GetPieceAtPos(_x, _y)->GetColor() != color)
			return true;
		else if (_y == (color ? 2 : 5) && board->GetPieceAtPos(_x, _y) == nullptr && board->GetPieceAtPos(_x, _y - direction) != nullptr && board->GetPieceAtPos(_x, _y - direction)->GetType() == PAWN && ((Pawn*)board->GetPieceAtPos(_x, _y - direction))->enPassantTurn == board->turnCounter)
			return true;
	}

	return false;
}

