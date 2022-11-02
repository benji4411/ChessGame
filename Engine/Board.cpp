#include "Board.h"


//Board class constructor
Board::Board()
{
	//Create the black pieces
	blackKing = new King(4, 0, false, this);
	Queen* blackQueen = new Queen(3, 0, false, this);
	Knight* blackKnight1= new Knight(1, 0, false, this);
	Knight* blackKnight2= new Knight(6, 0, false, this);
	Bishop* blackBishop1= new Bishop(5, 0, false, this);
	Bishop* blackBishop2= new Bishop(2, 0, false, this);
	Rook* blackRook1 = new Rook(0, 0, false, this);
	Rook* blackRook2 = new Rook(7, 0, false, this);
	Pawn* blackPawn1 = new Pawn(0, 1, false, this);
	Pawn* blackPawn2 = new Pawn(1, 1, false, this);
	Pawn* blackPawn3 = new Pawn(2, 1, false, this);
	Pawn* blackPawn4 = new Pawn(3, 1, false, this);
	Pawn* blackPawn5 = new Pawn(4, 1, false, this);
	Pawn* blackPawn6 = new Pawn(5, 1, false, this);
	Pawn* blackPawn7 = new Pawn(6, 1, false, this);
	Pawn* blackPawn8 = new Pawn(7, 1, false, this);

	//Create the white pieces
	whiteKing = new King(4, 7, true, this);
	Queen* whiteQueen = new Queen(3, 7, true, this);
	Knight* whiteKnight1 = new Knight(1, 7, true, this);
	Knight* whiteKnight2 = new Knight(6, 7, true, this);
	Bishop* whiteBishop1= new Bishop(5, 7, true, this);
	Bishop* whiteBishop2= new Bishop(2, 7, true, this);
	Rook* whiteRook1= new Rook(0, 7, true, this);
	Rook* whiteRook2= new Rook(7, 7, true, this);
	Pawn* whitePawn1= new Pawn(0, 6, true, this);
	Pawn* whitePawn2= new Pawn(1, 6, true, this);
	Pawn* whitePawn3= new Pawn(2, 6, true, this);
	Pawn* whitePawn4= new Pawn(3, 6, true, this);
	Pawn* whitePawn5= new Pawn(4, 6, true, this);
	Pawn* whitePawn6= new Pawn(5, 6, true, this);
	Pawn* whitePawn7= new Pawn(6, 6, true, this);
	Pawn* whitePawn8= new Pawn(7, 6, true, this);

	//Push all the black pieces into the pieces vector except the black king as it is unique and can be accessed more easily to check if it is attacked
	pieces.push_back(blackQueen);
	pieces.push_back(blackKnight1);
	pieces.push_back(blackKnight2);
	pieces.push_back(blackBishop1);
	pieces.push_back(blackBishop2);
	pieces.push_back(blackRook1);
	pieces.push_back(blackRook2);
	pieces.push_back(blackPawn1);
	pieces.push_back(blackPawn2);
	pieces.push_back(blackPawn3);
	pieces.push_back(blackPawn4);
	pieces.push_back(blackPawn5);
	pieces.push_back(blackPawn6);
	pieces.push_back(blackPawn7);
	pieces.push_back(blackPawn8);

	//Push all the white pieces into the pieces vector except the white king as it is unique and can be accessed more easily to check if it is attacked
	pieces.push_back(whiteQueen);;
	pieces.push_back(whiteKnight1);
	pieces.push_back(whiteKnight2);
	pieces.push_back(whiteBishop1);
	pieces.push_back(whiteBishop2);
	pieces.push_back(whiteRook1);
	pieces.push_back(whiteRook2);
	pieces.push_back(whitePawn1);
	pieces.push_back(whitePawn2);
	pieces.push_back(whitePawn3);
	pieces.push_back(whitePawn4);
	pieces.push_back(whitePawn5);
	pieces.push_back(whitePawn6);
	pieces.push_back(whitePawn7);
	pieces.push_back(whitePawn8);
}

