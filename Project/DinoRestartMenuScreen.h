#ifndef DINORESTARTMENUSCREEN_H
#define DINORESTARTMENUSCREEN_H

#include "Screen.h"
#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include <vector>
#include "ScreenManager.h"

namespace Engine {
	class DinoRestartMenuScreen :
		public Engine::Screen
	{
	public:
		DinoRestartMenuScreen();
		void Init();
		void Update();
		void Draw();
		void SetFinalScore(int finalScore);
	private:
		vector<Button*> buttons;
		Text* text;
		Text* textGameOver;
		int currentButtonIndex = 0;
		Sprite* bgSprite;
		int finalScore;

	};
}

#endif