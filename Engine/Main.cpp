#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>
#include <Settings.h>
#include <Map.h>
#include <Player.h>
#include <Camera.h>
#include <ContactListener.h>
#include <Background.h>
#include <SFMLDebugDraw.h>
#include <XMLUtils.h>
#include <Inventory.h>


sf::RenderWindow* sfWindow;
b2World* world;
ContactListener* contactListener;
Camera* camera;
Map* testMap;
Player* player;
Background* background;
SFMLDebugDraw* debugDraw;
Inventory* inventory;

void init() {
	// Load Assets
	Player::LoadAssets();
	XMLUtils::LoadItemAssets();

	// Create Contact Listener
	contactListener = new ContactListener();
	world->SetContactListener(contactListener);

	// Create Camera
	camera = new Camera();

	// Create Map
	testMap = new Map(world, sfWindow, "../../Resources/Maps/TestMap.xml");
	camera->SetMap(testMap);

	// Create Player
	player = new Player(world, testMap, sf::Vector2f(1400, 500), MALE_SEX);
	camera->SetTarget(player);

	// Create Background
	background = new Background();

	// Create Box2D Debug Draw
	debugDraw = new SFMLDebugDraw(sfWindow, world, camera);	

	inventory = new Inventory(sfWindow, player);
}

void update(int updateElapsed) {
	background->Update(updateElapsed, camera);

	testMap->Update(updateElapsed, camera);

	if(!camera->isCameraFreeRoam())
		player->HandleInputs(updateElapsed);

	player->Update(updateElapsed);

	camera->Update(updateElapsed);

	inventory->Update();
}

void render(int renderElapsed) {
	background->Render(sfWindow, camera);

	testMap->Render(sfWindow, camera);

	player->Render(sfWindow, *camera);

	inventory->Render();
}

void cleanUp() {
	delete contactListener;
	delete camera;
	delete testMap;
	delete player;
	delete background;
	delete inventory;

	AssetStore::CleanUp();
}

int main(void)
{
	// Create Music
	sf::Music music;
	music.openFromFile("../../Resources/Musics/Main.ogg");
	music.setVolume(25);
	//music.play();

	// Create SFML window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), SCREEN_TITLE);
	window.setVerticalSyncEnabled(V_SYNC);
	sfWindow = &window;
	
	// Create Box2D world
	b2World* m_world = new b2World(b2Vec2(0, WORLD_GRAVITY));
	world = m_world;
	
	// Initialize Scene
	init();
	
	// Create Counters and Clocks
	sf::Clock mainClock;
	sf::Clock generalClock;
	sf::Time renderElapsed = sf::Time::Zero;
	sf::Time renderCounter = sf::Time::Zero;
	sf::Time updateElapsed = sf::Time::Zero;
	sf::Time updateCounter = sf::Time::Zero;
	sf::Time box2DUpdateTime = sf::Time::Zero;
	sf::Time sceneUpdateTime = sf::Time::Zero;
	sf::Time sceneRenderTime = sf::Time::Zero;
	int totalRenders = 0;
	int totalUpdates = 0;

	// Main Game Loop
	while (window.isOpen())
	{
		sf::Time deltaTime = mainClock.restart();
		renderElapsed += deltaTime;
		updateElapsed += deltaTime;

		// Update
		if (updateElapsed.asSeconds() >= TARGET_UPS_TIME)
		{
			generalClock.restart();
			update(updateElapsed.asMilliseconds()); // Update Scene
			sceneUpdateTime = generalClock.restart();

			generalClock.restart();
			world->Step(TARGET_UPS_TIME, BOX2D_VELOCITY_ITERATIONS, BOX2D_POSITION_ITERATIONS); // Update Box2D
			box2DUpdateTime = generalClock.restart();

			updateCounter += updateElapsed;
			updateElapsed = sf::Time::Zero;
			totalUpdates++;
		}

		// Render
		if (renderElapsed.asSeconds() >= TARGET_FPS_TIME)
		{
			generalClock.restart();
			window.clear(sf::Color(0,0,0,255));
			render(renderElapsed.asMilliseconds()); // Render Scene
			debugDraw->Render(); // Render Box2D Debug Drawer
			window.display();
			sceneRenderTime = generalClock.restart();

			renderCounter += renderElapsed;
			renderElapsed = sf::Time::Zero;
			totalRenders++;
		}
		
		// Set Debug Title
		if(updateCounter.asSeconds() != 0 && renderCounter.asSeconds() != 0)
			window.setTitle(
				std::string(SCREEN_TITLE) +
				std::string(" Updates: ") + std::to_string((int)(totalUpdates / updateCounter.asSeconds())) +
				std::string(" / Renders: ") + std::to_string((int)(totalRenders / renderCounter.asSeconds())) +
				std::string(" / Per Physics(ms): ") + std::to_string(box2DUpdateTime.asMilliseconds()) + 
				std::string(" / Per Render(ms): ") + std::to_string(sceneRenderTime.asMilliseconds()) +
				std::string(" / Per Update(ms): ") + std::to_string(sceneUpdateTime.asMilliseconds())
			);

		// Reset Render and Update Counters
		if (updateCounter.asSeconds() >= 1) {
			totalUpdates = 0;
			updateCounter = sf::Time::Zero;
		}
		if (renderCounter.asSeconds() >= 1) {
			totalRenders = 0;
			renderCounter = sf::Time::Zero;
		}

		// Handle Inputs
		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {
			if (sfEvent.type == sfEvent.Closed)
				window.close();
			testMap->HandleWindowEvent(sfEvent, camera);
			inventory->HandleWindowEvent(sfEvent);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			window.close();
	}

	// Clean Everything
	cleanUp();

	return 0;
}

