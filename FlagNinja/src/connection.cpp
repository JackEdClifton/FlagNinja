
#include "pch.h"
#include "game.h"


/*
data that needs to be sent

position	: sf::Vector2f	-> 8 bytes
texture		: enum int		-> 4 bytes
angle		: float			-> 4 bytes

<<
gameState = int 4 bytes

<<>
players: max=5
position	8
texture		4
gun
	angle	4
= 80

<>
new bullets: max=64
position 8
direction 8
= 1024

<<
enemies: max=100
position	8
gun
	angle	4
= 1200

------------------

= 4096




client: hey, I exist

loop
	server: im in menu
	client: cool

server:	im in this map
		this is the player info
		this is the enemy info
		this is the new bullets

client:	this is my player info
		this is my new bullets



*/


#define BUFFER_SIZE 4096
#define PORT 53000
#define IP "192.168.0.39"

struct EntityInfo {
	float x, y;
	float gunX, gunY, angle;
	int animation;
};

struct Packet {
	unsigned int map;
	EntityInfo players[5];
	EntityInfo enemies[100];
};



// array for connections
std::vector<sf::TcpSocket*> connections;


// listen on a network port to add new connections to connections array
void Game::listen(bool& threadActive) {

	// setup objects
	sf::TcpListener listener;
	sf::TcpSocket* client = new sf::TcpSocket;
	listener.setBlocking(false);

	// listen on network port
	listener.listen(53000);
	while (threadActive) {

		// accept a new connection
		if (listener.accept(*client) == sf::Socket::Done) {
			std::cout << "Added client\n";
			connections.push_back(client);
			client = new sf::TcpSocket;
		}
		else
			std::cout << "Retrying to accept\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(450));

	}
}

// send and recieve data, server side
void Game::server(bool& threadActive) {

	// setup data buffers
	char dataOut[BUFFER_SIZE];
	char dataIn[BUFFER_SIZE];

	// zero memory
	for (int i = 0; i < BUFFER_SIZE; i++) dataOut[i] = 0;
	for (int i = 0; i < BUFFER_SIZE; i++) dataIn[i] = 0;

	// set output
	for (int i = 0; i < 12; i++)
		dataOut[i] = "Hello World"[i];

	// variables for network loop
	size_t received;
	bool deleteConnection;

	// network loop
	while (threadActive) {

		// iterate through each connection
		for (unsigned int i = 0; i < connections.size(); i++) {

			deleteConnection = false;

			deleteConnection |= sf::Socket::Status::Done != connections[i]->send(dataOut, BUFFER_SIZE);
			deleteConnection |= sf::Socket::Status::Done != connections[i]->receive(dataIn, BUFFER_SIZE, received);

			if (deleteConnection) {
				connections[i] = connections[connections.size() - 1];
				connections.pop_back();
				i -= 1;
				continue;
			}

			// process data
			std::cout << dataIn << std::endl;

			// zero memory
			for (unsigned int j = 0; j < received; j++) dataIn[j] = 0;
		}
	}
}

// send and recieve data, client side
void Game::client(bool& threadActive) {

	// debugging
	std::string exampleString = "This is a message from the client";

	// data buffers
	char dataOut[BUFFER_SIZE];
	char dataIn[BUFFER_SIZE];

	// zero memory
	for (int i = 0; i < BUFFER_SIZE; i++) dataOut[i] = 0;
	for (int i = 0; i < BUFFER_SIZE; i++) dataIn[i] = 0;

	// make a connection
	sf::TcpSocket socket;
	socket.connect(sf::IpAddress(IP), PORT);

	// network loop
	while (threadActive) {

		// load data into buffer to send
		for (int i = 0; i < exampleString.length(); i++)
			dataOut[i] = exampleString[i];


		// send data in buffer
		size_t recv;
		if (socket.send(dataOut, BUFFER_SIZE)) break;
		if (socket.receive(dataIn, sizeof(dataIn), recv)) break;

		// handle response
		std::cout << dataIn << std::endl;

		// zero memory
		for (int i = 0; i < BUFFER_SIZE; i++) dataOut[i] = 0;
		for (int i = 0; i < BUFFER_SIZE; i++) dataIn[i] = 0;
	}
	// return to single player
}









/*
Plan?

unsigned int map (null is not in game)


*/