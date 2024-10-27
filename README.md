
## 1. Main Menu Screen
Berikut adalah tutorial untuk membuat Main Menu Screen pada game Dino menggunakan Screen Manager. main menu screen game ini melibatkan dua kelas utama: ScreenManager untuk mengelola layar (screen) dan DinoMainMenuScreen sebagai layar utama (main menu screen) game

### Kelas ScreenManager
Kelas ScreenManager bertanggung jawab untuk mengelola tampilan layar yang aktif dalam game. Berikut fungsi yang ada:


*   Singleton Pattern: ScreenManager menggunakan singleton pattern untuk memastikan hanya ada satu instance ScreenManager di seluruh game. Ini diimplementasikan dengan fungsi GetInstance yang menerima parameter Game* engine.
*   Fungsi AddScreen: Digunakan untuk menambahkan layar baru (seperti Main Menu Screen) ke dalam ScreenManager.
*   Fungsi SetCurrentScreen: Untuk mengubah layar yang sedang aktif saat ini.
*   Fungsi Update dan Draw: Untuk memperbarui dan menggambar layar yang sedang aktif.


Pada ScreenManager.cpp, kita inisialisasi instance tunggal, dan implementasi fungsi seperti AddScreen, SetCurrentScreen, Update, dan Draw.

### Kelas DinoMainMenuScreen
Kelas DinoMainMenuScreen adalah turunan dari Screen yang berfungsi sebagai layar utama pada game. Berikut ini beberapa elemen penting dalam DinoMainMenuScreen:


*   Inisialisasi Background dan Tombol: Fungsi Init() membuat sprite untuk latar belakang dan tombol seperti "Play" dan "Exit".
*   Navigasi Tombol: Menggunakan tombol panah kiri dan kanan untuk berpindah antar tombol dan tombol SPACE untuk menekan tombol yang dipilih.
*   Fungsi Update dan Draw: Update() menangani logika navigasi dan interaksi, sedangkan Draw() untuk menggambar semua elemen layar utama.

Berikut implementasi DinoMainMenuScreen.h:

```cpp
#ifndef DINOMAINMENUSCREEN_H
#define DINOMAINMENUSCREEN_H

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
	class DinoMainMenuScreen :
		public Engine::Screen
	{
	public:
		DinoMainMenuScreen();
		void Init();
		void Update();
		void Draw();
	private:
		vector<Button*> buttons;
		Text* text;
		int currentButtonIndex = 0;
		Sprite* bgSprite;

	};
}

#endif
```
Berikut implementasi DinoMainMenuScreen.cpp:

```cpp
#include "DinoMainMenuScreen.h"
Engine::DinoMainMenuScreen::DinoMainMenuScreen()
{
	text = NULL;
	bgSprite = NULL;
}
void Engine::DinoMainMenuScreen::Init()
{
	// Create a Texture
	Texture* texture = new Texture("buttons.png");
	Texture* bgTexture = new Texture("background.png");
	bgSprite = new Sprite(bgTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	bgSprite->SetSize(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight);
	// Create Sprites
	Sprite* playSprite = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(6)->SetNumYFrames(1)->AddAnimation("normal", 5, 5)->AddAnimation("hover", 3, 4)
		->AddAnimation("press", 3, 4)->SetAnimationDuration(400);

	Sprite* exitSprite = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(6)->SetNumYFrames(1)->AddAnimation("normal", 2, 2)->AddAnimation("hover", 0, 1)
		->AddAnimation("press", 0, 1)->SetAnimationDuration(400);
	//Create Buttons
	Button* playButton = new Button(playSprite, "play");
	playButton->SetPosition((game->GetSettings()->screenWidth / 2) - (playSprite->GetScaleWidth() / 2),
		500);
	buttons.push_back(playButton);
	Button* exitButton = new Button(exitSprite, "exit");
	exitButton->SetPosition((game->GetSettings()->screenWidth / 2) - (exitSprite->GetScaleWidth() / 2),
		350);
	buttons.push_back(exitButton);
	// Set play button into active button
	currentButtonIndex = 0;
	buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	// Create Text
	text = (new Text("8-bit Arcade In.ttf", 200, game->GetDefaultTextShader()))
		->SetText("The Dino Game")->SetPosition(game->GetSettings()->screenWidth * 0.5f - 550, game->GetSettings()->screenHeight - 200.0f)->SetColor(255, 241, 193);
	// Add input mappings
	game->GetInputManager()->AddInputMapping("next", SDLK_RIGHT)
		->AddInputMapping("prev", SDLK_LEFT)
		->AddInputMapping("press", SDLK_SPACE);
}
void Engine::DinoMainMenuScreen::Update()
{
	// Set background
	game->SetBackgroundColor(52, 155, 235);
	if (game->GetInputManager()->IsKeyReleased("next")) {
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Next Button
		currentButtonIndex = (currentButtonIndex < (int)buttons.size() - 1) ? currentButtonIndex + 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}
	if (game->GetInputManager()->IsKeyReleased("prev")) {
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Prev Button
		currentButtonIndex = currentButtonIndex > 0 ? currentButtonIndex - 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}
	if (game->GetInputManager()->IsKeyReleased("press")) {
		// Set current button to press state
		Button* b = buttons[currentButtonIndex];
		b->SetButtonState(Engine::ButtonState::PRESS);
		// If play button then go to InGame, exit button then exit
		if ("play" == b->GetButtonName()) {
			ScreenManager::GetInstance(game)->SetCurrentScreen("ingame");
		}
		else if ("exit" == b->GetButtonName()) {
			game->SetState(Engine::State::EXIT);
		}
	}
	// Update All buttons
	for (Button* b : buttons) {
		b->Update(game->GetGameTime());
	}
}
void Engine::DinoMainMenuScreen::Draw()
{
	bgSprite->Draw();

	// Render all buttons
	for (Button* b : buttons) {
		b->Draw();
	}
	// Render title 
	text->Draw();
}
```