//Copy Constructor useful for the AI which will test all moves on another board
Board::Board(const Board& b)
{
	int x = 0;
	for (Piece* p : b.pieces) {
		x++;
		switch (p->GetType()) {
		case QUEEN: pieces.push_back(new Queen(p->GetX(), p->GetY(), p->GetColor(), this, p->disabled)); break;
		case BISHOP : pieces.push_back(new Bishop(p->GetX(), p->GetY(), p->GetColor(), this, p->disabled)); break;
		case ROOK: pieces.push_back(new Rook(p->GetX(), p->GetY(), p->GetColor(), this, p->disabled)); break;
		case PAWN: pieces.push_back(new Pawn(p->GetX(), p->GetY(), p->GetColor(), this, p->disabled)); break;
		case KNIGHT: pieces.push_back(new Knight(p->GetX(), p->GetY(), p->GetColor(), this, p->disabled)); break;
		}
	}

	blackKing = new King(b.blackKing->GetX(),b.blackKing->GetY(),b.blackKing->GetColor(),this);
	whiteKing = new King(b.whiteKing->GetX(),b.whiteKing->GetY(),b.whiteKing->GetColor(),this);
	
}

//Destructor to delete pointers to pieces
Board::~Board()
{
	for (Piece* p : pieces) {
		delete(p);
	}
	delete(blackKing);
	delete(whiteKing);
}

//Draw the cells
void Board::DrawBoard(Graphics& gfx)
{
	
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) 
			DrawCase(i * Graphics::ScreenWidth / width, j * Graphics::ScreenHeight / height, (i + j) % 2 == 0 ? beige : green, gfx);
		
}

//Draw the numbers and letters to indicate the position
void Board::DrawGrid(Graphics& gfx) {

	int i = 0;
	int j;
	for (j = 0; j < width; j++)
		DrawNumber(1 + j, 5, 5 + j * Graphics::ScreenHeight / height, (i + j) % 2 == 0 ? green : beige, gfx);
	j = 7;
	for (i = 0; i < width; i++)
		DrawAlpha('A' + i, 83 + i * Graphics::ScreenWidth / width, 780, (i + j) % 2 == 0 ? green : beige, gfx);

}

//Draw the pieces
void Board::DrawPieces(Graphics& gfx)
{
	for (auto (&p) : pieces)
		p->DrawPiece(gfx);
	whiteKing->DrawPiece(gfx);
	blackKing->DrawPiece(gfx);
}

//Draw a dot to indicate the legal moves
void Board::DrawDot(int x, int y, Graphics& gfx)
{

	Color pixel = gfx.GetPixel(x * Graphics::ScreenWidth / width , y * Graphics::ScreenHeight / height );
	Color rightColor;

	if (pixel.GetR() == green.GetR() && pixel.GetG() == green.GetG() && pixel.GetB() == green.GetB())
		rightColor = dotOnGreen;
	else if (pixel.GetR() == beige.GetR() && pixel.GetG() == beige.GetG() && pixel.GetB() == beige.GetB())
		rightColor = dotOnBeige;
	else if (pixel.GetR() == highLightOnBeige.GetR() && pixel.GetG() == highLightOnBeige.GetG() && pixel.GetB() == highLightOnBeige.GetB())
		rightColor = dotOnHighLightOnBeige;
	else if (pixel.GetR() == highLightOnGreen.GetR() && pixel.GetG() == highLightOnGreen.GetG() && pixel.GetB() == highLightOnGreen.GetB())
		rightColor = dotOnHighLightOnGreen;

	gfx.DrawCircle((float)((x + 0.5) * Graphics::ScreenWidth / width), (float)((y + 0.5) * Graphics::ScreenHeight / height), 20, 0, rightColor);
}

//Draw a circle to indicate pieces that can be capture
void Board::DrawDotAround(int x, int y, Graphics& gfx)
{

	Color pixel = gfx.GetPixel(x * Graphics::ScreenWidth / width , y * Graphics::ScreenHeight / height );
	Color rightColor;

	if (pixel.GetR() == green.GetR() && pixel.GetG() == green.GetG() && pixel.GetB() == green.GetB())
		rightColor = dotOnGreen;
	else if (pixel.GetR() == beige.GetR() && pixel.GetG() == beige.GetG() && pixel.GetB() == beige.GetB())
		rightColor = dotOnBeige;
	else if (pixel.GetR() == highLightOnBeige.GetR() && pixel.GetG() == highLightOnBeige.GetG() && pixel.GetB() == highLightOnBeige.GetB())
		rightColor = dotOnHighLightOnBeige;
	else if (pixel.GetR() == highLightOnGreen.GetR() && pixel.GetG() == highLightOnGreen.GetG() && pixel.GetB() == highLightOnGreen.GetB())
		rightColor = dotOnHighLightOnGreen;

	gfx.DrawCircle((float)((x + 0.5) * Graphics::ScreenWidth / width),(float)((y + 0.5) * Graphics::ScreenHeight / height), 50, 40, rightColor);
}

