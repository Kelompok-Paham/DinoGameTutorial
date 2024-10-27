#include "DinoGameScreen.h"
#include "DinoRestartMenuScreen.h"


Engine::DinoGameScreen::DinoGameScreen()
{
}
Engine ::DinoGameScreen::~DinoGameScreen() 
{
	for (Sprite* s : backgrounds) delete s;
	for (Sprite* s : middlegrounds) delete s;
	for (Sprite* s : foregrounds) delete s;
	for (Sprite* obstacle : obstacles) delete obstacle;
	delete dotTexture;
	delete monsterSprite;
	delete dotSprite1;
	delete dotSprite2;
	delete dotSprite3;
	delete dotSprite4;
	delete dotSprite11;
	delete dotSprite22;
	delete dotSprite33;
	delete dotSprite44;
	delete music;
	delete sound;
	delete textScore;
	delete textGameOver;
	delete textRestart;

}

void Engine::DinoGameScreen::Init()
{
	//Create background
	for (int i = 1; i <= 2; i++) {
		AddToLayer(backgrounds, "bgdesert" + to_string(i) + ".png");
	}
	for (int i = 3; i <= 4; i++) {
		AddToLayer(middlegrounds, "bgdesert" + to_string(i) + ".png");
	}
	for (int i = 5; i <= 5; i++) {
		AddToLayer(foregrounds, "bgdesert" + to_string(i) + ".png");
	}

	offset = 2;


	Texture* monsterTexture = new Texture("Dino.png");

	monsterSprite = new Sprite(monsterTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	monsterSprite->SetPosition(90, 0)->SetNumXFrames(24)->SetNumYFrames(1)->SetAnimationDuration(70)->SetScale(5.0f)->AddAnimation("attack", 13, 14)->AddAnimation("idle", 17, 20)->AddAnimation("run", 17, 20)->AddAnimation("jump", 17, 20);
	monsterSprite->SetBoundingBoxSize(monsterSprite->GetScaleWidth() - (43 * monsterSprite->GetScale()),
		monsterSprite->GetScaleHeight() - (4 * monsterSprite->GetScale()));

	//cactus textures
	cactusTextures = { "cactus1.png", "cactus2.png", "cactus3.png" };


	// This dot sprite is for visual debugging (to see the actual bounding box) 
	dotTexture = new Texture("dot.png");
	dotSprite1 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	dotSprite2 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	dotSprite3 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	dotSprite4 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	dotSprite11 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	dotSprite22 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	dotSprite33 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	dotSprite44 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	// Add input mapping 
	game->GetInputManager()/*->AddInputMapping("restart", SDLK_r)*/->AddInputMapping("Jump", SDLK_UP)->AddInputMapping("Attack", SDLK_x)->AddInputMapping("Run Right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT)->AddInputMapping("Run Left", SDL_CONTROLLER_BUTTON_DPAD_LEFT)->AddInputMapping("Jump", SDL_CONTROLLER_BUTTON_A)->AddInputMapping("Attack", SDL_CONTROLLER_BUTTON_X)->AddInputMapping("Quit", SDLK_ESCAPE)->AddInputMapping("Quit", SDL_CONTROLLER_BUTTON_Y);

	// Playing music
	music = (new Music("2021-08-16_-_8_Bit_Adventure_-_www.FesliyanStudios.com.ogg"))->SetVolume(40)->Play(true);

	sound = (new Sound("jump.wav"))->SetVolume(100);

	textScore = new Text("lucon.ttf", 24, game->GetDefaultTextShader());
	textGameOver = new Text("lucon.ttf", 24, game->GetDefaultTextShader());
	textRestart = new Text("lucon.ttf", 24, game->GetDefaultTextShader());

	int offset = 20; // Atur offset sesuai kebutuhan
	textScore->SetScale(2.0f)->SetColor(0, 0, 0)->SetPosition(50, game->GetSettings()->screenHeight - (textScore->GetFontSize() * textScore->GetScale()) - offset);
	//Set the background color
	game->SetBackgroundColor(102, 195, 242);
	// Set debug
	debug = false;

	// Seed for random number generation
	srand(static_cast<unsigned int>(time(0)));
	// Inisialisasi obstacle spawn timer
	obstacleSpawnTimer = 0;
	// Add input mappings
	game->GetInputManager()->AddInputMapping("mainmenu", SDLK_ESCAPE);

}

void Engine::DinoGameScreen::ResetGameState() {
	// Reset posisi monster dan kecepatan
	monsterSprite->SetPosition(90, 0);
	yVelocity = 0;
	jump = false;

	// Reset skor dan timer
	score = 0;
	scoreUpdateTime = 0;
	obstacleSpawnTimer = 0;
	timeElapsed = 0;
	spawnInterval = 1500;
	cactusSpeed = 0.5f;

	// Reset obstacles
	for (Sprite* obstacle : obstacles) {
		delete obstacle;
	}
	obstacles.clear();
	obstacleSpawnTimer = 0;

	// Reset game over flag
	gameOver = false;

	// Restart music if needed
	if (music) {
		music->Play(true);
	}
}

void Engine::DinoGameScreen::Update()
{
	
	if (!gameOver) {
		// Update parallax scrolling
		MoveLayer(backgrounds, 0.005f);
		MoveLayer(middlegrounds, 0.03f);
		MoveLayer(foregrounds, cactusSpeed);

		if (game->GetInputManager()->IsKeyReleased("mainmenu")) {
			// Mengatur ScreenManager untuk beralih ke layar menu utama
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
		}
		scoreUpdateTime += game->GetGameTime();  // Hitung waktu berlalu
		if (scoreUpdateTime >= 500) {      // Tambahkan skor setiap 500ms
			score += 1;
			scoreUpdateTime = 0;           // Reset timer
		}

		textScore->SetText("Score: " + std::to_string(score));
		monsterSprite->PlayAnim("idle");

		// Move monster sprite using keyboard
		vec2 oldMonsterPos = monsterSprite->GetPosition();
		float x = oldMonsterPos.x, y = oldMonsterPos.y;

		monsterSprite->SetPosition(x, y);

		if (game->GetInputManager()->IsKeyPressed("Jump") && !jump) {
			//Set gravity and yVelocity
			float ratio = (game->GetGameTime() / 16.7f);		//kecepatan berbeda-beda supaya konsisten dengan berbagai macam nilai fps dicari rasionya bisa juga dilimit target frame ratenya
			gravity = 0.075f * ratio;
			yVelocity = 1.8f;
			jump = true;
			sound->Play(false);
		}

		if (y > 0) {
			yVelocity -= gravity;
		}
		else if (y < 0) {
			jump = false;
			yVelocity = 0;
			y = 0;
		}

		y += yVelocity * game->GetGameTime();
		monsterSprite->SetPosition(x, y);

		if (game->GetInputManager()->IsKeyPressed("Jump")) {
			monsterSprite->PlayAnim("jump");
		}

		// Update monster sprite animation
		monsterSprite->Update(game->GetGameTime());


		// tambah kecepatan
			timeElapsed += game->GetGameTime();

		if (timeElapsed >= 10000) { // Setiap 10 detik
			spawnInterval -= 100; // tambahkan interval sebesar 20ms
			cactusSpeed += 0.2f;
			if (spawnInterval < minSpawnInterval) {
				spawnInterval = minSpawnInterval;
			}
			if (cactusSpeed > maxCactusSpeed) {
				cactusSpeed = maxCactusSpeed; // Batasi kecepatan 
			}
			timeElapsed = 0; // Reset timer
		}
		// Spawn cactus (obstacle) based on timer
		obstacleSpawnTimer += game->GetGameTime();
		int randomSpawnTime = rand() % 4000 + spawnInterval;  // Random between 500ms to 2000ms

		if (obstacleSpawnTimer >= randomSpawnTime) {
			spawnObstacle();
			obstacleSpawnTimer = 0;  // Reset timer
		}

		for (auto it = obstacles.begin(); it != obstacles.end(); ) {
			Sprite* obstacle = *it;
			obstacle->SetPosition(obstacle->GetPosition().x - cactusSpeed * game->GetGameTime(), obstacle->GetPosition().y);

			// Check collision with monster
			if (monsterSprite->GetBoundingBox()->CollideWith(obstacle->GetBoundingBox())) {
				gameOver = true;
				
				// Mengganti ke layar restart menu saat game over
				ScreenManager::GetInstance(game)->SetCurrentScreen("restartmenu");
				// Kirim skor ke layar DinoRestartMenuScreen
				DinoRestartMenuScreen* restartMenu = dynamic_cast<DinoRestartMenuScreen*>(ScreenManager::GetInstance(game)->GetCurrentScreen());
				if (restartMenu) {
					restartMenu->SetFinalScore(score);  // Set skor akhir sebelum berpindah layar
				}
				return;
			}

			// Remove obstacle if it goes off-screen
			if (obstacle->GetPosition().x < -150) {
				delete obstacle;
				it = obstacles.erase(it);
			}
			else {
				++it;
			}
		}

		if (debug) {
			//Shape for debug
			BoundingBox* bb = monsterSprite->GetBoundingBox();
			dotSprite1->SetPosition(bb->GetVertices()[0].x - (0.5f * dotSprite1->GetScaleWidth()), bb->GetVertices()[0].y - (0.5f * dotSprite1->GetScaleHeight()));
			dotSprite2->SetPosition(bb->GetVertices()[1].x - (0.5f * dotSprite2->GetScaleWidth()), bb->GetVertices()[1].y - (0.5f * dotSprite2->GetScaleHeight()));
			dotSprite3->SetPosition(bb->GetVertices()[2].x - (0.5f * dotSprite3->GetScaleWidth()), bb->GetVertices()[2].y - (0.5f * dotSprite3->GetScaleHeight()));
			dotSprite4->SetPosition(bb->GetVertices()[3].x - (0.5f * dotSprite4->GetScaleWidth()), bb->GetVertices()[3].y - (0.5f * dotSprite4->GetScaleHeight()));

			for (Sprite* obstacle : obstacles) {
				BoundingBox* bbObstacle = obstacle->GetBoundingBox();
				dotSprite11->SetPosition(bbObstacle->GetVertices()[0].x - (0.5f * dotSprite11->GetScaleWidth()), bbObstacle->GetVertices()[0].y - (0.5f * dotSprite11->GetScaleHeight()));
				dotSprite22->SetPosition(bbObstacle->GetVertices()[1].x - (0.5f * dotSprite22->GetScaleWidth()), bbObstacle->GetVertices()[1].y - (0.5f * dotSprite22->GetScaleHeight()));
				dotSprite33->SetPosition(bbObstacle->GetVertices()[2].x - (0.5f * dotSprite33->GetScaleWidth()), bbObstacle->GetVertices()[2].y - (0.5f * dotSprite33->GetScaleHeight()));
				dotSprite44->SetPosition(bbObstacle->GetVertices()[3].x - (0.5f * dotSprite44->GetScaleWidth()), bbObstacle->GetVertices()[3].y - (0.5f * dotSprite44->GetScaleHeight()));
			}

		}
	}
}

//void Engine::DinoGameScreen::Render()
void Engine::DinoGameScreen::Draw()
{
	DrawLayer(backgrounds);
	DrawLayer(middlegrounds);
	DrawLayer(foregrounds);
	textScore->Draw();
	textGameOver->Draw();
	textRestart->Draw();


	for (Sprite* obstacle : obstacles) {
		obstacle->Draw();
	}

	monsterSprite->Draw();

	if (debug) {
		dotSprite1->Draw();
		dotSprite2->Draw();
		dotSprite3->Draw();
		dotSprite4->Draw();

		dotSprite11->Draw();
		dotSprite22->Draw();
		dotSprite33->Draw();
		dotSprite44->Draw();
	}
}

void Engine::DinoGameScreen::MoveLayer(vector<Sprite*>& bg, float speed)
{
	for (Sprite* s : bg) {
		if (s->GetPosition().x < -game->GetSettings()->screenWidth + offset) {
			s->SetPosition(game->GetSettings()->screenWidth + offset - 1, 0);
		}
		s->SetPosition(s->GetPosition().x - speed * game->GetGameTime(), s->GetPosition().y);
		s->Update(game->GetGameTime());
	}
		}

void Engine::DinoGameScreen::DrawLayer(vector<Sprite*>& bg)
{
	for (Sprite* s : bg) {
		s->Draw();
	}
}

void Engine::DinoGameScreen::AddToLayer(vector<Sprite*>& bg, const std::string& name)
{
	Texture* texture = new Texture(name);

	Sprite* s = new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	s->SetSize(game->GetSettings()->screenWidth + offset, game->GetSettings()->screenHeight);
	bg.push_back(s);

	Sprite* s2 = new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	s2->SetSize(game->GetSettings()->screenWidth + offset, game->GetSettings()->screenHeight)->SetPosition(game->GetSettings()->screenWidth + offset - 1, 0);
	bg.push_back(s2);
}

// Fungsi untuk spawn obstacle (cactus)
void Engine::DinoGameScreen::spawnObstacle()
{
	// Pilih cactus texture secara acak
	int randomIndex = rand() % cactusTextures.size();
	Texture* cactusTexture = new Texture(cactusTextures[randomIndex]);

	// Buat sprite cactus
	Sprite* cactus = new Sprite(cactusTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	cactus->SetScale(4.0f);
	cactus->SetPosition(game->GetSettings()->screenWidth, 0); // Posisi awal di sisi kanan layar
	cactus->SetBoundingBoxSize(cactus->GetScaleWidth() - (16 * cactus->GetScale()), cactus->GetScaleHeight() - (4 * cactus->GetScale()));

	// Tambahkan cactus ke dalam daftar obstacles
	obstacles.push_back(cactus);
}