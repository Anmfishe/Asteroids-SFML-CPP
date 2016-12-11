#include "Asteroid.h"



void Asteroid::update(float dt, RenderWindow & w)
{
	Vector2f pos = circle.getPosition();
	pos.x += cos(angle)*speed*dt;
	pos.y += sin(angle)*speed*dt;
	if (pos.x > sw)	pos.x = 0;
	if (pos.x < 0) pos.x = sw;
	if (pos.y > sh)	pos.y = 0;
	if (pos.y < 0) pos.y = sh;
	circle.setPosition(pos);
}

Asteroid * Asteroid::getAsteroid(float angle)
{
	Asteroid *ast = new Asteroid(a_size - 1, circle.getPosition(), angle, sw, sh);
	return ast;
}

Asteroid::Asteroid(int size, Vector2f pos, float a, int SW, int SH)
{
	sw = SW;
	sh = SH;
	angle = a;
	if (size == 3) {
		a_size = 3;
		radius = 40;
		circle.setFillColor(Color::Blue);
		speed = 100;
	}
	else if (size == 2) {
		a_size = 2;
		radius = 20;
		circle.setFillColor(Color::Yellow);
		speed = 150;
	}
	else if (size == 1) {
		a_size = 1;
		radius = 10;
		circle.setFillColor(Color::Red);
		speed = 300;
	}
	circle.setRadius(radius);
	circle.setPosition(pos);
	circle.setOrigin(radius, radius);


}


Asteroid::~Asteroid()
{
}
