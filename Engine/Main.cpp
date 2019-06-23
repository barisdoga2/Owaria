#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>
#include <Settings.h>
#include <Map.h>
#include <Player.h>
#include <Camera.h>
#include <ContactListener.h>
#include <Background.h>
#include <tinyxml2.h>
#include <SFMLDebugDraw.h>

using namespace tinyxml2;

sf::RenderWindow* sfWindow;
b2World* world;
Map* testMap;
Player* player;
Camera* camera;
ContactListener* contactListener;
Background* background;

void init() {
	contactListener = new ContactListener();
	world->SetContactListener(contactListener);

	camera = new Camera();

	testMap = new Map(world, sfWindow, "../../Resources/Maps/TestMap.xml");

	player = new Player(world, testMap, sf::Vector2f(1400, 500));
	camera = new Camera(testMap, player);

	background = new Background();
}

void update(int updateElapsed) {
	background->Update(updateElapsed, camera);
	testMap->Update(updateElapsed, camera);
	if(!camera->isCameraFreeRoam())
		player->HandleInputs(updateElapsed);
	player->Update(updateElapsed);
	camera->Update(updateElapsed);
}

void render(int renderElapsed) {
	background->Render(sfWindow, camera);
	testMap->Render(sfWindow, camera);
	player->Render(sfWindow, *camera);
}

void cleanUp() {
	delete contactListener;
	delete camera;
	delete testMap;
	delete player;
	delete background;

	AssetStore::CleanUp();
}

int main(void)
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), SCREEN_TITLE);

	sf::Music music;
	if (!music.openFromFile("../../Resources/music.ogg")) {
		cout << "couldnt load" << endl;
	}
	music.setLoop(true);
	music.setVolume(25);
	music.play();

	window.setVerticalSyncEnabled(V_SYNC);
	sfWindow = &window;
	
	b2World* m_world = new b2World(b2Vec2(0, WORLD_GRAVITY));
	world = m_world;
	
	init();

	bool isDebugDrawEnabled = false;
	cout << "Press F1 to enable or disable Box2D Debug Draw!" << endl;
	SFMLDebugDraw debugDraw(window, camera);
	debugDraw.SetFlags(b2Draw::e_shapeBit);
	m_world->SetDebugDraw(&debugDraw);
	
	sf::Clock clock;
	sf::Time renderElapsed = sf::Time::Zero;
	sf::Time renderCounter = sf::Time::Zero;
	int renders = 0;

	sf::Time updateElapsed = sf::Time::Zero;
	sf::Time updateCounter = sf::Time::Zero;
	int updates = 0;

	sf::Clock physicsClock;
	sf::Time physicsCalcTook = sf::Time::Zero;

	sf::Clock updateClock;
	sf::Time updateCalcTook = sf::Time::Zero;

	sf::Clock rendersClock;
	sf::Time renderCalcTook = sf::Time::Zero;

	while (window.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		renderElapsed += deltaTime;
		updateElapsed += deltaTime;

		if (updateElapsed.asSeconds() >= TARGET_UPS_TIME)
		{
			updateClock.restart();
			update(updateElapsed.asMilliseconds());
			updateCalcTook = updateClock.restart();

			physicsClock.restart();
			world->Step(TARGET_UPS_TIME, BOX2D_VELOCITY_ITERATIONS, BOX2D_POSITION_ITERATIONS);
			physicsCalcTook = physicsClock.restart();

			updateCounter += updateElapsed;
			updateElapsed = sf::Time::Zero;
			updates++;
		}

		if (renderElapsed.asSeconds() >= TARGET_FPS_TIME)
		{
			rendersClock.restart();
			window.clear(sf::Color(30,30,30,255));
			render(renderElapsed.asMilliseconds());
			if(isDebugDrawEnabled)
				world->DrawDebugData();
			window.display();
			renderCalcTook = rendersClock.restart();
			renderCounter += renderElapsed;
			renderElapsed = sf::Time::Zero;
			renders++;
		}
		
		if(updateCounter.asSeconds() != 0 && renderCounter.asSeconds() != 0)
			window.setTitle(
				std::string(SCREEN_TITLE) +
				std::string(" Updates: ") + std::to_string((int)(updates / updateCounter.asSeconds())) +
				std::string(" / Renders: ") + std::to_string((int)(renders / renderCounter.asSeconds())) +
				std::string(" / Per Physics(ms): ") + std::to_string(physicsCalcTook.asMilliseconds()) + 
				std::string(" / Per Render(ms): ") + std::to_string(renderCalcTook.asMilliseconds()) +
				std::string(" / Per Update(ms): ") + std::to_string(updateCalcTook.asMilliseconds())
			);

		if (updateCounter.asSeconds() > 1) {
			updates = 0;
			updateCounter = sf::Time::Zero;
		}
		if (renderCounter.asSeconds() > 1) {
			renders = 0;
			renderCounter = sf::Time::Zero;
		}

		// Check Debug Draw is Enable or Disable Event
		static bool releaseFlag = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
			if (releaseFlag)
				releaseFlag = false;
		}
		else {
			if (!releaseFlag) {
				isDebugDrawEnabled = !isDebugDrawEnabled;
				releaseFlag = true;
			}
		}


		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {
			if (sfEvent.type == sfEvent.Closed)
				window.close();
			testMap->HandleWindowEvent(sfEvent, camera);
		}
			
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			window.close();
	}

	cleanUp();

	return 0;
}

