#include "DinoRestartMenuScreen.h"
#include "DinoGameScreen.h"

Engine::DinoRestartMenuScreen::DinoRestartMenuScreen()
{
    text = NULL;
    bgSprite = NULL;
    textGameOver = NULL;

}

void Engine::DinoRestartMenuScreen::Init()
{
    // Create background texture and sprite
    Texture* texture = new Texture("buttons.png");
    Texture* restartTexture = new Texture("restart.png");

    Texture* bgTexture = new Texture("background.png");

    bgSprite = new Sprite(bgTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
    bgSprite->SetSize(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight);

    // Create Restart Button
    Sprite* restartSprite = (new Sprite(restartTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
        ->SetNumXFrames(1)->SetNumYFrames(1)->AddAnimation("normal", 1, 1)
        ->AddAnimation("hover", 1, 1)->AddAnimation("press", 1, 1)->SetAnimationDuration(400);
    restartSprite->SetSize(120, 120); 
    Button* restartButton = new Button(restartSprite, "restart");
    restartButton->SetPosition((game->GetSettings()->screenWidth / 2) - (restartSprite->GetScaleWidth() / 2), 500);
    buttons.push_back(restartButton);

    // Create Exit Button
    Sprite* exitSprite = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
        ->SetNumXFrames(6)->SetNumYFrames(1)->AddAnimation("normal", 2, 2)
        ->AddAnimation("hover", 0, 1)->AddAnimation("press", 0, 1)->SetAnimationDuration(400);
    Button* exitButton = new Button(exitSprite, "exit");
    exitButton->SetPosition((game->GetSettings()->screenWidth / 2) - (exitSprite->GetScaleWidth() / 2), 350);
    buttons.push_back(exitButton);

    // Set active button
    currentButtonIndex = 0;
    buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);

    // Create Text
    text = (new Text("8-bit Arcade In.ttf", 200, game->GetDefaultTextShader()))
        ->SetText("Game Over")->SetPosition(game->GetSettings()->screenWidth * 0.5f - 400, game->GetSettings()->screenHeight - 200.0f)->SetColor(213, 168, 134);
    
    textGameOver = (new Text("lucon.ttf", 30, game->GetDefaultTextShader()))
        ->SetText("Final Score: 0")  // Set teks awal
        ->SetPosition(game->GetSettings()->screenWidth * 0.5f - 200, game->GetSettings()->screenHeight - 150)
        ->SetColor(213, 168, 134);

    // Add input mappings
    game->GetInputManager()->AddInputMapping("next", SDLK_RIGHT)
        ->AddInputMapping("prev", SDLK_LEFT)
        ->AddInputMapping("press", SDLK_SPACE);
}

void Engine::DinoRestartMenuScreen::Update()
{
    // Update background color
    game->SetBackgroundColor(52, 155, 235);

    // Navigate through buttons
    if (game->GetInputManager()->IsKeyReleased("next")) {
        buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
        currentButtonIndex = (currentButtonIndex < (int)buttons.size() - 1) ? currentButtonIndex + 1 : currentButtonIndex;
        buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
    }

    if (game->GetInputManager()->IsKeyReleased("prev")) {
        buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
        currentButtonIndex = currentButtonIndex > 0 ? currentButtonIndex - 1 : currentButtonIndex;
        buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
    }

    // Handle button press
    if (game->GetInputManager()->IsKeyReleased("press")) {
        Button* b = buttons[currentButtonIndex];
        b->SetButtonState(Engine::ButtonState::PRESS);

        if (b->GetButtonName() == "restart") {
      ScreenManager::GetInstance(game)->SetCurrentScreen("ingame"); // Kembali ke ingame

            // Panggil metode RestartGame() pada instance dari DinoGameScreen
            DinoGameScreen* gameScreen = dynamic_cast<DinoGameScreen*>(ScreenManager::GetInstance(game)->GetCurrentScreen());
            if (gameScreen) {
                gameScreen->ResetGameState(); 
            }
        }
        else if (b->GetButtonName() == "exit") {
            game->SetState(Engine::State::EXIT);
        }
    }

    // Update buttons
    for (Button* b : buttons) {
        b->Update(game->GetGameTime());
    }
}

void Engine::DinoRestartMenuScreen::Draw()
{
    bgSprite->Draw();

    for (Button* b : buttons) {
        b->Draw();
    }

    text->Draw();
    textGameOver->Draw();
}

void Engine::DinoRestartMenuScreen::SetFinalScore(int finalScore) {
    std::string finalScoreText = "Final Score: " + std::to_string(finalScore);
    //textGameOver->SetScale(3.0f)->SetText("Game Over! Final Score: " + std::to_string(score))->SetPosition(game->GetSettings()->screenWidth * 0.5f - 500, game->GetSettings()->screenHeight - 500.0f)->SetColor(0, 0, 0);

    textGameOver->SetText(finalScoreText)->SetPosition(game->GetSettings()->screenWidth * 0.5f - 130, game->GetSettings()->screenHeight - 240.0f)->SetColor(213, 168, 134);
}