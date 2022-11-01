/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"


Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	moveSound(L"move.wav"),
	captureSound(L"capture.wav"),
	checkSound(L"check.wav"),
	endSound(L"end.wav"),
	castlingSound(L"castling.wav")
{
	
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (gameState == MENU) {
		int mouseX = wnd.mouse.GetPosX();
		int mouseY = wnd.mouse.GetPosY();

		if (menu.Get1v1ButtonRec().Contains(mouseX, mouseY) && wnd.mouse.LeftIsPressed())
			gameState = PLAYERVSPLAYER;
		if (menu.GetAgainstBotButtonRec().Contains(mouseX, mouseY) && wnd.mouse.LeftIsPressed())
			gameState = PLAYERVSBOT;
	}
	else if (gameState == PLAYERVSPLAYER) {

		if (colorTurn) {
			PlayerTurn();
		}
		else {
			PlayerTurn();
		}

		// Undo move by pressing left arrow key
		if (wnd.kbd.KeyIsPressed(VK_LEFT) && LeftPressed == false) {
			if (board.UndoMove())
				colorTurn = !colorTurn;
			LeftPressed = true;
		}
		else if (!wnd.kbd.KeyIsPressed(VK_LEFT))
			LeftPressed = false;
	}
	else if (gameState == PLAYERVSBOT) {

		if (stockFishInitialized == false) {



			stockFishInitialized = true;
		}


		if (colorTurn) {
			PlayerTurn();
		}
		else {
			BotTurn();
		}
	}
		
}

