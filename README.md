
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

## 2. Game Screen
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
*   Pembaruan Posisi Vertikal dan Efek Gravitasi

Ketika tombol Jump ditekan, kode dibawah mengatur pergerakan vertikal karakter dengan mengaplikasikan gravitasi ketika karakter berada di udara dan menghentikannya saat mencapai tanah.
```cpp
if (y > 0) {
    yVelocity -= gravity;
} else if (y < 0) {
    jump = false;
    yVelocity = 0;
    y = 0;
}
```
y += yVelocity * game->GetGameTime();
monsterSprite->SetPosition(x, y);


*   Deteksi Input Lompatan dan Animasi

Kode dibawah digunakan untuk memeriksa apakah ketika tombol Jump ditekan, animasi yang dijalankan adalah animasi dari sprite jump.
```cpp
if (game->GetInputManager()->IsKeyPressed("Jump")) {
    monsterSprite->PlayAnim("jump");
}
```

*   Pembaruan Animasi Karakter

Kode dibawah digunakan untuk memperbarui animasi karakter berdasarkan waktu permainannya.
```cpp
monsterSprite->Update(game->GetGameTime());
```

*   Peningkatan Kesulitan Permainan

Kode dibawah berfungsi untuk meningkat tingkat kesulitan permainan secara bertahap dengan mempercepat kemunculan dan kecepatan kaktus setiap 10 detik. Setiap kali timeElapsed mencapai atau melebihi 10.000 (misal: 10 detik), nilai spawnInterval dikurangi dan cactusSpeed ditambah, sambil memastikan nilai-nilai ini tidak melampaui batas minimum dan maksimum yang telah ditentukan. Setelah perubahan diterapkan, timeElapsed di-reset ke 0 untuk memulai penghitungan waktu berikutnya.
```cpp
timeElapsed += game->GetGameTime();
if (timeElapsed >= 10000) {
    spawnInterval -= 100;
    cactusSpeed += 0.2f;
    if (spawnInterval < minSpawnInterval) spawnInterval = minSpawnInterval;
    if (cactusSpeed > maxCactusSpeed) cactusSpeed = maxCactusSpeed;
    timeElapsed = 0;
}
```

*   Pembuatan Rintangan Berdasarkan Timer

Pada kode dibawah, variabel obstacleSpawnTimer akan bertambah sesuai dengan waktu permainan, dan randomSpawnTime menentukan waktu acak untuk menunggu sebelum rintangan baru muncul. Jika obstacleSpawnTimer melebihi atau sama dengan randomSpawnTime, fungsi spawnObstacle() dipanggil untuk memunculkan rintangan, lalu obstacleSpawnTimer di-reset ke 0 untuk mulai menghitung ulang.
```cpp
obstacleSpawnTimer += game->GetGameTime();
int randomSpawnTime = rand() % 4000 + spawnInterval;
if (obstacleSpawnTimer >= randomSpawnTime) {
    spawnObstacle();
    obstacleSpawnTimer = 0;
}
```

*   Penghapusan Rintangan yang Keluar dari Layar dan Deteksi Tabrakan
  
Kode dibawah berfungsi untuk mengatur posisi dan status rintangan dalam permainan. Setiap rintangan dalam daftar obstacles diperbarui posisinya dengan menguranginya berdasarkan kecepatan kaktus (cactusSpeed) dan waktu permainan. Jika ada tabrakan antara karakter utama (monsterSprite) dan rintangan (obstacle), variabel gameOver akan menjadi true, layar permainan diubah ke "restartmenu", dan skor akhir ditampilkan. Rintangan yang keluar dari batas layar kiri (x < -150) akan dihapus dari memori, dan iterasi daftar obstacles berlanjut ke rintangan berikutnya.
```cpp
for (auto it = obstacles.begin(); it != obstacles.end(); ) {
    Sprite* obstacle = *it;
    obstacle->SetPosition(obstacle->GetPosition().x - cactusSpeed * game->GetGameTime(), obstacle->GetPosition().y);

    if (monsterSprite->GetBoundingBox()->CollideWith(obstacle->GetBoundingBox())) {
        gameOver = true;
        ScreenManager::GetInstance(game)->SetCurrentScreen("restartmenu");
        DinoRestartMenuScreen* restartMenu = dynamic_cast<DinoRestartMenuScreen*>(ScreenManager::GetInstance(game)->GetCurrentScreen());
        if (restartMenu) {
            restartMenu->SetFinalScore(score);
        }
        return;
    }

    if (obstacle->GetPosition().x < -150) {
        delete obstacle;
        it = obstacles.erase(it);
    } else {
        ++it;
    }
}
```

*   Mode Debug untuk Menampilkan Bounding Box

