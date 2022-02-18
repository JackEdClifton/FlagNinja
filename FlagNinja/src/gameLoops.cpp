
#include "pch.h"
#include "game.h"





// game loop for main menu
void Game::mainMenu() {

	// setup logo objects
	sf::Texture logoTexture;
	sf::RectangleShape logo;
	logoTexture.loadFromFile("./assets/mainMenu/logo.psd");
	logo.setTexture(&logoTexture);
	logo.setSize({ window::width / 2.0f, window::height / 2.0f });
	logo.setPosition((window::width - logo.getSize().x) / 2, 20.0f);

	// setup button objects
	sf::RectangleShape button;
	sf::Text buttonText;
	buttonText.setFont(font);
	buttonText.setFillColor(sf::Color::Black);

	// setup position and size for options
	float width = window::width / 5.0f;
	float height = window::height / 10.0f;
	float xPos = (window::width - width) / 2.0f;
	float yPos = window::height / 2.0f;

	const char* optionNames[] = { "Continue", "Select Level", "Settings" };

	// game loop
	while (window.isOpen()) {

		updateGameAttributes();  // update timer and mouse values

		// sfml events
		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			// force exit game
			if (sfEvent.type == sf::Event::Closed)
				window.close();

			// close window
			if (sfEvent.type == sf::Event::KeyPressed)
				if (sfEvent.key.code == sf::Keyboard::Escape)
					return;

			// check if shooting button has been clicked
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;

		}

		// draw boxes and text for options
		for (int i = 0; i < 3; i++) {

			// set default button variables
			button.setFillColor(sf::Color(0xdd2222ff));
			button.setPosition(xPos, yPos + (height + 50.0f) * i);
			button.setSize({ width, height });

			if (sf::isPointWithinRect(mousePosition, button.getPosition(), { width, height })) {

				// set mouse hovering button variables
				button.setFillColor(sf::Color(0xff2222ff));
				button.setPosition(xPos - 10.0f, yPos - 10.0f + (height + 50.0f) * i);
				button.setSize({ width + 20.0f, height + 20.0f });

				// if user has clicked perform an action
				if (mouseButtonDown) {
					mouseButtonDown = false;

					// continue
					if (i == 0) {
						// find first locked map
						int latestUnlockedMap = 0;
						while (userdata.levels[latestUnlockedMap].isUnlocked && latestUnlockedMap < maps.size())
							latestUnlockedMap++;

						// go back to last unlocked map
						latestUnlockedMap--;

						// play the latest unloked map
						currentMapIndex = latestUnlockedMap;
						playGame(maps[latestUnlockedMap]);
					}

					// level selection menu
					else if (i == 1)
						levelSelection();

					// settings menu
					else if (i == 2)
						settings();
				}
			}

			window.draw(button);

			// draw text
			buttonText.setString(optionNames[i]);
			buttonText.setPosition(
				xPos + (width - buttonText.getGlobalBounds().width) / 2.0f,
				(yPos + (height + 50.0f) * i) + (buttonText.getGlobalBounds().height) / 4.0f
			);
			window.draw(buttonText);

		}

		// draw
		window.draw(logo);
		drawUI();

		// update display
		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
}








