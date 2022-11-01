/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Board.h"
#include "Piece.h"
#include "Sound.h"
#include "ChessAI.h"
#include "Menu.h"
#include "Rec.h"
#include <future>
#include <chrono>

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	void PlayerTurn();
	void BotTurn();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	Board board;
	Piece* pieceHolded = nullptr;
	Piece* pieceSelected = nullptr;
	bool pieceIsHolded = false;
	int xPieceHolded = -1;
	int yPieceHolded = -1;
	bool ableToSelected = true;
	bool colorTurn = true;

	bool LeftPressed = false;

	int XlastMoveBefore = -1;
	int YlastMoveBefore = -1;
	int XlastMoveAfter = -1;
	int YlastMoveAfter = -1;

	std::pair<Move*, float> bestMove;
	bool minimaxIsCalculating = false;
	std::future<std::pair<Move*, float>> futureBestMove;
	Board* copiedBoard = nullptr;

	Sound captureSound;
	Sound checkSound;
	Sound moveSound;
	Sound endSound;
	Sound castlingSound;

	enum { MENU, PLAYERVSPLAYER, PLAYERVSBOT } gameState = MENU;

	Menu menu;

	bool stockFishInitialized = false;

	/********************************/
};