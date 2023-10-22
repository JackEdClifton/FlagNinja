
#include "pch.h"
#include "game.h"


// read users data for each level from a file
void Game::loadUserData() {

	// open file
	std::ifstream file("./user/userdata.jcf");

	// create a user folder if it does not exist
	if (file.fail())
		std::filesystem::create_directory("./user");

	std::string data;
	std::getline(file, data);

	// validate the line is the correct size
	if ((data.size() - 8) % 12) {
		std::cerr << "File Error\n";
		return;
	}

	// clear old userdata array
	userdata.levels.clear();

	// get starting and ending pointers to data stream
	char* dataPtr = const_cast<char*>(data.c_str());
	char* endPtr = dataPtr + data.size();

	// set the users totalPoints variable
	userdata.totalPoints = *(long long*)(void*)(data.c_str());
	dataPtr += sizeof(userdata.totalPoints);
	
	// read each 'Level'
	while (dataPtr < endPtr) {

		// is map unlocked
		auto a = *(bool*)(void*)(dataPtr);
		dataPtr += sizeof(a) + 3;

		// highest score
		auto b = *(int*)(void*)(dataPtr);
		dataPtr += sizeof(b);
		
		// best time
		auto c = *(float*)(void*)(dataPtr);
		dataPtr += sizeof(c);

		// add object to userdata
		userdata.levels.emplace_back(a, b, c);
	}

	// close the file
	file.close();
}



// write users data for each level into a file
void Game::saveUserData() {

	std::string buffer;

	// save total points
	buffer.append(
		(char*)((void*)(&userdata.totalPoints)),
		sizeof(userdata.totalPoints)
	);

	// save each levels data
	for (auto& level : userdata.levels) {
		buffer.append(
			(char*)((void*)(&level)),  // data
			sizeof(bool) + sizeof(int) + sizeof(float)+3  // size
		);
	}

	// write data to a file
	std::ofstream file("./user/userdata.jcf");
	file.write(buffer.c_str(), buffer.size());
	file.close();
}
