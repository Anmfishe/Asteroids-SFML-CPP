#include "Ship.h"



Ship::Ship(int sw, int sh)
{
	circle.setRadius(radius);
	circle.setOrigin(Vector2f(radius, radius));
	this->sw = sw;
	this->sh = sh;
	circle.setPosition(sw / 2, sh / 2);
	tex.loadFromFile("ship.png");
	circle.setTexture(&tex);
}
void Ship::update(float dt, RenderWindow &w) {
	Vector2f pos = circle.getPosition();
	accel = Vector2f(0, 0);

	if (Keyboard::isKeyPressed(Keyboard::W))
		accel.y += -accel_speed;
	if (Keyboard::isKeyPressed(Keyboard::A))
		accel.x += -accel_speed;
	if (Keyboard::isKeyPressed(Keyboard::S))
		accel.y += accel_speed;
	if (Keyboard::isKeyPressed(Keyboard::D))
		accel.x += accel_speed;

	if (accel.x == 0) {
		if (vel.x > 0) {
			accel.x += -accel_speed;
		}
		else if (vel.x < 0) {
			accel.x += accel_speed;
		}
	}
	if(accel.y == 0){
		if (vel.y > 0) {
			accel.y += -accel_speed;
		}
		else if (vel.y < 0) {
			accel.y += accel_speed;
		}
	}

	vel.x += accel.x * dt;
	vel.y += accel.y * dt;

	if (vel.x > speed_cap) vel.x = speed_cap;
	if (vel.y > speed_cap) vel.y = speed_cap;
	if (vel.x < -speed_cap) vel.x = -speed_cap;
	if (vel.y < -speed_cap) vel.y = -speed_cap;

	pos.x += vel.x * dt;
	pos.y += vel.y * dt;

	if (pos.y > sh) {
		pos.y = 0;
	}
	else if (pos.y < 0) {
		pos.y = sh;
	}
	if (pos.x > sw) {
		pos.x = 0;
	}
	else if (pos.x < 0) {
		pos.x = sw;
	}

	circle.setPosition(pos);
	look_at_mouse(w);
	
}

Ship::~Ship()
{
}

void Ship::look_at_mouse(RenderWindow & w)
{
	Vector2f pos = circle.getPosition();
	Vector2i mousePos = Mouse::getPosition(w);


	float dx = pos.x - mousePos.x;
	float dy = pos.y - mousePos.y;

	float rotation = (atan2(dy, dx)) * 180 / PI;
	rotation -= 90;

	circle.setRotation(rotation);
}
