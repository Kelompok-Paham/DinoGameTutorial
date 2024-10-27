
### 1. Main Menu Screen
Berikut adalah tutorial untuk membuat Main Menu Screen pada game Dino menggunakan Screen Manager. main menu screen game ini melibatkan dua kelas utama: ScreenManager untuk mengelola layar (screen) dan DinoMainMenuScreen sebagai layar utama (main menu screen) game

## Kelas ScreenManager
Kelas ScreenManager bertanggung jawab untuk mengelola tampilan layar yang aktif dalam game. Berikut fungsi yang ada:


*   Singleton Pattern: ScreenManager menggunakan singleton pattern untuk memastikan hanya ada satu instance ScreenManager di seluruh game. Ini diimplementasikan dengan fungsi GetInstance yang menerima parameter Game* engine.
*   Fungsi AddScreen: Digunakan untuk menambahkan layar baru (seperti Main Menu Screen) ke dalam ScreenManager.
*   Fungsi SetCurrentScreen: Untuk mengubah layar yang sedang aktif saat ini.
*   Fungsi Update dan Draw: Untuk memperbarui dan menggambar layar yang sedang aktif.


Pada ScreenManager.cpp, kita inisialisasi instance tunggal, dan implementasi fungsi seperti AddScreen, SetCurrentScreen, Update, dan Draw.

## Kelas DinoMainMenuScreen
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


Text can be **bold**, _italic_, or ~~strikethrough~~.

[Link to another page](./another-page.html).

There should be whitespace between paragraphs.

There should be whitespace between paragraphs. We recommend including a README, or a file with information about your project.

### Header 1

This is a normal paragraph following a header. GitHub is a code hosting platform for version control and collaboration. It lets you and others work together on projects from anywhere.

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
