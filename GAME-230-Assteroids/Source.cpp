#include "Bullet.h"
#include "Ship.h"
#include "Asteroid.h"

using namespace sf;

void update_state(float dt);
void render_frame();
void load();
void loadAsteroids();
const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 800;
int curr_asteroids;
int num_asteroids = 2;

RenderWindow window;

vector<GameObject*> SceneGraph;

int main()
{
	window.create(VideoMode(SCREENWIDTH, SCREENHEIGHT), "Asteroids");
	Clock clock;
	load();
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float dt = clock.restart().asSeconds();

		update_state(dt);
		render_frame();
		window.display();
	}

	return 0;
}

void update_state(float dt)
{
	for (auto i = SceneGraph.begin(); i != SceneGraph.end(); ++i) {
		if ((*i)->dead()) {
			SceneGraph.erase(i);
			delete *i;
			break;
		}
		(*i)->update(dt, window);
	}
}

void render_frame()
{
	window.clear(Color(0, 0, 0));
	for (auto i = SceneGraph.begin(); i != SceneGraph.end(); ++i) {
		(*i)->render(window);
	}
}
void load() {
	Ship *ship = new Ship(SCREENWIDTH, SCREENHEIGHT);
	SceneGraph.push_back(ship);
	curr_asteroids = num_asteroids;
	loadAsteroids();
}
void loadAsteroids() {
	for (int i = 0; i < num_asteroids; i++) {
		Asteroid *ast = new Asteroid();
		SceneGraph.push_back(ast);
	}
}