#pragma once


namespace Textures {

	extern sf::Texture* Background;

	extern sf::Texture* Player_0;
	extern sf::Texture* Player_1;
	extern sf::Texture* Player_2;
	extern sf::Texture* Player_3;
	extern sf::Texture* Player_4;
	
	extern sf::Texture* Enemy_1;
	
	extern sf::Texture* Coin_0;
	extern sf::Texture* Coin_1;
	extern sf::Texture* Coin_2;
	
	extern sf::Texture* Flag_0;
	extern sf::Texture* Flag_1;
	
	extern sf::Texture* Grass;
	extern sf::Texture* Dirt;
	extern sf::Texture* Gun;
	extern sf::Texture* Bullet;
	
	bool init();
	void destroy();
}