
#include "pch.h"
#include "game.h"
#include "colour.h"
#include "button.h"



// game loop for main menu
void Game::mainMenu() {

	// setup logo objects
	sf::Texture logoTexture;
	sf::RectangleShape logo;
	logoTexture.loadFromFile("./assets/mainMenu/logo.psd");
	logo.setTexture(&logoTexture);
	logo.setSize({ window::width / 3.0f, window::height / 3.0f });
	logo.setPosition((window::width - logo.getSize().x) / 2.0f, 20.0f);

	// setup position and size for options
	float width = window::width / 5.0f;
	float height = window::height / 10.0f;
	float xPos = (window::width - width) / 2.0f;
	float yPos = window::height / 3.0f;

	const char* optionNames[] = { "Continue", "Select Level", "Multiplayer", "Settings" };

	// setup button objects
	std::vector<Button> buttons;
	for (int i = 0; i < 4; i++) {
		Button button;
		button.text.setString(optionNames[i]);
		button.setColour(Colour::Button::LightGreen, Colour::Button::DarkGreen);
		button.background.setSize({ width, height });
		button.text.setFont(font);
		button.setPosition(xPos, yPos + (height + 50.0f) * i);
		buttons.push_back(button);
	}
	const int buttonsSize = buttons.size();


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
		for (unsigned int i = 0; i < buttonsSize; i++) {

			if (sf::isPointWithinRect(mousePosition, buttons[i].background.getPosition(), { width, height })) {
				buttons[i].active();

				// if user has clicked perform an action
				if (mouseButtonDown) {
					mouseButtonDown = false;

					// continue button
					if (i == 0) {
						
						// find first locked map
						int latestUnlockedMap = 0;
						while (userdata.levels[latestUnlockedMap+1].isUnlocked && latestUnlockedMap < maps.size()-1)
							latestUnlockedMap++;


						// play the latest unloked map
						currentMapIndex = latestUnlockedMap;
						playGame(maps[latestUnlockedMap]);
					}

					else if (i == 1) levelSelection();  // level selection menu
					else if (i == 2) networkLoop();  // connect to other players
					else if (i == 3) settings();  // settings menu
				}
			}
			else
				buttons[i].inactive();

			buttons[i].draw(window);
		}

		// draw
		window.draw(logo);
		drawUI();

		// update display
		window.display();
		window.clear(Colour::Background::Background);
	}
}



// game loop to play a level
void Game::playGame(const std::string& map) {

	// load a map
	readMap(map);

	// set default values
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
		drawUI();
		drawPauseMenu();

		// update display
		window.display();
		window.clear(Colour::Background::Background);
	}
	mouseButtonDown = false;

	// save progress
	if (gameMode == Mode::gameWon) {

		// unlock next map
		if (userdata.levels.size() > currentMapIndex + 1)
			userdata.levels[currentMapIndex + 1].isUnlocked = true;

		// save score and time
		if (score > userdata.levels[currentMapIndex].highScore) {
			userdata.levels[currentMapIndex].highScore = score;
			userdata.levels[currentMapIndex].bestTime = totalTimer;
		}

		// write to file
		saveUserData();
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

	// sizes and positons for level buttons
	const float width = window::width / 6.0f;
	const float height = window::height / 6.0f;
	const float xPos = (window::width - width) / 4.0f;
	const float yPos = 50.0f;

	// scrolling variables
	float scroll = 0.0f;
	float overallScroll = 0.0f;
	float maxScroll = 0.0f;

	// array for button objects
	std::vector<HighScoreButton> buttons;

	// setup button objects
	int row = 0;
	int column = 1;

	const int mapSize = maps.size();
	for (int i = 0; i < mapSize; i++) {

		// new button
		HighScoreButton button;
		button.setColour(Colour::Button::LightGreen, Colour::Button::DarkGreen);
		button.background.setSize({ width, height });
		button.text.setFont(font);
		button.highScoreText.setFont(font);

		// highscore text
		button.highScoreText.setCharacterSize(15);


		// set button position and colour
		button.text.setString(maps[i].substr(0, maps[i].size() - 4));
		button.setPosition(xPos + (width + 20.0f) * column, yPos + (height + 20.0f) * row - scroll);

		// set highScore text attributes
		if (userdata.levels[i].highScore)
			button.highScoreText.setString("High Score: " + std::to_string(userdata.levels[i].highScore));
		else
			button.setColour(Colour::Button::LightRed, Colour::Button::DarkRed);

		// setup button if level is dissabled
		if (!userdata.levels[i].isUnlocked)
			button.setColour(Colour::Button::LightGrey, Colour::Button::DarkGrey);

		// update column and row values
		if (column % 3)
			column += 1;
		else {
			row += 1;
			column = 1;
		}

		buttons.push_back(button);
	}
	const int buttonsSize = buttons.size();


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

				// get scroll value
				scroll -= sfEvent.mouseWheel.delta * 10.0f;
				if (scroll + overallScroll < 0.0f) scroll = 0.0f;
				if (scroll + overallScroll > maxScroll) scroll = maxScroll - overallScroll;
				overallScroll += scroll;

				// move buttons
				for (int i = 0; i < buttons.size(); i++) {
					auto a = buttons[i].background.getPosition();
					buttons[i].setPosition(a.x, a.y - scroll);
				}
				// reset scroll
				scroll = 0.0f;
			}

			// shooting button
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;

		}

		// draw buttons
		for (int i = 0; i < buttonsSize; i++) {

			// assume the button is not hovered over
			buttons[i].inactive();

			// setup button if user is hovering with mouse
			if (userdata.levels[i].isUnlocked && sf::isPointWithinRect(mousePosition, buttons[i].background.getPosition(), buttons[i].background.getSize())) {
				buttons[i].active();

				// user clicking on an option
				if (mouseButtonDown) {
					mouseButtonDown = false;
					currentMapIndex = i;
					playGame(maps[i]);
					return;
				}
			}

			// draw
			buttons[i].draw(window);
		}

		// draw title name and UI
		window.draw(logo);
		drawUI();

		// update window
		window.display();
		window.clear(Colour::Background::Background);
	}
}



