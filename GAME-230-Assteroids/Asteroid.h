#pragma once
#include "GameObject.h"
class Asteroid :
	public GameObject
{
public:
	void update(float dt, RenderWindow &w);
	void hit();
	void render(RenderWindow &w) {
		w.draw(circle);
	};
	bool dead() {
		return is_dead;
	};
	int get_size() {
		return a_size;
	}
	float getRadius() {
		return radius;
	}
	Vector2f getPos() {
		return circle.getPosition();
	}
	int getType() {
		return 1;
	}
	int getSize() {
		return a_size;
	}
	Asteroid *getAsteroid(float angle);
	Asteroid(int size, Vector2f pos, float a, int SW, int SH);
	~Asteroid();
private:
	CircleShape circle;
	Vector2f vel;
	float radius;
	float speed;
	float angle;
	int a_size;
	bool is_dead = false;
	int sw, sh;
};

