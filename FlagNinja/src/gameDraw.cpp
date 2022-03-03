
#include "pch.h"
#include "game.h"

#include "colour.h"
#include "button.h"



// draw options for in-game pause menu
void Game::drawPauseMenu() {
	if (!players[0].getPaused()) return;

	// dim screen
	sf::RectangleShape dimRect(sf::Vector2f((float)window::width, (float)window::height));
	dimRect.setFillColor(Colour::Background::Dim);
	window.draw(dimRect);


	// setup position and size for options
	const float width = window::width / 5.0f;
	const float height = window::height / 10.0f;
	const float xPos = (window::width - width) / 2.0f;
	const float yPos = window::height / 4.0f;

	const char* optionNames[] = { "Continue", "Settings", "Main Menu" };


	// setup button objects
	std::vector<Button> buttons;
	for (int i = 0; i < 3; i++) {
		Button button;
		button.setColour(Colour::Button::LightGreen, Colour::Button::DarkGreen);  // TODO: fix pause menu text colour
		button.text.setString(optionNames[i]);
		button.text.setFont(font);
		button.background.setSize({ width, height });
		button.setPosition(xPos, yPos + (height + 50.0f) * i);
		buttons.push_back(button);
	}
	const int buttonsSize = buttons.size();

	// draw button objects
	for (int i = 0; i < buttonsSize; i++) {

		// check if mouse is inside a button
		if (sf::isPointWithinRect(mousePosition, buttons[i].background.getPosition(), { width, height })) {
			buttons[i].active();

			// if user has clicked perform an action
			if (mouseButtonDown) {

				// continue
				if (i == 0)
					players[0].setPaused(false);

				// settings
				else if (i == 1) {
					settings();
					gameMode = Mode::mainGame;
				}

				// exit to main menu
				else if (i == 2)
					gameMode = Mode::mainMenu;
			}
		}
		else
			buttons[i].inactive();

		// draw text
		buttons[i].draw(window);
	}


}

// draw objects to the screen
void Game::drawObjects() {
	adjustCamera();

	// platforms
	for (auto& platform : platforms)
		if (isSpriteInWindow(platform, window))
			window.draw(platform);

	// players
	for (auto& player : players) {
		window.draw(player);
		window.draw(player.gun);
		player.drawHealthBar(&window);
	}

	// enemies
	for (auto& enemy : enemies) {
		window.draw(*enemy);
		window.draw(enemy->gun);
		enemy->drawHealthBar(&window);
	}


	for (auto& obj : bullets) window.draw(*obj);
	for (auto& obj : coins) window.draw(obj);

	window.draw(flag);
}

// draw the UI (text)
void Game::drawUI() {

	// setup text object
	sf::Text text;
	text.setFont(font);
	text.setFillColor(Colour::Text::UI);
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(20);

	// fps counter
	text.setString("fps: " + std::to_string(int(1.0f / deltaTime)));
	text.setPosition(50.0f, 20.0f);
	window.draw(text);

	// draw options for main game only
	if (gameMode == Mode::mainGame) {

		// score counter
		text.setPosition(200.0f, 20.0f);
		text.setString("Score: " + std::to_string(score));
		window.draw(text);
		

		// coins counter
		text.move(text.getGlobalBounds().width + 50.0f, 0.0f);
		text.setString("Coins: " + std::to_string(collectedCoins) + "/" + std::to_string(totalCoins));
		window.draw(text);

		// enemy counter
		text.move(text.getGlobalBounds().width + 50.0f, 0.0f);
		text.setString("Enemies: " + std::to_string(killedEnemies) + "/" + std::to_string(totalEnemies));
		window.draw(text);

		// game timer
		text.move(text.getGlobalBounds().width + 50.0f, 0.0f);
		text.setString("Timer: " + std::to_string(int(totalTimer)) + "." + std::to_string(int(totalTimer * 10.0f) % 10));
		window.draw(text);

		// temp max bullets
		static unsigned int maxSize = 0;
		maxSize = std::max(maxSize, bullets.size());
		text.move(text.getGlobalBounds().width + 50.0f, 0.0f);
		text.setString("Max Bullets: " + std::to_string(maxSize));
		window.draw(text);

		// temp max bullets
		text.move(text.getGlobalBounds().width + 50.0f, 0.0f);
		text.setString("Bullets: " + std::to_string(bullets.size()));
		window.draw(text);
	}
}