## 3. Game Screen
Kelas DinoGameScreen berfungsi sebagai papan permainan game yang dibuat. Dalam kelas ini, kita akan membuat elemen-elemen utama seperti latar belakang, karakter, musik, text, dan berbagai komponen lainnya yang diperlukan untuk game. Pada kelas ini juga ditentukan apa yang bisa dilakukan oleh pemain, bagaimana rintangan dimunculkan, peraturan skor, dan sebagainya. 

Pada file DinoGameScreen.h, didefinisikan fungsi dan variabel yang diperlukan. Berikut adalah implementasi dari file DinoGameScreen.h:

```cpp
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
```

Semua deklarasi yang ada pada file DinoGameScreen.h kemudian diimplementasikan di file DinoGameScreen.cpp. Isi dari file DinoGameScreen.cpp ini mencakup:
### a. Konstruktor dan Destruktor
*   Konstruktor

Konstruktor DinoGameScreen diinisialisasi secara kosong. Sementara itu, variabel seperti sprites, musik, dan textures akan diinisialisasi pada fungsi Init().
```cpp
Engine::DinoGameScreen::DinoGameScreen(){}
```
*   Destruktor

Destruktor berfungsi untuk memastikan semua alokasi memori dibersihkan untuk menghindari memory leak. Destruktor menghapus setiap elemen dalam daftar backgrounds, middlegrounds, foregrounds, dan obstacles, serta menghapus elemen-elemen lainnya seperti dotTexture, monsterSprite, dan text.
```cpp
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
```
### b. Inisialisasi Game di Fungsi Init()
Fungsi `Init()` digunakan untuk menyiapkan *assets* game seperti latar belakang, karakter, input kontrol, dan musik.

*   Membuat Latar Belakang

Latar belakang, *middleground*, dan *foreground* ditambahkan dalam beberapa lapisan dengan metode `AddToLayer()`. Latar belakang akan bergerak dengan kecepatan berbeda untuk menghasilkan efek paralaks.

```cpp
for (int i = 1; i <= 2; i++) {
    AddToLayer(backgrounds, "bgdesert" + to_string(i) + ".png");
}
for (int i = 3; i <= 4; i++) {
    AddToLayer(middlegrounds, "bgdesert" + to_string(i) + ".png");
}
for (int i = 5; i <= 5; i++) {
    AddToLayer(foregrounds, "bgdesert" + to_string(i) + ".png");
}
```

*   Membuat Karakter

Karakter utama (dino) diwakili oleh `monsterSprite`. Sprite ini menggunakan gambar `Dino.png`, dengan pengaturan animasi untuk berbagai aksi seperti `attack`, `idle`, `run`, dan `jump`.

