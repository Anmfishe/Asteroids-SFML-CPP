#include "Bullet.h"
#include "Ship.h"
#include "Asteroid.h"
#include <algorithm>
#include <stdio.h>
#include <conio.h>


void update_state(float dt);
void render_frame();
void load();
void loadAsteroids();
void make_bullet();
Vector2i getBucket(Vector2f pos);
Vector2f getRandPosition();
void bucket_add(Vector2i newBucket, GameObject * obj);
void bucket_remove(Vector2i curBucket, GameObject * obj);
bool detect_collisions(GameObject * obj, Vector2i newBucket);
bool check_collision(GameObject * obj1, GameObject * obj2);
void make_new_asteroids(GameObject * ast);
void next_level();
void reset_game();
const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 800;
const float PI = 3.14159265;
int curr_asteroids;
int num_asteroids = 2;
int * lives = new int;
int curr_lives;
float delay = 2;
float curr_delay = 0;
float score = 0;
int target_score = 0;
bool lose_screen = false;
Text score_t;
Text lives_t;
Text lose_text;
Font font;
Ship *ship;
SoundBuffer s_buff;
SoundBuffer p_buff;
SoundBuffer w_buff;
Sound shoot;
Sound pop;
Sound win;
RenderWindow window;

vector<GameObject*> SceneGraph;

const float BUCKET_WIDTH = 100;
const float BUCKET_HEIGHT = 100;
const int COLUMNS = 8;
const int ROWS = 8;
vector<GameObject*> grid[COLUMNS][ROWS];

float random_range(float min, float max) {
	return min + (rand() % (int)(max - min + 1));
}
float dist(Vector2f x, Vector2f y) {
	float dx = y.x - x.x;
	float dy = y.y - x.y;

	return sqrt(dx*dx + dy*dy);
}

int main()
{
	*lives = 10;
	curr_lives = *lives;
	s_buff.loadFromFile("pew.wav");
	shoot.setBuffer(s_buff);
	p_buff.loadFromFile("pop.wav");
	pop.setBuffer(p_buff);
	w_buff.loadFromFile("win.wav");
	win.setBuffer(w_buff);
	font.loadFromFile("font.TTF");
	score_t.setFont(font);
	score_t.setCharacterSize(16);
	string s = "Score: " + to_string(score);
	score_t.setString(s);
	score_t.setPosition(0, 0);
	lives_t.setFont(font);
	lives_t.setCharacterSize(16);
	lives_t.setString("Lives: " + to_string(*lives));
	lives_t.setPosition(SCREENWIDTH - 80, SCREENHEIGHT - 20);
	lose_text.setFont(font);
	lose_text.setPosition(0, 0);
	lose_text.setCharacterSize(48);

	srand(time(NULL));
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
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left && !lose_screen)
				{
					shoot.play();
					make_bullet();
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == Keyboard::Return && lose_screen) {
					lose_screen = false;
					reset_game();
				}
			}
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
	if (!lose_screen) {
		if (*lives == 0) {
			lose_text.setString("Your score was: " + to_string(target_score) +
				"\nPress Enter to play again.");
			lose_screen = true;
		}
		if (curr_lives > *lives) {
			lives_t.setString("Lives: " + to_string(*lives));
			curr_lives = *lives;
		}
		if (score < target_score) {
			score += 0.01;
			score_t.setString("Score: " + to_string(score));
		}
		else if (score > target_score) {
			score = target_score;
			score_t.setString("Score: " + to_string(score));
		}
		curr_asteroids = 0;
		for (auto i = SceneGraph.begin(); i != SceneGraph.end(); ++i) {
			GameObject * obj = (*i);
			Vector2i curBucket = getBucket(obj->getPos());
			if (obj->dead()) {
				bucket_remove(curBucket, obj);
				delete (obj);
				SceneGraph.erase(i);
				curr_asteroids = -1;
				break;
			}
			if (obj->getType() == 1) curr_asteroids++;
			obj->update(dt, window);
			Vector2i newBucket = getBucket(obj->getPos());
			if (curBucket.x != newBucket.x || curBucket.y != newBucket.y) {
				bucket_remove(curBucket, obj);
				bucket_add(newBucket, obj);
			}
			if (detect_collisions(obj, newBucket)) {
				curr_asteroids = -1;
				break;
			}
		}
		if (curr_asteroids == 0) {
			if (curr_delay == 0) {
				win.play();
			}
			curr_delay += dt;
			if (curr_delay > delay) {
				next_level();
			}
		}
		Vector2i curBucket = getBucket(ship->getPos());
		ship->update(dt, window);
		Vector2i newBucket = getBucket(ship->getPos());
		if (curBucket.x != newBucket.x || curBucket.y != newBucket.y) {
			bucket_remove(curBucket, ship);
			bucket_add(newBucket, ship);
		}
		detect_collisions(ship, newBucket);
	}
}
void render_frame()
{
	
	window.clear(Color(0, 0, 0));
	if (!lose_screen) {
		for (auto i = SceneGraph.begin(); i != SceneGraph.end(); ++i) {
			(*i)->render(window);
		}
		ship->render(window);
		window.draw(score_t);
		window.draw(lives_t);
	}
	else {
		window.draw(lose_text);
	}
}
void load() {
	ship = new Ship(SCREENWIDTH, SCREENHEIGHT, lives);
	Vector2i curBucket = getBucket(ship->getPos());
	bucket_add(curBucket, ship);
	loadAsteroids();
}
void next_level() {
	curr_delay = 0;
	num_asteroids ++;
	loadAsteroids();
}
void reset_game()
{
	
	for (auto i = SceneGraph.begin(); i != SceneGraph.end(); ++i) {
		delete *i;
	}
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			auto & v = grid[i][j];
			v.clear();
		}
	}
	SceneGraph.clear();
	Vector2i curBucket = getBucket(ship->getPos());
	bucket_add(curBucket, ship);
	*lives = 10;
	curr_lives = *lives;
	score = 0;
	target_score = 0;
	num_asteroids = 2;
	score_t.setString("Score: " + to_string(score));
	lives_t.setString("Lives: " + to_string(*lives));
	loadAsteroids();
}
void loadAsteroids() {
	for (int i = 0; i < num_asteroids; i++) {
		int size = random_range(1, 3);
		Vector2f pos = getRandPosition();
		float angle = random_range(0, 2 * PI);
		Asteroid *ast = new Asteroid(size, pos, angle, SCREENWIDTH, SCREENHEIGHT);
		SceneGraph.push_back(ast);
		Vector2i curBucket = getBucket(ast->getPos());
		bucket_add(curBucket, ast);
	}
}
void make_bullet() {
	Bullet *b = ship->get_bullet();
	SceneGraph.push_back(b);
	Vector2i curBucket = getBucket(b->getPos());
	bucket_add(curBucket, b);
}
Vector2i getBucket(Vector2f pos)
{
	int col = int(pos.x / BUCKET_WIDTH);
	int row = int(pos.y / BUCKET_HEIGHT);
	if (col < 0)
		col = 0;
	else if (col >= COLUMNS)
		col = COLUMNS - 1;
	if (row < 0)
		row = 0;
	else if (row >= ROWS)
		row = ROWS - 1;
	return Vector2i(col, row);
}
Vector2f getRandPosition() {
	Vector2f res;
	res.x = random_range(0, SCREENWIDTH);
	res.y = random_range(0, SCREENHEIGHT);
	if (dist(ship->getPos(), res) < 50) {
		res = getRandPosition();
	}
	return res;
}