//Highlight cases to indicate last move
void Board::HighlightCase(int x, int y, Graphics& gfx)
{
	const Color highLightOnBeige = { 246,246,105 };
	const Color highLightOnGreen = { 186,202,43 };
	for (int i = 0; i < Graphics::ScreenWidth / width; i++)
		for (int j = 0; j < Graphics::ScreenHeight / height; j++)
			gfx.PutPixel(x * Graphics::ScreenWidth / width + i, y * Graphics::ScreenHeight / height + j, (x + y) % 2 == 0 ? highLightOnBeige : highLightOnGreen);
}

//Move a piece to a (x,y) position, capture a piece if necessary and promote if pawn on the last row
void Board::Move(Piece* piece, int x, int y)
{
	if (x < 0 || x > 7 || y < 0 || y > 7)
		return;
	turnCounter++;

	if (GetPieceAtPos(x, y) != nullptr) { //regular capture
		moves.push(MoveData(piece, GetPieceAtPos(x, y), std::make_pair(piece->GetX(), piece->GetY())));
		GetPieceAtPos(x, y)->Draw(false);
		GetPieceAtPos(x, y)->disabled = true;
	}
	else if ((piece->GetType() == PAWN) && piece->GetX() != x) { //en passant
		moves.push(MoveData(piece, GetPieceAtPos(x, piece->GetY()), std::make_pair(piece->GetX(), piece->GetY())));
		GetPieceAtPos(x, piece->GetY())->Draw(false);
		GetPieceAtPos(x, piece->GetY())->disabled = true;
	}else
		moves.push(MoveData(piece, nullptr, std::make_pair(piece->GetX(), piece->GetY())));//no capture

	if (piece->GetType() == KING)
		((King*)piece)->IncreaseMoveCounter();
	if (piece->GetType() == ROOK)
		((Rook*)piece)->IncreaseMoveCounter();
	// castling
	if (piece->GetType() == KING && abs(x - piece->GetX()) == 2) {
		if (piece->GetColor() == true) {
			if (x - piece->GetX() < 0) 
				GetPieceAtPos(0, 7)->MoveTo(3, 7);
			else
				GetPieceAtPos(7, 7)->MoveTo(5, 7);
		}
		else {
			if (x - piece->GetX() < 0)
				GetPieceAtPos(7, 7)->MoveTo(3, 0);
			else
				GetPieceAtPos(7, 0)->MoveTo(5, 0);
		}
	}
	if ((piece->GetType() == PAWN) && abs(piece->GetY() - y) == 2) {
			((Pawn*)piece)->enPassantTurn = turnCounter;
	}
	piece->MoveTo(x, y);
	// Queen promotion
	if (piece->GetType() == PAWN &&( y == 7 && !piece->GetColor() || y == 0 && piece->GetColor())) {
		piece->Draw(false);
		piece->disabled = true;
		pieces.push_back(new Queen(x, y, piece->GetColor(), this));
	}
	

}

//Undo the last move, useful for the AI to test moves
bool Board::UndoMove()
{
	if (!moves.empty()) {
		turnCounter--;

		Piece* piece = moves.top().pieceMoved;

		if (piece->GetType() == KING)
			((King*)piece)->DecreaseMoveCounter();
		if (piece->GetType() == ROOK)
			((Rook*)piece)->DecreaseMoveCounter();

		//Undo Queen promotion
		if (piece->disabled == true) {
			DeletePiece(GetPieceAtPos(piece->GetX(), piece->GetY()));
			piece->Draw(true);
			piece->disabled = false;
		}
		//Undo Castling
		if (piece->GetType() == KING && abs(moves.top().from.first - piece->GetX()) == 2) {
			if (piece->GetColor() == true) {
				if (moves.top().from.first - piece->GetX() > 0) {
					GetPieceAtPos(3, 7)->MoveTo(0, 7);
				}
				else {
					GetPieceAtPos(5, 7)->MoveTo(7, 7);
				}
			}
			else {
				if (moves.top().from.first - piece->GetX() > 0) {
					GetPieceAtPos(3, 0)->MoveTo(0, 0);
				}
				else {
					GetPieceAtPos(5, 0)->MoveTo(7, 0);
				}
			}

		}

		//Undo capture
		if (moves.top().pieceDisabled != nullptr) {
			moves.top().pieceDisabled->Draw(true);
			moves.top().pieceDisabled->disabled = false;
		}
		//Undo move
		piece->MoveTo(moves.top().from.first, moves.top().from.second);
		moves.pop();
		return true;

	}
	else
		return false;
}

