#ifndef DINOGAMESCREEN_H
#define DINOGAMESCREEN_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Music.h"
#include "Sound.h"
#include "Text.h"
#include <vector>
#include "DinoMainMenuScreen.h"
#include "DinoGameScreen.h"
#include "ScreenManager.h"

namespace Engine {
	class DinoGameScreen :
		public Engine::Screen
	{
	public:
		DinoGameScreen();
		~DinoGameScreen();

		void Init();
		void Update();
		void Draw();
		void ResetGameState();
	private:
		Texture* dotTexture = NULL;
		vector<Sprite*> platforms;
		Sprite* monsterSprite = NULL;
		Sprite* backgroundSprite = NULL;
		Sprite* dotSprite1 = NULL;
		Sprite* dotSprite2 = NULL;
		Sprite* dotSprite3 = NULL;
		Sprite* dotSprite4 = NULL;
		Sprite* dotSprite11 = NULL;
		Sprite* dotSprite22 = NULL;
		Sprite* dotSprite33 = NULL;
		Sprite* dotSprite44 = NULL;
		Music* music = NULL;;
		Sound* sound = NULL;
		Text* textScore = NULL;
		Text* textGameOver = NULL;
		Text* textRestart = NULL;

		float yVelocity = 0, gravity = 0;
		bool jump = false, debug = false;
		bool gameStarted = false;
		bool gameOver = false;
		vector<Sprite*> obstacles;
		float obstacleSpawnTimer = 0;
		float spawnInterval = 1500;
		int score = 0;              // Variabel skor
		float scoreUpdateTime = 0;   // Waktu untuk menambah skor

		float cactusSpeed = 0.5f;
		float maxCactusSpeed = 2.0f;
		float minSpawnInterval = 750;
		float timeElapsed = 0;

		vector<Sprite*> backgrounds, middlegrounds, foregrounds;
		std::vector<std::string> cactusTextures;
		void MoveLayer(vector<Sprite*>& bg, float speed);
		void DrawLayer(vector<Sprite*>& bg);
		void AddToLayer(vector<Sprite*>& bg, const std::string& name);
		void spawnObstacle();
		float offset = 0;
	};
}
#endif