void bucket_add(Vector2i newBucket, GameObject * obj)
{
	vector<GameObject*> & v
		= grid[newBucket.x][newBucket.y];
	v.push_back(obj);

}

void bucket_remove(Vector2i curBucket, GameObject * obj)
{
	vector<GameObject*> & v
		= grid[curBucket.x][curBucket.y];
	v.erase(remove(v.begin(), v.end(), obj));
}

bool detect_collisions(GameObject * obj, Vector2i newBucket)
{
	int left = max(newBucket.x - 1, 0);
	int right = min(newBucket.x + 1, COLUMNS - 1);
	int top = max(newBucket.y - 1, 0);
	int bot = min(newBucket.y + 1, ROWS - 1);
	for (int bx = left; bx <= right; ++bx)
	{
		for (int by = top; by <= bot; ++by)
		{
			vector<GameObject*> & v = grid[bx][by];
			for (GameObject* o : v)
			{
				if (o != obj){
					if (check_collision(o, obj)) {
						return true;

					}
				}
			}
		}
	}
	return false;
}

bool check_collision(GameObject * obj1, GameObject * obj2)
{
	int t1 = obj1->getType();
	int t2 = obj2->getType();
	if (t1 == 1 && t2 == 1) return false;
	if (t1 == 0 && t2 == 0) return false;
	if ((t1 == 2 && t2 == 0) || (t1 == 0 && t2 == 2)) return false;
	float r1 = obj1->getRadius();
	float r2 = obj2->getRadius();
	Vector2f pos1 = obj1->getPos();
	Vector2f pos2 = obj2->getPos();
	if (dist(pos1, pos2) < (r1 + r2)) {
		obj1->hit();
		obj2->hit();
		if (t1 == 0 && t2 == 1) {
			target_score += 10;
			make_new_asteroids(obj2);
			pop.play();
		}
		else if (t1 == 1 && t2 == 0) {
			target_score += 10;
			make_new_asteroids(obj1);
			pop.play();
		}
		return true;
	}
	else {
		return false;
	}
}

void make_new_asteroids(GameObject * ast)
{
	if (auto asteroid = dynamic_cast<Asteroid*>(ast) ) {
		Vector2f pos = asteroid->getPos();
		int nSize = asteroid->getSize() - 1;
		if (nSize == 0) return;
		for (int i = 0; i < 2; i++) {
			float angle = random_range(0, 2 * PI);
			Asteroid * a = new Asteroid(nSize, pos, angle, SCREENWIDTH, SCREENHEIGHT);
			SceneGraph.push_back(a);
			Vector2i curBucket = getBucket(a->getPos());
			bucket_add(curBucket, a);
		}
	}
	
}