```cpp
Texture* monsterTexture = new Texture("Dino.png");

monsterSprite = new Sprite(monsterTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
monsterSprite->SetPosition(90, 0)->SetNumXFrames(24)->SetNumYFrames(1)->SetAnimationDuration(70)->SetScale(5.0f)->AddAnimation("attack", 13, 14)->AddAnimation("idle", 17, 20)->AddAnimation("run", 17, 20)->AddAnimation("jump", 17, 20);
monsterSprite->SetBoundingBoxSize(monsterSprite->GetScaleWidth() - (43 * monsterSprite->GetScale()), monsterSprite->GetScaleHeight() - (4 * monsterSprite->GetScale()));
```

*   Inisialisasi Cactus Texture

Kaktus-kaktus sebagai rintangan dalam game diatur dengan daftar gambar `cactusTextures` yang akan di-*spawn* secara acak saat game berjalan.

```cpp
cactusTextures = { "cactus1.png", "cactus2.png", "cactus3.png" };
```

*   Visual Debugging dengan Dot Sprite

Dot sprite digunakan untuk debugging, memperlihatkan *bounding box* dari karakter dan rintangan untuk memastikan deteksi benturan berfungsi dengan baik.

```cpp
dotTexture = new Texture("dot.png");
dotSprite1 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
dotSprite2 = new Sprite(dotTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
// Lanjutkan inisialisasi dotSprite lainnya
```

*   Menambahkan Input Mapping

Input mapping ditambahkan untuk mengatur kontrol karakter seperti lompat (`Jump`), serang (`Attack`), dan navigasi lainnya. Setiap kontrol dikaitkan dengan tombol tertentu, misalnya, `Jump` diatur dengan tombol panah `UP`.

```cpp
game->GetInputManager()->AddInputMapping("Jump", SDLK_UP)->AddInputMapping("Attack", SDLK_x)->AddInputMapping("Run Right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
```

*   Inisialisasi Musik dan Efek Suara

Musik dan efek suara dimuat menggunakan objek `Music` dan `Sound`. Musik diputar secara berulang, sedangkan efek suara dimainkan saat terjadi aksi tertentu.

```cpp
music = (new Music("2021-08-16_-_8_Bit_Adventure_-_www.FesliyanStudios.com.ogg"))->SetVolume(40)->Play(true);
sound = (new Sound("jump.wav"))->SetVolume(100);
```

*   Inisialisasi Skor dan Teks Game Over

Teks skor (`textScore`) diatur untuk menampilkan nilai skor pada layar, sedangkan `textGameOver` akan muncul saat pemain kalah.

```cpp
textScore = new Text("lucon.ttf", 24, game->GetDefaultTextShader());
textScore->SetScale(2.0f)->SetColor(0, 0, 0)->SetPosition(0, game->GetSettings()->screenHeight - (textScore->GetFontSize() * textScore->GetScale()));
```

*   Pengaturan Warna Latar

Warna latar belakang diset ke nilai RGB tertentu.

```cpp
game->SetBackgroundColor(102, 195, 242);
```

### c. Fungsi Reset Game (ResetGameState)
Fungsi ini digunakan untuk mengatur ulang status permainan setiap kali game dimulai ulang. Ini mencakup pengaturan posisi karakter, skor, *timer*, dan mengosongkan daftar *obstacles*.

```cpp
void Engine::DinoGameScreen::ResetGameState() {
    monsterSprite->SetPosition(90, 0);
    yVelocity = 0;
    jump = false;
    score = 0;
    scoreUpdateTime = 0;
    obstacleSpawnTimer = 0;
    timeElapsed = 0;
    spawnInterval = 1500;
    cactusSpeed = 0.5f;

    for (Sprite* obstacle : obstacles) {
        delete obstacle;
    }
    obstacles.clear();
    obstacleSpawnTimer = 0;
    gameOver = false;

    if (music) {
        music->Play(true);
    }
}
```
### d. fungsi update
Berikut adalah tutorial langkah demi langkah untuk memahami dan mengimplementasikan fungsi `Update` dari kelas `DinoGameScreen`:

*   Parallax Scrolling

Pada bagian ini, fungsi `MoveLayer` digunakan untuk menggerakkan layer background dengan kecepatan yang berbeda, menciptakan efek **parallax scrolling**. Lapisan-lapisan bergerak pada kecepatan yang semakin cepat dari latar belakang ke latar depan (`backgrounds`, `middlegrounds`, `foregrounds`), yang memberi efek kedalaman visual pada game.

