#include "Lesson12_DinoGame.h"
#include "DinoMainMenuScreen.h"
#include "DinoGameScreen.h"
#include "DinoRestartMenuScreen.h"


Engine::Lesson12_DinoGame::Lesson12_DinoGame(Setting* setting) :Engine::Game(setting)
{
	setting->windowTitle = "Dino Game";
}

Engine::Lesson12_DinoGame::~Lesson12_DinoGame()
{
}

void Engine::Lesson12_DinoGame::Init()
{
	Engine::ScreenManager::GetInstance(this)->AddScreen("ingame", new DinoGameScreen())
		->AddScreen("mainmenu", new DinoMainMenuScreen())->AddScreen("restartmenu", new DinoRestartMenuScreen())->SetCurrentScreen("mainmenu");

}

void Engine::Lesson12_DinoGame::Update()
{
	Engine::ScreenManager::GetInstance(this)->Update();
	
}

void Engine::Lesson12_DinoGame::Render()
{
	Engine::ScreenManager::GetInstance(this)->Draw();
}