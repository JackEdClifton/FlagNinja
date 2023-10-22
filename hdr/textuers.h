#pragma once


namespace Textures {

	// player skins
	extern sf::Texture* Player_0;
	extern sf::Texture* Player_1;
	extern sf::Texture* Player_2;
	extern sf::Texture* Player_3;
	extern sf::Texture* Player_4;

	// enemy skins
	extern sf::Texture* GoodPeng;
	extern sf::Texture* BadPeng;
	extern sf::Texture* Enemy_1;
	
	// collectable skins
	extern sf::Texture* Coin_0;
	extern sf::Texture* Coin_1;
	extern sf::Texture* Coin_2;
	extern sf::Texture* Health_0;
	extern sf::Texture* Health_1;
	
	// flag skins
	extern sf::Texture* Flag_0;
	extern sf::Texture* Flag_1;
	
	// floor skins
	extern sf::Texture* Grass;
	extern sf::Texture* Dirt;
	extern sf::Texture* Snow;
	extern sf::Texture* SnowDirt;

	// weapon skins
	extern sf::Texture* Gun;
	extern sf::Texture* Bullet;
	
	bool init();
	void destroy();
}