//Check if stalemate
bool Board::StillLegalMove(bool color)
{
	for (auto (&p) : pieces)
		if (!p->disabled && p->GetColor() == color && p->hasLegalMove())
			return true;
	if (color && whiteKing->hasLegalMove())
		return true;
	if (!color && blackKing->hasLegalMove())
		return true;
	return false;
}

//Get a piece at a position (x,y)
Piece* Board::GetPieceAtPos(int _x, int _y)
{
	Piece* result = nullptr;
	for (auto (&p) : pieces)
		if (!p->disabled && p->GetX() == _x && p->GetY() == _y)
			result = p;
	if (whiteKing->GetX() == _x && whiteKing->GetY() == _y)
		result = whiteKing;
	if (blackKing->GetX() == _x && blackKing->GetY() == _y)
		result = blackKing;
	return result;
}

//Get the disabled piece at a position (x,y) (disabled = captured piece or pawn promoted) 
Piece* Board::GetDisabledPieceAtPos(int _x, int _y)
{
	Piece* result = nullptr;
	for (auto (&p) : pieces)
		if (p->disabled && p->GetX() == _x && p->GetY() == _y)
			result = p;
	return result;
}

//Delete a piece, useful to delete a queen when undo promotion
void Board::DeletePiece(Piece* p)
{
	auto it = pieces.begin();
	for (it; it != pieces.end(); it++) {
		if (*it == p) {
			pieces.erase(it);
			delete p;
			break;
		}
	}

}

//Check if the king is attacked
bool Board::isKingInCheck(bool _color)
{
	int x = 0;
	int y = 0; 
	if (_color) {
		x = whiteKing->GetX();
		y = whiteKing->GetY();
	}
	else {
		x = blackKing->GetX();
		y = blackKing->GetY();
	}
	for (auto (&p) : pieces)
		if (p->GetColor() != _color && !p->disabled && p->isLegalMove(x,y,false))
			return true;
	if (!_color && whiteKing->isLegalMove(x, y, false))
		return true;
	if (_color && blackKing->isLegalMove(x, y, false))
		return true;
	return false;
}

//Check if a cell is attacked
bool Board::isCellAttackedBy(int _x, int _y, bool _color)
{
	for (auto (&p) : pieces)
		if (p->GetColor() == _color && !p->disabled && p->isLegalMove(_x, _y, false))
			return true;
	if (_color && whiteKing->isLegalMove(_x, _y, false))
		return true;
	if (!_color && blackKing->isLegalMove(_x, _y, false))
		return true;
	return false;
}

//Check if the game is ended by mate or stalemate
bool Board::GameIsOver() {
	if ((isKingInCheck(true) && !StillLegalMove(true)) || (isKingInCheck(false) && !StillLegalMove(false)))
		return true;
	else
		return false;
}

//return the evaluation of a position for white
float Board::whiteScore()
{
	float result = 0;
	if (isKingInCheck(false) && !StillLegalMove(false))
		return 10000000;
	if(!isKingInCheck(false) && !StillLegalMove(false))
		return -10000000;
	for (auto (&p) : pieces)
		if (p->GetColor() && !p->disabled){
			switch (p->GetType()) {
			case KING: result += 200; break;
			case QUEEN: result += 9; break;
			case ROOK: result += 5; break;
			case BISHOP: result += 3; break;
			case KNIGHT: result += 3; break;
			case PAWN: result += 1; break;
			}
		
		}
	return result;
}

//return the evaluation of a position for black
float Board::blackScore()
{
	float result = 0;
	if (isKingInCheck(true) && !StillLegalMove(true))
		return 10000000;
	if (!isKingInCheck(true) && !StillLegalMove(true))
		return -10000000;
	for (auto (&p) : pieces)
		if (!p->GetColor() && !p->disabled) {
			switch (p->GetType()) {
			case KING: result += 200; break;
			case QUEEN: result += 9; break;
			case ROOK: result += 5;  break;
			case BISHOP: result += 3.3f;  break;
			case KNIGHT: result += 3.2f; break;
			case PAWN: result += 1; break;
			}

		}
	return result;
}

