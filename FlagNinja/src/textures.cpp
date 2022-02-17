
#include "pch.h"

namespace Textures {
	sf::Texture* Background;

	sf::Texture* Player_0;
	sf::Texture* Player_1;
	sf::Texture* Player_2;
	sf::Texture* Player_3;
	sf::Texture* Player_4;
	
	sf::Texture* Enemy_1;
	
	sf::Texture* Coin_0;
	sf::Texture* Coin_1;
	sf::Texture* Coin_2;
	
	sf::Texture* Flag_0;
	sf::Texture* Flag_1;
	
	sf::Texture* Grass;
	sf::Texture* Dirt;
	sf::Texture* Gun;
	sf::Texture* Bullet;

	bool init() {
		Background = new sf::Texture();

		Player_0 = new sf::Texture();
		Player_1 = new sf::Texture();
		Player_2 = new sf::Texture();
		Player_3 = new sf::Texture();
		Player_4 = new sf::Texture();
		
		Enemy_1 = new sf::Texture();
		
		Coin_0 = new sf::Texture();
		Coin_1 = new sf::Texture();
		Coin_2 = new sf::Texture();
		
		Flag_0 = new sf::Texture();
		Flag_1 = new sf::Texture();
		
		Grass = new sf::Texture();
		Dirt = new sf::Texture();
		Gun = new sf::Texture();
		Bullet = new sf::Texture();


		bool allLoaded = true;
		allLoaded &= Background->loadFromFile("./assets/world/background.psd");

		allLoaded &= Player_0->loadFromFile("./assets/player/player_0.psd");
		allLoaded &= Player_1->loadFromFile("./assets/player/player_1.psd");
		allLoaded &= Player_2->loadFromFile("./assets/player/player_2.psd");
		allLoaded &= Player_3->loadFromFile("./assets/player/player_3.psd");
		allLoaded &= Player_4->loadFromFile("./assets/player/player_4.psd");
		
		allLoaded &= Enemy_1->loadFromFile("./assets/enemy/enemy_1.psd");
		
		allLoaded &= Coin_0->loadFromFile("./assets/world/coin_0.psd");
		allLoaded &= Coin_1->loadFromFile("./assets/world/coin_1.psd");
		allLoaded &= Coin_2->loadFromFile("./assets/world/coin_2.psd");
		
		allLoaded &= Flag_0->loadFromFile("./assets/world/flag_0.psd");
		allLoaded &= Flag_1->loadFromFile("./assets/world/flag_1.psd");
		
		allLoaded &= Grass->loadFromFile("./assets/world/grass.psd");
		allLoaded &= Dirt->loadFromFile("./assets/world/dirt.psd");
		allLoaded &= Gun->loadFromFile("./assets/guns/pistol.psd");
		allLoaded &= Bullet->loadFromFile("./assets/guns/bullet.psd");

		if (!allLoaded)
			std::cerr << "WARNING! - Not all textures could be loaded\n";

		return allLoaded;
	}

	void destroy() {
		delete
			Player_0, Player_1, Player_2, Player_3, Player_4,
			Enemy_1,
			Coin_0, Coin_1, Coin_2,
			Flag_0, Flag_1,
			Grass, Dirt, Gun, Bullet;
	}
}
