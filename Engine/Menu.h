#pragma once
#include "Graphics.h"
#include "MainWindow.h"
#include "Rec.h"

class Menu
{
public:
	Menu();
	void DrawMenu(Graphics& gfx);
	Rec Get1v1ButtonRec();
	Rec GetAgainstBotButtonRec();
private:
	void Draw1v1Button(Graphics& gfx);
	void DrawAgainstBotButton(Graphics& gfx);
	void DrawTitle(Graphics& gfx);
private:
	const int padding = 50;
	Rec _1v1Button;
	Rec AgainstBotButton;
};

