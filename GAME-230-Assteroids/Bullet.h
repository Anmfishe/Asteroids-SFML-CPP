#pragma once
#include "GameObject.h"
class Bullet :
	public GameObject
{
public:
	void update(float dt, RenderWindow &w);
	void render(RenderWindow &w) {
		w.draw(circle);
	};
	bool dead() {
		return is_dead;
	};
	Bullet(float angle, Vector2f pos, int sw, int sh);
	~Bullet();
private:
	CircleShape circle;
	Vector2f vel;
	int sw, sh;
	float angle;
	float speed = 500;
	float curr_lifetime = 0;
	float lifetime = 2;
	bool is_dead = false;
};