// game loop to play a level
void Game::playGame(const std::string& map) {

	// load a map
	readMap(map);

	score = 0.0f;
	totalTimer = 0.0f;
	gameMode = Mode::mainGame;

	// game loop for main game
	while (window.isOpen() && gameMode == Mode::mainGame) {

		// game logic
		updateGameAttributes();  // update timer and mouse values
		handleInput();  // handle input from the user
		handleCollisions();  // handle collisions for all collidable objects
		updateEntitys();  // call update method for game objects

		// draw
		drawObjects();
		drawUI(false);
		drawPauseMenu();

		// update display
		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
	mouseButtonDown = false;

	// save progress
	if (gameMode == Mode::gameWon) {

		if (userdata.levels.size() > currentMapIndex + 1)
			userdata.levels[currentMapIndex + 1].isUnlocked = true;

		if (score > userdata.levels[currentMapIndex].highScore) {
			userdata.levels[currentMapIndex].highScore = score;
			userdata.levels[currentMapIndex].bestTime = totalTimer;
		}
	}
}








// game loop to select a level
void Game::levelSelection() {

	sf::Texture logoTexture;
	sf::RectangleShape logo;
	logoTexture.loadFromFile("./assets/mainMenu/logo.psd");
	logo.setTexture(&logoTexture);
	logo.setSize({ window::width / 4.0f, window::height / 4.0f });
	logo.setPosition(50.0f, 50.0f);

	// not needed yet but we dont need this inside the game loop
	// sizes and positons for level buttons
	const float width = window::width / 6.0f;
	const float height = window::height / 6.0f;
	const float xPos = (window::width - width) / 4.0f;
	const float yPos = 50.0f;
	float scroll = 0.0f;

	// mainloop for level selection
	gameMode = Mode::levelSelection;
	while (window.isOpen()) {

		updateGameAttributes();  // update timer and mouse values

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			// force exit game
			if (sfEvent.type == sf::Event::Closed)
				window.close();

			// exit menu
			if (sfEvent.type == sf::Event::KeyPressed)
				if (sfEvent.key.code == sf::Keyboard::Escape)
					return;

			// scroll through levels
			if (sfEvent.type == sf::Event::MouseWheelMoved) {
				scroll -= sfEvent.mouseWheel.delta * 10.0f;

				const float maxScroll = 0.0f;

				if (scroll < 0.0f) scroll = 0.0f;
				if (scroll > maxScroll) scroll = maxScroll;
			}

			// shooting button
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;

		}



		// create button to draw options
		sf::RectangleShape button({ width, height });
		sf::Text buttonText;
		buttonText.setFont(font);
		buttonText.setFillColor(sf::Color::Black);

		// highscore text
		sf::Text highScoreText;
		highScoreText.setFont(font);
		highScoreText.setFillColor(sf::Color::Black);
		highScoreText.setCharacterSize(15);

		int row = 0;
		int column = 1;

		for (int i = 0l; i < maps.size(); i++) {

			// set button position and colour
			button.setPosition(xPos + (width + 20.0f) * column, yPos + (height + 20.0f) * row - scroll);
			button.setFillColor(sf::Color(userdata.levels[i].highScore ? 0x22dd22ff : 0xdd2222ff));
			
			// set button text attributes
			buttonText.setString(maps[i].substr(0, maps[i].size() - 4));
			const sf::Vector2f textSize = { buttonText.getGlobalBounds().width , buttonText.getGlobalBounds().height };
			buttonText.setPosition(button.getPosition() + (button.getSize() - textSize) / 2.0f);

			// set highScore text attributes
			if (userdata.levels[i].highScore) {
				highScoreText.setPosition(xPos + (width + 20.0f) * column + 20.0f, yPos + (height + 20.0f) * row - scroll + 10.0f);
				highScoreText.setString("High Score: " + std::to_string(userdata.levels[i].highScore));
			}
			else {
				highScoreText.setString("");
			}

			// setup button if level is dissabled
			if (!userdata.levels[i].isUnlocked) {
				button.setFillColor(sf::Color(0x666666ee));
			}

			// setup button if user is hovering with mouse
			else if (sf::isPointWithinRect(mousePosition, button.getPosition(), button.getSize())) {

				// set colour
				button.setFillColor(sf::Color(userdata.levels[i].highScore ? 0x22ff22ff : 0xff2222ff));

				// user clicking on an option
				if (mouseButtonDown) {
					mouseButtonDown = false;
					currentMapIndex = i;
					playGame(maps[i]);
					return;
				}
			}

			// draw
			window.draw(button);
			window.draw(highScoreText);
			window.draw(buttonText);

			// update column and row values
			if (column % 3) {
				column += 1;
			}
			else {
				row += 1;
				column = 1;
			}
		}

		// draw title name and UI
		window.draw(logo);
		drawUI();

		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
}





// game loop to modify game settings
void Game::settings() {
	sf::Texture logoTexture;
	sf::RectangleShape logo;
	logoTexture.loadFromFile("./assets/mainMenu/logo.psd");
	logo.setTexture(&logoTexture);
	logo.setSize({ window::width / 4.0f, window::height / 4.0f });
	logo.setPosition(50.0f, 50.0f);

	sf::RectangleShape button;
	sf::Text buttonText;
	buttonText.setFont(font);
	buttonText.setFillColor(sf::Color::Black);

	const char* optionNames[] = { "V Sync", "Hard Mode", "Background Music" };

	float scroll = 0.0f;

	// mainloop for level selection
	gameMode = Mode::settings;
	while (window.isOpen()) {

		updateGameAttributes();  // update timer and mouse values

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			// close window for full screens
			if (sfEvent.type == sf::Event::KeyPressed)
				if (sfEvent.key.code == sf::Keyboard::Escape)
					return;

			// scroll through levels
			if (sfEvent.type == sf::Event::MouseWheelMoved) {
				scroll -= sfEvent.mouseWheel.delta * 10.0f;

				const float maxScroll = 0.0f;

				if (scroll < 0.0f) scroll = 0.0f;
				if (scroll > maxScroll) scroll = maxScroll;
			}

			// shooting button
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;

		}

		const float width = window::width / 5.0f;
		const float height = window::height / 10.0f;
		const float xPos = (window::width - width) / 2.0f;
		const float yPos = window::height / 5.0f;

		// draw options
		for (int i = 0; i < 3; i++) {

			// set colour
			if (*settings::options[i])
				button.setFillColor(sf::Color(0x22ff22ff));  // green, on
			else
				button.setFillColor(sf::Color(0xdd2222ff));  // red, off

			// set mouse hovering values
			button.setPosition(xPos - 10.0f, yPos - 10.0f + (height + 50.0f) * i);
			button.setSize({ width + 20.0f, height + 20.0f });

			// check if mouse is over an option and if the user can clicked mouse
			if (sf::isPointWithinRect(mousePosition, button.getPosition(), { width, height })) {
				if (mouseButtonDown) {
					mouseButtonDown = false;

					*settings::options[i] = !*settings::options[i];
					if (i == 0)
						window.setVerticalSyncEnabled(settings::isVsyncEnabled);

					if (i == 2) {
						if (settings::playMusic) backgroundMusic.play();
						else backgroundMusic.pause();
					}

				}

			}
			// set default values
			else {
				button.setPosition(xPos, yPos + (height + 50.0f) * i);
				button.setSize({ width, height });
			}
			window.draw(button);


			// draw text
			buttonText.setString(optionNames[i]);
			buttonText.setPosition(
				xPos + (width - buttonText.getGlobalBounds().width) / 2.0f,
				(yPos + (height + 50.0f) * i) + (buttonText.getGlobalBounds().height) / 4.0f
			);
			window.draw(buttonText);

		}

		// draw logo and UI
		window.draw(logo);
		drawUI();

		// update display
		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
}
