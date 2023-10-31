#include "World01.h"
#include "World02.h"
#include "World03.h"
#include "World04.h"
#include "World05.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

using namespace std;

int main(int argc, char* argv[])
{


	//testing the strings
	std::string testString = "Hello World!";
	std::cout << "Original string: " << testString << std::endl;

	// to uppercase
	std::string upperString = nc::StringUtils::ToUpper(testString);
	std::cout << "Uppercase string: " << upperString << std::endl;

	// to lowercase
	std::string lowerString = nc::StringUtils::ToLower(testString);
	std::cout << "Lowercase string: " << lowerString << std::endl;

	// equal strings?
	std::string testString1 = "HeLLo WoRLd!";
	bool isEqual = nc::StringUtils::IsEqual(testString, testString1);
	std::cout << "Are they equal? " << (isEqual ? "true" : "false") << std::endl;
	
	// unique
	std::string uniqueString = nc::StringUtils::CreateUnique(testString);
	std::cout << "Unique string: " << uniqueString << std::endl;


	INFO_LOG("Initialize Engine...")

	//nc::MemoryTracker::Initialize();
	nc::seedRandom((unsigned int)time(nullptr));
	nc::setFilePath("assets");

	ENGINE.Initialize();

	auto world = make_unique<nc::World05>();
	world->Initialize();

	// main loop
	bool quit = false;
	while (!quit)
	{
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<nc::Renderer>());
	}


	world->Shutdown();
	ENGINE.Shutdown();

	return 0;
}
