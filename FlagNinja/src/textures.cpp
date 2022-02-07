
#include "pch.h"

namespace Textures {
	sf::Texture* PlayerDefault;
	sf::Texture* Player1;
	sf::Texture* Player2;
	sf::Texture* Grass;
	sf::Texture* Dirt;
	sf::Texture* Gun;
	sf::Texture* Bullet;

	void init() {
		PlayerDefault = new sf::Texture();
		Player1 = new sf::Texture();
		Player2 = new sf::Texture();
		Grass = new sf::Texture();
		Dirt = new sf::Texture();
		Gun = new sf::Texture();
		Bullet = new sf::Texture();

		PlayerDefault->loadFromFile("./assets/player/default.psd");
		Player1->loadFromFile("./assets/player/player_1.psd");
		Player2->loadFromFile("./assets/player/player_2.psd");
		Grass->loadFromFile("./assets/world/grass.psd");
		Dirt->loadFromFile("./assets/world/dirt.psd");
		Gun->loadFromFile("./assets/guns/pistol.psd");
		Bullet->loadFromFile("./assets/guns/bullet.psd");
	}

	void destroy() {
		delete PlayerDefault;
		delete Player1;
		delete Player2;
		delete Grass;
		delete Dirt;
		delete Gun;
		delete Bullet;
	}
}