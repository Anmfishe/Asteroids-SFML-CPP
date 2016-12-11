#pragma once
#include "Bullet.h"
class Ship :
	public GameObject
{
public:
	void update(float dt, RenderWindow &w);
	void render(RenderWindow &w) {
		w.draw(circle);
	};
	bool dead() {
		return false;
	};
	Vector2f getPos() {
		return circle.getPosition();
	}
	Ship(int sw, int sh);
	~Ship();
	Bullet* get_bullet();
private:
	void look_at_mouse(RenderWindow &w);
	CircleShape circle;
	Vector2f vel;
	Vector2f accel;
	Texture tex;
	float radius = 40;
	int sw, sh;
	float accel_speed = 500;
	float speed_cap = 300;
	const float PI = 3.14159265;
	float rotation;
};

