#pragma once
#include "GameObject.h"
class Bullet :
	public GameObject
{
public:
	void update(float dt, RenderWindow &w) {

	};
	void render(RenderWindow &w) {
		w.draw(circle);
	};
	bool dead() {
		return false;
	};
	Bullet();
	~Bullet();
private:
	CircleShape circle;
	Vector2f vel;
};

