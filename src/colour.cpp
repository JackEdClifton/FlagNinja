
#include "pch.h"
#include "colour.h"


namespace Colour {

	namespace Background {
		extern sf::Color Background = sf::Color(30, 50, 240);
		extern sf::Color Dim = sf::Color(0, 0, 0, 100);
	}

	namespace Button {

		// green
		sf::Color LightGreen(0x22ff22ff);
		sf::Color DarkGreen(0x22dd22ff);

		// red
		sf::Color LightRed(0xff2222ff);
		sf::Color DarkRed(0xdd2222ff);

		// grey
		sf::Color LightGrey(0x666666ee);
		sf::Color DarkGrey(0x444444ee);
	}

	namespace Text {
		sf::Color Default = sf::Color::Black;
		sf::Color UI = sf::Color::White;
	}

	namespace HealthBar {
		sf::Color Normal = sf::Color::Green;
		sf::Color Damaged = sf::Color::Red;
	}

}



/* pretend this doesnt exist
// green
sf::Color LightGreen(0x397b44ff);
sf::Color DarkGreen(0xb6d53cff);

// red
sf::Color LightRed(0x7b3944ff);
sf::Color DarkRed(0xd5b644ff);

// grey
sf::Color LightGrey(0x666666ee);
sf::Color DarkGrey(0x444444ee);
*/