void Game::ComposeFrame()
{
	if (gameState == MENU) {
		gfx.FillScreen(beige);
		menu.DrawMenu(gfx);				
	}
	else if (gameState == PLAYERVSPLAYER || gameState == PLAYERVSBOT) {
		board.DrawBoard(gfx);
		if (XlastMoveAfter != -1 && YlastMoveAfter != -1) {
			board.HighlightCase(XlastMoveAfter, YlastMoveAfter, gfx);
		}
		if (XlastMoveBefore != -1 && YlastMoveBefore != -1) {
			board.HighlightCase(XlastMoveBefore, YlastMoveBefore, gfx);
		}
		if (pieceSelected != nullptr) {
			board.HighlightCase(pieceSelected->GetX(), pieceSelected->GetY(), gfx);
			pieceSelected->showLegalMove(gfx);
		}
		board.DrawGrid(gfx);
		board.DrawPieces(gfx);

		if (pieceIsHolded == true)
			pieceHolded->DrawPiece(gfx, wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
	}
}

void Game::BotTurn()
{
	
	if (!minimaxIsCalculating) {
		copiedBoard = new Board(board);
		futureBestMove = std::async(std::launch::async, ChessAI::minimax, std::ref(*copiedBoard), 4, -INFINITY, INFINITY, colorTurn, false);

		minimaxIsCalculating = true;
	}
	else if (futureBestMove.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
		auto bestMove = futureBestMove.get();
		if (bestMove.first != nullptr) {

			Move move = *bestMove.first;
			bool playMove = board.GetPieceAtPos(move.move.first, move.move.second) == nullptr;
			XlastMoveBefore = move.piece->GetX();
			YlastMoveBefore = move.piece->GetY();
			XlastMoveAfter = move.move.first;
			YlastMoveAfter = move.move.second;
			board.Move(board.GetPieceAtPos(move.piece->GetX(), move.piece->GetY()), move.move.first, move.move.second);//getPieceAtPose is used to find the corresponding piece from copiedBoard to board
			if (!board.StillLegalMove(!colorTurn))//game end
				endSound.Play();
			if (board.isKingInCheck(!colorTurn))
				checkSound.Play();
			else {
				if (move.piece->GetType() == KING && abs(XlastMoveBefore - XlastMoveAfter) == 2)
					castlingSound.Play();
				else if (!playMove || move.piece->GetType() == PAWN && abs(XlastMoveBefore - XlastMoveAfter) == 1)
					captureSound.Play();
				else
					moveSound.Play();
			}
		}
		colorTurn = !colorTurn;
		minimaxIsCalculating = false;
		delete(copiedBoard);
	}
	
}

void Game::PlayerTurn()
{
	//piece holding
	if (wnd.mouse.LeftIsPressed() && pieceIsHolded == false) {
		if (board.GetPieceAtPos(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100) == nullptr)
			pieceHolded = nullptr;
		else if (board.GetPieceAtPos(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100)->GetColor() == colorTurn)
			pieceHolded = board.GetPieceAtPos(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100);
		else
			pieceHolded = nullptr;
		if (pieceHolded != nullptr) {
			pieceHolded->Draw(false);
			pieceIsHolded = true;
			xPieceHolded = wnd.mouse.GetPosX() / 100;
			yPieceHolded = wnd.mouse.GetPosY() / 100;
		}
	}//piece released
	else if (!wnd.mouse.LeftIsPressed() && pieceIsHolded == true) {
		pieceHolded->Draw(true);
		if ((xPieceHolded != wnd.mouse.GetPosX() / 100 || yPieceHolded != wnd.mouse.GetPosY() / 100) && pieceHolded->isLegalMove(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100, true)) {
			XlastMoveAfter = wnd.mouse.GetPosX() / 100;
			YlastMoveAfter = wnd.mouse.GetPosY() / 100;
			XlastMoveBefore = pieceHolded->GetX();
			YlastMoveBefore = pieceHolded->GetY();
			bool playMove = board.GetPieceAtPos(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100) == nullptr;
			board.Move(pieceHolded, wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100);
			if (!board.StillLegalMove(!colorTurn))//game end
				endSound.Play();
			if (board.isKingInCheck(!colorTurn))
				checkSound.Play();
			else {
				if (pieceHolded->GetType() == KING && abs(XlastMoveBefore - XlastMoveAfter) == 2)
					castlingSound.Play();
				else if (!playMove || pieceHolded->GetType() == PAWN && abs(XlastMoveBefore - XlastMoveAfter) == 1)
					captureSound.Play();
				else
					moveSound.Play();
			}
			colorTurn = !colorTurn;
			pieceSelected = nullptr;
		}
		pieceIsHolded = false;
		pieceHolded = nullptr;
	}
	//piece selection (show legal move)
	if (wnd.mouse.LeftIsPressed() && ableToSelected == true) {
		if (pieceSelected != nullptr && pieceSelected->isLegalMove(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100, true)) {
			XlastMoveAfter = wnd.mouse.GetPosX() / 100;
			YlastMoveAfter = wnd.mouse.GetPosY() / 100;
			XlastMoveBefore = pieceSelected->GetX();
			YlastMoveBefore = pieceSelected->GetY();
			bool playMove = board.GetPieceAtPos(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100) == nullptr;
			board.Move(pieceSelected, wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100);
			if (!board.StillLegalMove(!colorTurn))//game end
				endSound.Play();
			if (board.isKingInCheck(!colorTurn))
				checkSound.Play();
			else {
				if (playMove)
					moveSound.Play();
				else
					captureSound.Play();
			}
			pieceSelected = nullptr;
			colorTurn = !colorTurn;
		}
		else if (board.GetPieceAtPos(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100) == nullptr)
			pieceSelected = nullptr;
		else if (board.GetPieceAtPos(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100)->GetColor() == colorTurn)
			pieceSelected = board.GetPieceAtPos(wnd.mouse.GetPosX() / 100, wnd.mouse.GetPosY() / 100);
		else
			pieceSelected = nullptr;
		ableToSelected = false;
	}
	else if (!wnd.mouse.LeftIsPressed())
		ableToSelected = true;

	//cancel selection and holding
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE) || wnd.mouse.RightIsPressed()) {
		if (pieceSelected != nullptr) {
			pieceSelected = nullptr;
		}
		if (pieceHolded != nullptr) {
			pieceIsHolded = false;
			pieceHolded->Draw(true);
			pieceHolded = nullptr;
		}
	}

}