```cpp
MoveLayer(backgrounds, 0.005f);
MoveLayer(middlegrounds, 0.03f);
MoveLayer(foregrounds, cactusSpeed);
```

*   Mengatur Kembali ke Menu Utama

Ketika tombol yang dipetakan sebagai `mainmenu` (biasanya tombol `Escape`) dilepaskan, layar akan beralih ke menu utama menggunakan `ScreenManager`.
```cpp
if (game->GetInputManager()->IsKeyReleased("mainmenu")) {
ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
}
```

*   Menghitung dan Menambah Skor

Setiap 500 ms, nilai `score` bertambah satu. `scoreUpdateTime` direset setelah penambahan skor. Variabel `textScore` kemudian diperbarui agar menampilkan skor terbaru di layar.
```cpp
scoreUpdateTime += game->GetGameTime();  // Menghitung waktu berlalu
if (scoreUpdateTime >= 500) {            // Tambahkan skor setiap 500ms
score += 1;
scoreUpdateTime = 0;                 // Reset timer
}
textScore->SetText("Score: " + std::to_string(score));
```

*   Mengatur Animasi Monster

Untuk setiap frame baru, animasi `idle` dari monster akan dimainkan. Animasi ini bisa diganti nanti tergantung pada aksi tertentu seperti melompat atau menyerang.
```cpp
monsterSprite->PlayAnim("idle");
```

*   Menangkap Input untuk Menggerakkan Karakter

Posisi awal monster diambil dan disimpan dalam variabel `x` dan `y`. Hal ini memungkinkan karakter berada di posisi yang sama jika tidak ada aksi tertentu.
```cpp
vec2 oldMonsterPos = monsterSprite->GetPosition();
float x = oldMonsterPos.x, y = oldMonsterPos.y;
monsterSprite->SetPosition(x, y);
```

*   Menangani Input Jump

Ketika tombol `Jump` ditekan dan monster sedang tidak dalam status `jump`, nilai `yVelocity` diatur, yang memungkinkan karakter melompat. **Gravitasi** ditambahkan untuk mempercepat laju jatuhnya karakter saat berada di udara.
```cpp
if (game->GetInputManager()->IsKeyPressed("Jump") && !jump) {
float ratio = (game->GetGameTime() / 16.7f); // Rasio FPS untuk kecepatan
gravity = 0.075f * ratio;
yVelocity = 1.8f;
jump = true;
sound->Play(false);
}
```

## 5. Restart Menu
Fungsi-fungsi di dalam kelas `DinoRestartMenuScreen` bertujuan untuk menyediakan tampilan dan interaksi menu restart ketika pemain kalah. Kelas ini mengelola semua aspek visual dan logika navigasi dalam layar restart, memastikan pengguna dapat memilih antara mengulang permainan atau keluar.

### Penjelasan Fungsi
#### 1. Konstruktor DinoRestartMenuScreen()
##### Fungsi:
Konstruktor ini bertujuan untuk inisialisasi awal variabel-variabel anggota dalam kelas DinoRestartMenuScreen, yaitu text, bgSprite, dan textGameOver, yang diatur ke NULL sebagai nilai awal.
##### Penjelasan:
Dengan mengatur nilai awal variabel ke NULL, konstruktor memastikan bahwa variabel-variabel tersebut tidak memiliki nilai sampah yang bisa menyebabkan masalah saat digunakan.

#### Kode
```cpp
Engine::DinoRestartMenuScreen::DinoRestartMenuScreen()
{
    text = NULL;
    bgSprite = NULL;
    textGameOver = NULL;

}
```

#### 2. Init()
##### Fungsi:
Init() bertugas untuk menginisialisasi elemen-elemen visual dan input pada layar restart game, termasuk:
* Latar belakang (bgSprite)
* Tombol restart dan tombol keluar
* Teks "Game Over" dan skor akhir
* Input navigasi tombol menggunakan panah kanan dan kiri serta tombol Space untuk memilih.
##### Penjelasan:
* Membuat sprite latar belakang yang menyesuaikan dengan ukuran layar game.
* Membuat tombol restart dan tombol keluar (restartButton dan exitButton), menambahkan animasi normal, hover, dan press untuk efek visual saat tombol dipilih atau ditekan, dan menentukan posisi tombol di layar.
* Membuat teks "Game Over" dan skor akhir dengan pengaturan warna, font, dan ukuran yang sesuai.
* Menambahkan input navigasi untuk berpindah antar tombol dan mengonfirmasi pilihan.
* Menyimpan tombol dalam daftar buttons, serta mengatur tombol restartButton sebagai tombol default yang dipilih (dengan status HOVER).

