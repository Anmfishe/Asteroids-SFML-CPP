#pragma once
#include "Bullet.h"
class Ship :
	public GameObject
{
public:
	void update(float dt, RenderWindow &w);
	void hit();
	void render(RenderWindow &w) {
		w.draw(circle);
	};
	bool dead() {
		return false;
	};
	float getRadius() {
		return radius;
	}
	Vector2f getPos() {
		return circle.getPosition();
	}
	int getType() {
		return 2;
	}
	Ship(int sw, int sh, int * l);
	~Ship();
	Bullet* get_bullet();
private:
	void look_at_mouse(RenderWindow &w);
	CircleShape circle;
	Vector2f vel;
	Vector2f accel;
	Texture tex, f_tex;
	float radius = 40;
	int sw, sh;
	float accel_speed = 500;
	float speed_cap = 300;
	const float PI = 3.14159265;
	float rotation;
	float invl_time = 3;
	float accuml_time = 3;
	int * lives;
	SoundBuffer buff;
	Sound hit_s;
};

