#include <iostream>
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "SFML\System.hpp"
#include "MainMenu.h"
#include <cstdlib>
#include <vector>
#include <sstream>
#include <fstream>

using namespace sf;
using namespace std;

int main() {

	srand(time(NULL));

	sf::RenderWindow menu(VideoMode(640, 480), "Main Menu");
	MainMenu MainMenuText(menu.getSize().x, menu.getSize().y);

	//MENU LOOP
	while (menu.isOpen()) {
		Event choice;
		while (menu.pollEvent(choice)) {
			if (choice.type == Event::Closed) {
				menu.close();
			}
			if (choice.type == Event::KeyReleased) {
				if (choice.key.code == Keyboard::Space) {
					menu.close();
				}
			}
		}
		menu.clear();
		MainMenuText.draw(menu);
		menu.display();
	}

	int gameOver = 1;

	sf::RenderWindow window(VideoMode(640, 480), "Shooter");
	window.setFramerateLimit(60);

	//SCORE
	int points = 0;

	Font FontScore;
	FontScore.loadFromFile("Fonts/Premier2019-rPv9.ttf");

	std::ostringstream board;
	board << "Score: " << points;

	Text score;
	score.setCharacterSize(30);
	score.setPosition(10.f, 10.f);
	score.setFont(FontScore);
	score.setString(board.str());


	//TANK
	Texture tankT;
	Sprite tank;

	tankT.loadFromFile("Textures/tank.jpg");

	tank.setTexture(tankT);
	tank.setScale(Vector2f(0.30f, 0.30f));
	tank.setPosition(window.getSize().x / 2 - tank.getGlobalBounds().width / 2, window.getSize().y - tank.getGlobalBounds().height);

	Vector2f tankCenter;
	int shootTimer = 0;

	//ENEMY
	CircleShape enemy;
	enemy.setFillColor(Color::Red);
	enemy.setRadius(35.f);

	std::vector<CircleShape> enemies;
	enemies.push_back(CircleShape(enemy));

	int enemySpawnTimer = 0;

	//BULLETS
	RectangleShape bullet;
	bullet.setFillColor(Color::Yellow);
	bullet.setSize(Vector2f(5.f, 10.f));

	std::vector<RectangleShape> bullets;
	bullets.push_back(RectangleShape(bullet));

	//GAME LOOP
	while (window.isOpen() && gameOver > 0) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
				window.close();
			}
		}

		//UPDATE

		//TANK
		tankCenter = Vector2f(tank.getPosition().x + tank.getGlobalBounds().width / 2, tank.getPosition().y);

		tank.setPosition(Mouse::getPosition(window).x, tank.getPosition().y);

		if (tank.getPosition().x < 0) {
			tank.setPosition(0, window.getSize().y - tank.getGlobalBounds().height);
		}

		if (tank.getPosition().x > window.getSize().x - tank.getGlobalBounds().width) {
			tank.setPosition(window.getSize().x - tank.getGlobalBounds().width, window.getSize().y - tank.getGlobalBounds().height);
		}

		//BULLETS
		if (shootTimer < 15) {
			shootTimer++;
		}

		if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= 15) {
			bullet.setPosition(tankCenter);
			bullets.push_back(RectangleShape(bullet));

			shootTimer = 0;
		}

		for (int i = 0; i < bullets.size(); i++) {
			bullets[i].move(0.f, -10.f);

			if (bullets[i].getPosition().y <= 0) {
				bullets.erase(bullets.begin() + i);
			}
		}

		//ENEMIES
		if (enemySpawnTimer < 25) {
			enemySpawnTimer++;
		}

		if (enemySpawnTimer >= 25) {
			enemy.setPosition((rand() % int(window.getSize().x) - enemy.getRadius()), 0.f);
			enemies.push_back(CircleShape(enemy));

			enemySpawnTimer = 0;
		}

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i].move(0.f, 5.f);

			if (enemies[i].getPosition().y > window.getSize().y) {
				gameOver = 0;

				ofstream Highscore;
				Highscore.open("highscore.txt");
				Highscore << "Current highscore: " << points << endl;
				Highscore.close();
			}
		}

		//COLLISION
		for (int i = 0; i < bullets.size(); i++)
		{
			for (int k = 0; k < enemies.size(); k++)
			{
				if (bullets[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds())) {
					bullets.erase(bullets.begin() + i);
					enemies.erase(enemies.begin() + k);
					points++;
					board.str("");
					board << "Score: " << points;
					score.setString(board.str());
					break;
				}
			}
		}

		//DRAW
		window.clear(sf::Color::Black);

		window.draw(score);

		window.draw(tank);

		for (int i = 0; i < enemies.size(); i++) {
			window.draw(enemies[i]);
		}

		for (int i = 0; i < bullets.size(); i++) {
			window.draw(bullets[i]);
		}

		window.display();
	}

	return 0;
}