#### Kode
```cpp
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
```

#### 3. Update()
##### Fungsi:
Update() menangani logika yang berkaitan dengan interaksi pengguna, yaitu navigasi antar tombol dan pemrosesan ketika tombol ditekan.
##### Penjelasan:
* Mengubah warna latar belakang sesuai dengan kebutuhan tampilan menu restart.
* Menangani navigasi antar tombol dengan memeriksa apakah pengguna menekan next (panah kanan) atau prev (panah kiri), lalu memperbarui status tombol yang sedang dipilih.
* Jika pengguna menekan tombol press (tombol Space), Update() mengecek tombol mana yang dipilih dan menentukan tindakan:
* Tombol Restart: Mengganti layar saat ini ke DinoGameScreen (layar utama game) dan memanggil ResetGameState() untuk memulai ulang permainan.
* Tombol Exit: Mengatur status game ke EXIT, yang menandakan game akan keluar.

#### Kode
```cpp
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
```

#### 4. Draw()
##### Fungsi:
Draw() bertugas untuk menggambar elemen-elemen visual di layar, termasuk latar belakang, tombol-tombol, serta teks "Game Over" dan skor akhir.
##### Penjelasan:
* Memanggil Draw() pada bgSprite untuk menampilkan latar belakang.
* Melakukan loop melalui daftar buttons dan memanggil Draw() pada setiap tombol untuk menampilkan status terkini (normal, hover, atau press).
* Menampilkan teks "Game Over" dan teks skor akhir (text dan textGameOver).

#### Kode
```cpp
void Engine::DinoRestartMenuScreen::Draw()
{
    bgSprite->Draw();

    for (Button* b : buttons) {
        b->Draw();
    }

    text->Draw();
    textGameOver->Draw();
}
```

#### 5. SetFinalScore(int finalScore)
##### Fungsi:
SetFinalScore() digunakan untuk memperbarui teks yang menampilkan skor akhir.
##### Penjelasan:
* Mengubah teks textGameOver untuk menampilkan nilai skor akhir menggunakan parameter finalScore.
* Mengatur posisi dan warna dari teks skor akhir agar sesuai dengan tampilan menu.

#### Kode
```cpp
void Engine::DinoRestartMenuScreen::SetFinalScore(int finalScore) {
    std::string finalScoreText = "Final Score: " + std::to_string(finalScore);
    //textGameOver->SetScale(3.0f)->SetText("Game Over! Final Score: " + std::to_string(score))->SetPosition(game->GetSettings()->screenWidth * 0.5f - 500, game->GetSettings()->screenHeight - 500.0f)->SetColor(0, 0, 0);

    textGameOver->SetText(finalScoreText)->SetPosition(game->GetSettings()->screenWidth * 0.5f - 130, game->GetSettings()->screenHeight - 240.0f)->SetColor(213, 168, 134);
}
```

### Kode Keseluruhan
```cpp
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
```

