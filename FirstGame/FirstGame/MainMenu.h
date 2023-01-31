#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class MainMenu {
public:
	MainMenu(float width, float height);

	void draw(RenderWindow& main);

	~MainMenu();
private:
	Font FontMainMenu;
	Text MainMenuText;
};