// game loop to modify game settings
void Game::settings() {

	// load logo texture
	sf::Texture logoTexture;
	sf::RectangleShape logo;
	logoTexture.loadFromFile("./assets/mainMenu/logo.psd");
	logo.setTexture(&logoTexture);
	logo.setSize({ window::width / 4.0f, window::height / 4.0f });
	logo.setPosition(50.0f, 50.0f);

	// size and position for buttons
	const float width = window::width / 5.0f;
	const float height = window::height / 10.0f;
	const float xPos = (window::width - width) / 2.0f;
	const float yPos = window::height / 5.0f;

	// setup button objects
	std::vector<Button> buttons;
	for (int i = 0; i < 3; i++) {
		Button button;
		button.text.setFont(font);
		button.background.setSize({ width, height });
		button.text.setString(settings::optionNames[i]);
		button.setPosition(xPos, yPos + (height + 50.0f) * i);

		// set colour
		*settings::options[i]
			? button.setColour(Colour::Button::LightGreen, Colour::Button::DarkGreen)  // green, on
			: button.setColour(Colour::Button::LightRed, Colour::Button::DarkRed);  // red, off

		buttons.push_back(button);
	}
	const int buttonsSize = buttons.size();

	float scroll = 0.0f;
	float overallScroll = 0.0f;
	const float maxScroll = 0.0f;

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

				// get scroll value
				scroll -= sfEvent.mouseWheel.delta * 10.0f;
				if (scroll + overallScroll < 0.0f) scroll = 0.0f;
				if (scroll + overallScroll > maxScroll) scroll = maxScroll - overallScroll;
				overallScroll += scroll;

				// move buttons
				for (int i = 0; i < buttons.size(); i++) {
					auto a = buttons[i].background.getPosition();
					buttons[i].setPosition(a.x, a.y - scroll);
				}
				// reset scroll
				scroll = 0.0f;
			}

			// shooting button
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;

		}

		// draw options
		for (int i = 0; i < buttonsSize; i++) {

			// check if mouse is over an option and if the user can clicked mouse
			buttons[i].inactive();
			if (sf::isPointWithinRect(mousePosition, buttons[i].background.getPosition(), { width, height })) {
				buttons[i].active();
				if (mouseButtonDown) {
					mouseButtonDown = false;

					// swap setting
					*settings::options[i] = !*settings::options[i];
					*settings::options[i]
						? buttons[i].setColour(Colour::Button::LightGreen, Colour::Button::DarkGreen)  // green, on
						: buttons[i].setColour(Colour::Button::LightRed, Colour::Button::DarkRed);  // red, off

					// apply any changes
					if (i == 0) window.setVerticalSyncEnabled(settings::isVsyncEnabled);
					if (i == 2) settings::playMusic ? backgroundMusic.play() : backgroundMusic.pause();

				}

			}
			buttons[i].draw(window);
		}

		// draw logo and UI
		window.draw(logo);
		drawUI();

		// update display
		window.display();
		window.clear(Colour::Background::Background);
	}
}



