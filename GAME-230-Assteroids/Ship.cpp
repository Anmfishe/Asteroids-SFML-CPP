#include "Ship.h"



Ship::Ship(int sw, int sh, int * l)
{
	circle.setRadius(radius);
	circle.setOrigin(Vector2f(radius, radius));
	this->sw = sw;
	this->sh = sh;
	circle.setPosition(sw / 2, sh / 2);
	tex.loadFromFile("ship.png");
	circle.setTexture(&tex);
	f_tex.loadFromFile("ship_fire.png");
	lives = l;
	buff.loadFromFile("hit.wav");
	hit_s.setBuffer(buff);
}
void Ship::update(float dt, RenderWindow &w) {
	if (accuml_time <= invl_time) {
		accuml_time += dt;
	}
	Vector2f pos = circle.getPosition();
	accel = Vector2f(0, 0);
	if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::A) ||
		Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::D))
		circle.setTexture(&f_tex);
	else
		circle.setTexture(&tex);
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

void Ship::hit()
{
	//still invl
	if (accuml_time <= invl_time) {
		return;
	}
	//ship was hit
	else {
		hit_s.play();
		*lives = *lives - 1;
		accuml_time = 0;
	}
}

Ship::~Ship()
{
}

Bullet * Ship::get_bullet()
{
	Vector2f pos = circle.getPosition();
	pos.x += cos(rotation) * radius;
	pos.y += sin(rotation) * radius;
	Bullet *b = new Bullet(rotation, pos, sw, sh);
	return b;
}

void Ship::look_at_mouse(RenderWindow & w)
{
	Vector2f pos = circle.getPosition();
	Vector2i mousePos = Mouse::getPosition(w);

	float dx = pos.x - mousePos.x;
	float dy = pos.y - mousePos.y;
	 
	float rot = (atan2(dy, dx));
	rotation = rot + PI;
	rot = rot * 180 / PI;
	circle.setRotation(rot - 90);
}
