#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Settings.h>
#include <Map.h>
#include <Player.h>
#include <Camera.h>
#include <ContactListener.h>
#include <Background.h>
#include <tinyxml2.h>
#include <TGUI/TGUI.hpp>

using namespace tinyxml2;

sf::RenderWindow* sfWindow;
b2World* world;
Map* testMap;
Player* player;
Camera* camera;
ContactListener* contactListener;
Background* background;
tgui::Gui* gui;

void init() {
	gui = new tgui::Gui(*sfWindow);
	tgui::Button::Ptr button = tgui::Button::create("asd");

	gui->add(button);

	contactListener = new ContactListener();
	world->SetContactListener(contactListener);

	camera = new Camera();

	testMap = new Map(world, "../../Resources/Maps/TestMap.xml");

	player = new Player(world, testMap, sf::Vector2f(1400, 500));
	camera = new Camera(testMap, player);

	background = new Background();
}

void update(int updateElapsed) {
	background->Update(updateElapsed, camera);
	testMap->Update(updateElapsed);
	player->HandleInputs(updateElapsed);
	player->Update(updateElapsed);
	camera->Update(updateElapsed);
}

void render(int renderElapsed) {
	background->Render(sfWindow, camera);
	testMap->Render(sfWindow, *camera);
	player->Render(sfWindow, *camera);

	gui->draw();
}

void cleanUp() {
	delete gui;
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
	window.setVerticalSyncEnabled(V_SYNC);
	sfWindow = &window;
	
	b2World* m_world = new b2World(b2Vec2(0, 10));
	world = m_world;

	init();
	
	sf::Clock clock;
	sf::Time renderElapsed = sf::Time::Zero;
	sf::Time renderCounter = sf::Time::Zero;
	int renders = 0;

	sf::Time updateElapsed = sf::Time::Zero;
	sf::Time updateCounter = sf::Time::Zero;
	int updates = 0;

	sf::Clock physicsClock;
	sf::Time physicsCalcTook = sf::Time::Zero;

	sf::Clock rendersClock;
	sf::Time renderCalcTook = sf::Time::Zero;

	while (window.isOpen())
	{
		sf::Time deltaTime = clock.restart();
		renderElapsed += deltaTime;
		updateElapsed += deltaTime;

		if (updateElapsed.asSeconds() >= TARGET_UPS_TIME)
		{
			update(updateElapsed.asMilliseconds());

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
			window.display();
			renderCalcTook = rendersClock.restart();
			renderCounter += renderElapsed;
			renderElapsed = sf::Time::Zero;
			renders++;
		}

		if (updateCounter.asSeconds() > 1) {
			updates = 0;
			updateCounter = sf::Time::Zero;
		}
		if (renderCounter.asSeconds() > 1) {
			renders = 0;
			renderCounter = sf::Time::Zero;
		}
		window.setTitle(std::string(SCREEN_TITLE) + std::string(" Updates: ") + std::to_string(updates / updateCounter.asSeconds()) + std::string(" / Renders: ") + std::to_string(renders / renderCounter.asSeconds()) + std::string(" / Per Physics(ms): ") + std::to_string(physicsCalcTook.asMilliseconds()) + std::string(" / Per Render(ms): ") + std::to_string(renderCalcTook.asMilliseconds()));

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {
			if (sfEvent.type == sfEvent.Closed)
				window.close();
			gui->handleEvent(sfEvent);
		}
			
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			window.close();
	}

	cleanUp();

	return 0;
}