//return all the possible moves for a position
std::vector<PossibleMoves> Board::AllPossibleMove(bool _color)
{
	std::vector<PossibleMoves> result;
	std::vector<std::pair<int, int>> possibleMove;

	for (auto (&p) : pieces)
		if (p->GetColor() == _color && !p->disabled) {
			possibleMove = p->PossibleMove();
			if (!possibleMove.empty())
				result.push_back(PossibleMoves(p, possibleMove));
		}
	if (_color) {
		possibleMove = whiteKing->PossibleMove();
		if(!possibleMove.empty())
			result.push_back(PossibleMoves(whiteKing, possibleMove));
	}
	if (!_color) {
		possibleMove = blackKing->PossibleMove();
		if(!possibleMove.empty())
			result.push_back(PossibleMoves(blackKing, possibleMove));
	}

	return result;
}


//Draw a cell
void Board::DrawCase(int x, int y, Color c, Graphics& gfx)
{
	for (int i = 0; i < Graphics::ScreenWidth / width; i++)
		for (int j = 0; j < Graphics::ScreenHeight / height; j++)
			gfx.PutPixel(x + i, y + j, c);

}

//Draw a number
void Board::DrawNumber(int number, int x, int y, Color c, Graphics& gfx)
{
	const bool _1[5][4] = { 0,1,1,0,
							1,1,1,0,
							0,1,1,0,
							0,1,1,0,
							1,1,1,1 };
	const bool _2[5][4] = { 0,1,1,0,
							1,0,0,1,
							0,0,1,0,
							0,1,0,0,
							1,1,1,1 };
	const bool _3[5][4] = { 1,1,1,0,
							0,0,0,1,
							0,1,1,0,
							0,0,0,1,
							1,1,1,0};
	const bool _4[5][4] = { 1,0,0,1,
							1,0,0,1,
							1,1,1,1,
							0,0,0,1,
							0,0,0,1 };
	const bool _5[5][4] = { 1,1,1,1,
							1,0,0,0,
							1,1,1,0,
							0,0,0,1,
							1,1,1,0 };
	const bool _6[5][4] = { 0,0,1,0,
							0,1,0,0,
							1,1,1,0,
							1,0,0,1,
							0,1,1,0};

	const bool _7[5][4] = { 1,1,1,1,
							0,0,1,0,
							0,1,0,0,
							0,1,0,0,
							0,1,0,0 };

	const bool _8[5][4] = { 0,1,1,0,
							1,0,0,1,
							0,1,1,0,
							1,0,0,1,
							0,1,1,0 };

	const int size = 3;

	switch (number) {
	case 1: for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			if (_1[j][i] == true)
				for (int l = 0; l < size;l++)
					for (int L = 0; L < size; L++)
						gfx.PutPixel(x + i * size + L, y + j * size + l, c);
		break;
	case 2: for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			if (_2[j][i] == true)
				for (int l = 0; l < size;l++)
					for (int L = 0; L < size; L++)
						gfx.PutPixel(x + i * size + L, y + j * size + l, c);
		break;
	case 3: for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			if (_3[j][i] == true)
				for (int l = 0; l < size;l++)
					for (int L = 0; L < size; L++)
						gfx.PutPixel(x + i * size + L, y + j * size + l, c);
		break;
	case 4: for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			if (_4[j][i] == true)
				for (int l = 0; l < size;l++)
					for (int L = 0; L < size; L++)
						gfx.PutPixel(x + i * size + L, y + j * size + l, c);
		break;
	case 5: for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			if (_5[j][i] == true)
				for (int l = 0; l < size;l++)
					for (int L = 0; L < size; L++)
						gfx.PutPixel(x + i * size + L, y + j * size + l, c);
		break;
	case 6: for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			if (_6[j][i] == true)
				for (int l = 0; l < size;l++)
					for (int L = 0; L < size; L++)
						gfx.PutPixel(x + i * size + L, y + j * size + l, c);
		break;
	case 7: for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			if (_7[j][i] == true)
				for (int l = 0; l < size;l++)
					for (int L = 0; L < size; L++)
						gfx.PutPixel(x + i * size + L, y + j * size + l, c);
		break;
	case 8: for (int i = 0; i < 4; i++)
		for (int j = 0; j < 5; j++)
			if (_8[j][i] == true)
				for (int l = 0; l < size;l++)
					for (int L = 0; L < size; L++)
						gfx.PutPixel(x + i * size + L, y + j * size + l, c);
		break;
	default: break;
	}
}

