#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <vector>
#include <cmath>
using namespace sf;
using namespace std;
class GameObject
{
public:
	virtual void update(float dt, RenderWindow &w) = 0;
	virtual void render(RenderWindow &w) = 0;
	virtual bool dead() = 0;
	GameObject();
	~GameObject();
};

