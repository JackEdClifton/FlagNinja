
#include "SFML/Graphics.hpp"

namespace Textures {
	sf::Texture* PlayerDefault;
	sf::Texture* Player1;
	sf::Texture* Player2;
	sf::Texture* Platform;
	sf::Texture* Gun;
	sf::Texture* Bullet;

	void init() {
		PlayerDefault = new sf::Texture();
		Player1 = new sf::Texture();
		Player2 = new sf::Texture();
		Platform = new sf::Texture();
		Gun = new sf::Texture();
		Bullet = new sf::Texture();

		PlayerDefault->loadFromFile("./assets/player/default.psd");
		Player1->loadFromFile("./assets/player/player_1.psd");
		Player2->loadFromFile("./assets/player/player_2.psd");
		Platform->loadFromFile("./assets/world/grass_2.psd");
		Gun->loadFromFile("./assets/guns/pistol.psd");
		Bullet->loadFromFile("./assets/guns/bullet.psd");
	}
}