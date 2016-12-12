#pragma once
#include "GameObject.h"
class Bullet :
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
	float getRadius() {
		return radius;
	}
	Vector2f getPos() {
		return circle.getPosition();
	}
	int getType() {
		return 0;
	}
	Bullet(float angle, Vector2f pos, int sw, int sh);
	~Bullet();
private:
	CircleShape circle;
	Vector2f vel;
	int sw, sh;
	float angle;
	float speed = 750;
	float curr_lifetime = 0;
	float lifetime = 1.5;
	float radius = 5;
	bool is_dead = false;
};