## 6. Dino game.cpp
Kelas ini berfungsi sebagai struktur dasar game yang memungkinkan perpindahan antara layar menu utama dan layar permainan serta pengaturan loop utama yang menyatukan pembaruan dan rendering layar. Berikut adalah penjelasan fungsi dan alur kode:
### 1. Struktur Program
#### a. Inklusi Header:
```cpp
#include "Lesson12_DinoGame.h"
#include "DinoMainMenuScreen.h"
#include "DinoGameScreen.h"
```
Tiga header ini menyertakan deklarasi yang dibutuhkan, termasuk kelas Lesson12_DinoGame, DinoMainMenuScreen, dan DinoGameScreen.
#### b. Konstruktor Lesson12_DinoGame:
```cpp
Engine::Lesson12_DinoGame::Lesson12_DinoGame(Setting* setting) :Engine::Game(setting)
{
    setting->windowTitle = "Dino Game";
}
```
Konstruktor ini memanggil konstruktor dari kelas dasar Game dengan objek setting dan mengatur judul jendela game menjadi "Dino Game".
#### c. Destruktor Lesson12_DinoGame:
```cpp
Engine::Lesson12_DinoGame::~Lesson12_DinoGame()
{
}
```
Destruktor kosong, tetapi mungkin akan digunakan untuk membersihkan sumber daya ketika objek Lesson12_DinoGame dihapuskan.
### 2. Metode dalam Lesson12_DinoGame
#### a. Init():
```cpp
void Engine::Lesson12_DinoGame::Init()
{
    Engine::ScreenManager::GetInstance(this)->AddScreen("ingame", new DinoGameScreen())
        ->AddScreen("mainmenu", new DinoMainMenuScreen())->SetCurrentScreen("mainmenu");
}
```
Metode Init menginisialisasi layar dalam game menggunakan ScreenManager. ScreenManager menambahkan tiga layar:
* "ingame": merupakan instance DinoGameScreen, yang digunakan untuk menampilkan layar utama saat gameplay berlangsung.
* "mainmenu": merupakan instance DinoMainMenuScreen, yang berfungsi sebagai layar menu utama ketika game pertama kali dijalankan.
* "restartmenu": merupakan instance DinoRestartMenuScreen, yang ditampilkan ketika game over dan memungkinkan pemain untuk memulai ulang atau kembali ke menu utama.
#### b. Update():
```cpp
void Engine::Lesson12_DinoGame::Update()
{
    Engine::ScreenManager::GetInstance(this)->Update();
}
```
Update() memanggil metode Update() pada ScreenManager, yang akan menangani logika atau status game setiap frame (misalnya, untuk mendeteksi input pengguna).
#### c. Render():
```cpp
void Engine::Lesson12_DinoGame::Render()
{
    Engine::ScreenManager::GetInstance(this)->Draw();
}
```
Render() memanggil Draw() dari ScreenManager, yang menampilkan tampilan grafis pada layar sesuai dengan tampilan atau elemen game yang aktif.

Text can be **bold**, _italic_, or ~~strikethrough~~.

[Link to another page](./another-page.html).

There should be whitespace between paragraphs.

There should be whitespace between paragraphs. We recommend including a README, or a file with information about your project.




### Dino Game

> This is a blockquote following a header.
>
> When something is important enough, you do it even if the odds are not in your favor.

### Header 3

```js
// Javascript code with syntax highlighting.
var fun = function lang(l) {
  dateformat.i18n = require('./lang/' + l)
  return true;
}
```

```ruby
# Ruby code with syntax highlighting
GitHubPages::Dependencies.gems.each do |gem, version|
  s.add_dependency(gem, "= #{version}")
end
```

### Header 4

*   This is an unordered list following a header.
*   This is an unordered list following a header.
*   This is an unordered list following a header.

### Header 5

1.  This is an ordered list following a header.
2.  This is an ordered list following a header.
3.  This is an ordered list following a header.

### Header 6

| head1        | head two          | three |
|:-------------|:------------------|:------|
| ok           | good swedish fish | nice  |
| out of stock | good and plenty   | nice  |
| ok           | good `oreos`      | hmm   |
| ok           | good `zoute` drop | yumm  |

### There's a horizontal rule below this.

* * *

### Here is an unordered list:

*   Item foo
*   Item bar
*   Item baz
*   Item zip

### And an ordered list:

1.  Item one
1.  Item two
1.  Item three
1.  Item four

### And a nested list:

- level 1 item
  - level 2 item
  - level 2 item
    - level 3 item
    - level 3 item
- level 1 item
  - level 2 item
  - level 2 item
  - level 2 item
- level 1 item
  - level 2 item
  - level 2 item
- level 1 item

### Small image

![Octocat](https://github.githubassets.com/images/icons/emoji/octocat.png)

### Large image

![Branching](https://guides.github.com/activities/hello-world/branching.png)


### Definition lists can be used with HTML syntax.

<dl>
<dt>Name</dt>
<dd>Godzilla</dd>
<dt>Born</dt>
<dd>1952</dd>
<dt>Birthplace</dt>
<dd>Japan</dd>
<dt>Color</dt>
<dd>Green</dd>
</dl>

```
Long, single-line code blocks should not wrap. They should horizontally scroll if they are too long. This line should be long enough to demonstrate this.
```


```
The final element.
```