//Draw a letter
void Board::DrawAlpha(char alpha,int x, int y, Color c, Graphics& gfx)
{
	const bool A[5][4] = {  0,1,1,0,
							1,0,0,1,
							1,1,1,1,
							1,0,0,1,
							1,0,0,1 };
	const bool B[5][4] = {  1,1,1,0,
							1,0,0,1,
							1,1,1,0,
							1,0,0,1,
							1,1,1,0 };
	const bool C[5][4] = {  0,1,1,0,
							1,0,0,0,
							1,0,0,0,
							1,0,0,0,
							0,1,1,0 };
	const bool D[5][4] = {  1,1,1,0,
							1,0,0,1,
							1,0,0,1,
							1,0,0,1,
							1,1,1,0 };
	const bool E[5][4] = {  1,1,1,1,
							1,0,0,0,
							1,1,1,0,
							1,0,0,0,
							1,1,1,1 };
	const bool F[5][4] = {  1,1,1,1,
							1,0,0,0,
							1,1,1,0,
							1,0,0,0,
							1,0,0,0 };

	const bool G[5][5] = {  0,1,1,1,0,
							1,0,0,0,0,
							1,0,0,1,1,
							1,0,0,0,1,
							0,1,1,1,0 };

	const bool H[5][4] = {  1,0,0,1,
							1,0,0,1,
							1,1,1,1,
							1,0,0,1,
							1,0,0,1 };

	const int size = 3;

	switch (alpha) {
	case 'A': for (int i = 0; i < 4; i++)
				for (int j = 0; j < 5; j++)
					if (A[j][i] == true)
						for (int l = 0; l < size;l++)
							for (int L = 0; L < size; L++)
								gfx.PutPixel(x + i * size + L, y + j * size + l, c);
			  break;
	case 'B': for (int i = 0; i < 4; i++)
				for (int j = 0; j < 5; j++)
					if (B[j][i] == true)
						for (int l = 0; l < size;l++)
							for (int L = 0; L < size; L++)
								gfx.PutPixel(x + i * size + L, y + j * size + l, c);
				break;
	case 'C': for (int i = 0; i < 4; i++)
				for (int j = 0; j < 5; j++)
					if (C[j][i] == true)
						for (int l = 0; l < size;l++)
							for (int L = 0; L < size; L++)
								gfx.PutPixel(x + i * size + L, y + j * size + l, c);
				break;
	case 'D': for (int i = 0; i < 4; i++)
				for (int j = 0; j < 5; j++)
					if (D[j][i] == true)
						for (int l = 0; l < size;l++)
							for (int L = 0; L < size; L++)
								gfx.PutPixel(x + i * size + L, y + j * size + l, c);
				break;
	case 'E': for (int i = 0; i < 4; i++)
				for (int j = 0; j < 5; j++)
					if (E[j][i] == true)
						for (int l = 0; l < size;l++)
							for (int L = 0; L < size; L++)
								gfx.PutPixel(x + i * size + L, y + j * size + l, c);
				break;
	case 'F': for (int i = 0; i < 4; i++)
				for (int j = 0; j < 5; j++)
					if (F[j][i] == true)
						for (int l = 0; l < size;l++)
							for (int L = 0; L < size; L++)
								gfx.PutPixel(x + i * size + L, y + j * size + l, c);
				break;
	case 'G': for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					if (G[j][i] == true)
						for (int l = 0; l < size;l++)
							for (int L = 0; L < size; L++)
								gfx.PutPixel(x + i * size + L, y + j * size + l, c);
				break;
	case 'H': for (int i = 0; i < 4; i++)
				for (int j = 0; j < 5; j++)
					if (H[j][i] == true)
						for (int l = 0; l < size;l++)
							for (int L = 0; L < size; L++)
								gfx.PutPixel(x + i * size + L, y + j * size + l, c);
		   	break;
	default: break;
	}
	
}
