#include "MainMenu.h"

MainMenu::MainMenu(float width, float height) {

	FontMainMenu.loadFromFile("Fonts/Premier2019-rPv9.ttf");

	//PLAY
	MainMenuText.setFont(FontMainMenu);
	MainMenuText.setFillColor(Color::White);
	MainMenuText.setString("PRESS SPACE TO PLAY");
	MainMenuText.setCharacterSize(30);
	MainMenuText.setPosition(200, 200);
}

MainMenu::~MainMenu() {

}

void MainMenu::draw(RenderWindow& main) {
	main.draw(MainMenuText);
}