Kode dibawah akan mengaktifkan tampilan titik-titik (dots) di setiap sudut bounding box untuk monsterSprite dan rintangan-rintangan (obstacles) jika variabel debug bernilai true. Bounding box (bb) dari monsterSprite diambil, lalu setiap titik (seperti dotSprite1) diposisikan di setiap sudut bounding box dengan mempertimbangkan skala titik untuk pusat penempatan yang tepat. Hal yang sama dilakukan untuk setiap rintangan dalam daftar obstacles: bounding box (bbObstacle) dari masing-masing rintangan diambil, dan titik-titik (seperti dotSprite11) diposisikan di sudut-sudutnya. Kode ini digunakan untuk mempermudah melihat area interaksi bounding box dan posisi presisi mereka dalam mode debug.
```cpp
if (debug) {
    BoundingBox* bb = monsterSprite->GetBoundingBox();
    dotSprite1->SetPosition(bb->GetVertices()[0].x - (0.5f * dotSprite1->GetScaleWidth()), bb->GetVertices()[0].y - (0.5f * dotSprite1->GetScaleHeight()));
    // Pengaturan serupa untuk dotSprite2, dotSprite3, dotSprite4

    for (Sprite* obstacle : obstacles) {
        BoundingBox* bbObstacle = obstacle->GetBoundingBox();
        dotSprite11->SetPosition(bbObstacle->GetVertices()[0].x - (0.5f * dotSprite11->GetScaleWidth()), bbObstacle->GetVertices()[0].y - (0.5f * dotSprite11->GetScaleHeight()));
        // Pengaturan serupa untuk dotSprite22, dotSprite33, dotSprite44
    }
}
```

### e. Fungsi Draw
Fungsi Draw() dalam kelas DinoGameScreen digunakan untuk menggambarkan elemen-elemen visual dalam permainan, dimulai dengan backgrounds, kemudian middlegrounds, lalu foregrounds. Selanjutnya, fungsi ini akan menampilkan teks untuk skor, game over, dan restart. Setelah itu, isi dari daftar obstacles dan karakter utama (monsterSprite) digambar. Jika mode debug aktif, titik-titik debug juga akan ditampilkan untuk mempermudah melihat penempatan dari posisi bounding box.

```cpp
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
```

### f. Fungsi MoveLayer
Fungsi MoveLayer() dalam kelas DinoGameScreen digunakan untuk menggerakkan lapisan latar belakang (bg) berdasarkan kecepatan yang ditentukan. Dalam setiap iterasi, fungsi memeriksa apakah posisi horizontal (x) dari setiap sprite dalam latar belakang telah keluar dari layar kiri (kurang dari -screenWidth ditambah offset). Jika ya, posisi sprite diatur kembali ke sisi kanan layar untuk menciptakan efek loop. Kemudian, posisi sprite diperbarui dengan mengurangi kecepatan (speed) yang dikalikan dengan waktu permainan (game->GetGameTime()), dan setiap sprite juga diperbarui secara umum.

```cpp
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
```

### g. Fungsi DrawLayer
Fungsi ini berfungsi untuk menggambar semua sprite dalam lapisan latar belakang (bg). Dengan menggunakan loop, fungsi ini memanggil metode Draw() pada setiap sprite (s) dalam vektor bg, sehingga setiap elemen visual di lapisan tersebut ditampilkan di layar. Fungsi ini sederhana namun efektif untuk memastikan semua elemen dalam lapisan tersebut digambar.
```cpp
void Engine::DinoGameScreen::DrawLayer(vector<Sprite*>& bg)
{
   for (Sprite* s : bg) {
       s->Draw();
   }
}
```

### h. Fungsi AddToLayer
Fungsi AddToLayer() menambahkan dua sprite ke lapisan latar belakang (bg) menggunakan tekstur yang diberikan berdasarkan nama. Pertama, sebuah objek Texture dibuat dari nama yang diterima, dan sprite pertama (s) dibuat dengan tekstur ini, menggunakan shader dan quad default, serta diatur ukurannya sesuai dengan lebar dan tinggi layar. Sprite kedua (s2) juga dibuat dengan tekstur yang sama, diatur dengan ukuran yang sama, dan posisinya ditetapkan di luar layar kanan untuk menciptakan efek tampilan yang berulang. Kedua sprite kemudian ditambahkan ke vektor bg, sehingga keduanya akan digambar di lapisan latar belakang. 
```cpp
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
```

### i. Fungsi spawnObstacle()
Fungsi ini digunakan untuk membuat dan menambahkan rintangan (kaktus) baru ke dalam permainan. Pertama, indeks acak dipilih dari daftar tekstur kaktus (cactusTextures), dan tekstur baru dibuat berdasarkan indeks tersebut. Kemudian, objek Sprite untuk kaktus dibuat dengan tekstur tersebut, diatur skalanya menjadi 4.0, dan posisinya ditetapkan di luar layar kanan (screenWidth). Bounding box kaktus disesuaikan ukurannya berdasarkan skala untuk memastikan kolisi yang tepat, dan akhirnya kaktus tersebut ditambahkan ke dalam vektor obstacles agar dapat ditampilkan dalam permainan.
```cpp
void Engine::DinoGameScreen::spawnObstacle()
{
   int randomIndex = rand() % cactusTextures.size();
   Texture* cactusTexture = new Texture(cactusTextures[randomIndex]);

   Sprite* cactus = new Sprite(cactusTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
   cactus->SetScale(4.0f);
   cactus->SetPosition(game->GetSettings()->screenWidth, 0);
   cactus->SetBoundingBoxSize(cactus->GetScaleWidth() - (16 * cactus->GetScale()), cactus->GetScaleHeight() - (4 * cactus->GetScale()));

   obstacles.push_back(cactus);
}
```