// game loop for connectiong to other devices
void Game::networkLoop() {

	// threads
	threadActive = true;
	bool listeningThreadActive = true;
	std::thread listeningThread;  // listen for new connections if device is host

	// load logo texture
	sf::Texture logoTexture;
	sf::RectangleShape logo;
	logoTexture.loadFromFile("./assets/mainMenu/logo.psd");
	logo.setTexture(&logoTexture);
	logo.setSize({ window::width / 4.0f, window::height / 4.0f });
	logo.setPosition(50.0f, 50.0f);

	// size and position for buttons
	const float width = window::width / 5.0f;
	const float height = window::height / 10.0f;
	const float xPos = (window::width - width) / 2.0f;
	const float yPos = window::height / 5.0f;

	// button things
	std::string buttonText[] = { "Client", "Connect", "Ip: " };

	// setup button objects
	std::vector<Button> buttons;
	for (int i = 0; i < 3; i++) {
		Button button;
		button.text.setFont(font);
		button.background.setSize({ width, height });
		button.text.setString(buttonText[i]);
		button.setPosition(xPos, yPos + (height + 50.0f) * i);
		button.setColour(Colour::Button::LightGreen, Colour::Button::DarkGreen);
		buttons.push_back(button);
	}
	const int buttonsSize = buttons.size();

	// mainloop for level selection
	gameMode = Mode::settings;
	while (window.isOpen()) {

		updateGameAttributes();  // update timer and mouse values

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			// close window for full screens
			if (sfEvent.type == sf::Event::KeyPressed)
				if (sfEvent.key.code == sf::Keyboard::Escape) {
					listeningThreadActive = false;
					if (listeningThread.joinable()) listeningThread.join();
					return;
				}

			// shooting button
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;

			// keyboard input
			if (sfEvent.type == sf::Event::TextEntered && buttons[0].text.getString() == "Client") {
				char key = sfEvent.text.unicode;

				// num in
				if (((key >= '0' && key <= '9') || key == '.') && buttonText[2].size() < 19) {
					buttonText[2] += key;
				}

				// backspace
				else if (key == '\b' && buttonText[2].size() > 4)
					buttonText[2].erase(buttonText[2].size() - 1, 1);

				// set and realign text
				buttons[2].text.setString(buttonText[2]);
				buttons[2].setPosition(buttons[2].background.getPosition());
			}

		}

		// draw options
		for (int i = 0; i < buttonsSize; i++) {

			// check if mouse is over an option and if the user can clicked mouse
			buttons[i].inactive();
			if (sf::isPointWithinRect(mousePosition, buttons[i].background.getPosition(), { width, height })) {
				buttons[i].active();
				if (mouseButtonDown) {
					mouseButtonDown = false;

					// toggle host/client
					if (i == 0) {

						// toggle to client
						if (buttons[0].text.getString() == "Host") {
							buttons[0].text.setString("Client");

							buttons[1].text.setString("Connect");

							// reset string to target ip
							buttons[2].text.setString(buttonText[2]);
							buttons[2].setPosition(buttons[2].background.getPosition());
						}

						// toggle to host
						else {
							buttons[0].text.setString("Host");

							buttons[1].text.setString("Listen");
						
							// set string to current ip (other devices will need to connect to host)
							buttons[2].text.setString("Ip: 192.168.0.21"); // TODO: Get IP dynamically
							buttons[2].setPosition(buttons[2].background.getPosition());
						}
					}

					// connect button clicked
					if (i == 1) {
						threadActive = listeningThreadActive = false;

						if (networkThread.joinable()) networkThread.join();
						if (listeningThread.joinable()) listeningThread.join();

						threadActive = listeningThreadActive = true;

						// start server listening
						if (buttons[0].text.getString() == "Host") {
							networkThread = std::thread([&]() {server(threadActive); });
							listeningThread= std::thread([&]() {listen(listeningThreadActive); });
						}

						// try to connect to server
						else {
							networkThread = std::thread([&]() {client(threadActive); });
						}
					}

				}

			}
			buttons[i].draw(window);
		}

		// draw logo and UI
		window.draw(logo);
		drawUI();

		// update display
		window.display();
		window.clear(Colour::Background::Background);
	}
	// close connection
	listeningThreadActive = false;
	listeningThread.join();
}