## 3. Restart Menu
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

## 4. Dino game
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

#### Kode Keseluruhan
```cpp
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
```

## 5. Launcher.cpp
"launcher" biasanya merujuk pada komponen atau fungsi yang digunakan untuk memulai atau menyiapkan suatu program atau aplikasi. Berikut adalah penjelasan bagian-bagian dari kode ini:

### 1. Inklusi Header
```cpp
#include "Setting.h"
#include "Game.h"
#include "Lesson01_BgColor.h"
#include "Lesson02_Lerp.h"
#include "Lesson03_Animation.h"
#include "Lesson04_Movement.h"
#include "Lesson05_Input.h"
#include "Lesson06_ObjectPool.h"
#include "Lesson07_GUI.h"
#include "Lesson08_Rotation.h"
#include "Lesson09_Bullet.h"
#include "Lesson10_Platform.h"
#include "Lesson11_ParallaxScrolling.h"
#include "Lesson12_DinoGame.h"
#include "Lesson12_Dino.h"
#include "DinoGameDum.h"
#include "DinoMainMenuScreen.h"
#include "LessonAI01_SteeringBehaviors.h"
#include "LessonAI02_NPCWave.h"
```

Ini memuat semua header yang diperlukan untuk menjalankan game yang mungkin mencakup fitur dan mekanik game tertentu.

### 2. Fungsi main
```cpp
int main(int argc, char** argv) { 
	Engine::Setting* setting = new Engine::Setting();
	setting->screenWidth = 1600;
	setting->screenHeight = 900;
	setting->windowFlag = Engine::WindowFlag::FULLSCREEN;
	setting->vsync = false;
	setting->targetFrameRate = 75;
	Engine::Game* game = new Engine::Lesson12_DinoGame(setting);
	//Engine::Game* game = new Engine::Lesson07_GUI(setting);

	game->Run();
	delete setting;
	delete game;

	return 0;
}
```
Pada fungsi main, dilakukan beberapa langkah berikut:
#### a. Inisialisasi Setting:
```cpp
Engine::Setting* setting = new Engine::Setting();
```
Ini membuat objek Setting untuk konfigurasi awal permainan, seperti ukuran layar, mode layar penuh, sinkronisasi vertikal, dan kecepatan frame target.

#### b. Pengaturan Resolusi, Layar Penuh, dan V-Sync:
```cpp
setting->screenWidth = 1600;
setting->screenHeight = 900;
setting->windowFlag = Engine::WindowFlag::FULLSCREEN;
setting->vsync = false;
setting->targetFrameRate = 75;
```
Beberapa pengaturan utama termasuk:
* Resolusi: Layar diatur ke resolusi 1600x900.
* Layar Penuh: Pengaturan windowFlag dengan Engine::WindowFlag::FULLSCREEN menunjukkan mode layar penuh.
* VSync: vsync = false berarti sinkronisasi vertikal dinonaktifkan.
* Target Frame Rate: Ditentukan ke 75 frame per detik.
  
#### c. Memilih Game yang Akan Dijalankan:
```cpp
Engine::Game* game = new Engine::Lesson12_DinoGame(setting);
```
Engine::Lesson12_DinoGame dipilih untuk dijalankan. Jika  ingin menjalankan pelajaran lain,  cukup mengganti instansiasi objek Game yang sesuai (misalnya, Lesson07_GUI atau kelas lainnya).

#### d. Menjalankan Game:
```cpp
game->Run();
```
Run() memulai loop utama game, di mana game akan terus berjalan hingga pengguna keluar atau game berakhir.

#### e. Menghapus Objek yang Dialokasikan:
```cpp
delete setting;
delete game;
```
Setelah game selesai dijalankan, objek setting dan game dihapus untuk membebaskan memori.

#### Kode Keseluruhan
```cpp
#include "Setting.h"
#include "Game.h"

#include "Lesson01_BgColor.h"
#include "Lesson02_Lerp.h"
#include "Lesson03_Animation.h"
#include "Lesson04_Movement.h"
#include "Lesson05_Input.h"
#include "Lesson06_ObjectPool.h"
#include "Lesson07_GUI.h"
#include "Lesson08_Rotation.h"
#include "Lesson09_Bullet.h"
#include "Lesson10_Platform.h"
#include "Lesson11_ParallaxScrolling.h"
#include "Lesson12_DinoGame.h"

#include "LessonAI01_SteeringBehaviors.h"
#include "LessonAI02_NPCWave.h"

int main(int argc, char** argv) { 
	Engine::Setting* setting = new Engine::Setting();
	setting->screenWidth = 1600;
	setting->screenHeight = 900;
	setting->windowFlag = Engine::WindowFlag::WINDOWED;
	setting->vsync = false;
	setting->targetFrameRate = 75;
	Engine::Game* game = new Engine::Lesson12_DinoGame(setting);
	game->Run();
	delete setting;
	delete game;

	return 0;
}
```
