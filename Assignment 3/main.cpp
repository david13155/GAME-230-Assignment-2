#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <string>
#include <iostream>
#include <fstream>

using namespace sf;
using namespace std;

//Game Loop Pattern
void initialize(RenderWindow& window);
void userInput(RenderWindow& window, Event& e);
void render(RenderWindow& window);

Vector2<int> MouseTile(RenderWindow& window);
void SaveFile();
void LoadFile();
void LoadTiles();

const string SCREEN_SHOT_NAME = "Assignment 2 Level Screenshot.png";

int tileSelected = 0;
Texture tileTexture[21];
Sprite tileSprite[21];
int level[10][10];
Sprite tileMap[10][10];

int main() {
	LoadTiles();

	RenderWindow mainWindow(VideoMode(750, 750), "Assignment 2");

	initialize(mainWindow);

	while (mainWindow.isOpen())
	{
		Event event;
		while (mainWindow.pollEvent(event))
		{
			userInput(mainWindow, event);
		}
		render(mainWindow);
	}
	return 0;
}

void initialize(RenderWindow& window) 
{

	tileSelected = 0;

	cout << "press S to Save" << endl;
	cout << "Press L to Load" << endl;
	cout << "Press Space to Sreenshot" << endl;

	for (int i = 0; i < 10; i++) 
	{
		for (int j = 0; j < 10; j++) 
		{
			level[i][j] = -1;
			tileMap[i][j].setPosition(j * 70 + (window.getSize().x % 70) / 2, i * 70 + (window.getSize().y % 70) / 2);
		}
	}
	return;

}

void userInput(RenderWindow& window, Event& e) 
{

	if (e.type == Event::KeyPressed) 
	{

		//Select
		if (e.key.code == Keyboard::Up) 
		{
			tileSelected = (tileSelected + 1) % 21;
		}
		if (e.key.code == Keyboard::Down) 
		{
			tileSelected = (tileSelected - 1 + 21) % 21;
		}

		//Save
		if (e.key.code == Keyboard::S) 
		{
			SaveFile();
		}
		//Load
		if (e.key.code == Keyboard::L) 
		{
			LoadFile();
		}

		//Screenshot
		if (e.key.code == Keyboard::Space)
		{
			Texture screenShot;
			screenShot.create(window.getSize().x, window.getSize().y);
			screenShot.update(window);
			if (screenShot.copyToImage().saveToFile(SCREEN_SHOT_NAME))
			{
				cout << "Screenshot saved to " << SCREEN_SHOT_NAME << endl;
			}
		}

	}

	//Place
	if (Mouse::isButtonPressed(Mouse::Left)) 
	{
		Vector2<int> mousePos = MouseTile(window);
		level[mousePos.y][mousePos.x] = tileSelected;
	}

	//Erase
	if (Mouse::isButtonPressed(Mouse::Right)) 
	{
		Vector2<int> mousePos = MouseTile(window);
		level[mousePos.y][mousePos.x] = -1;
	}

	if (e.type == Event::Closed)
		window.close();

	return;
}

void render(RenderWindow& window) 
{
	window.clear();
	//Draw
	for (int i = 0; i < 10; i++) 
	{
		for (int j = 0; j < 10; j++) 
		{
			if (level[i][j] >= 0 && level[i][j] < 21) 
			{
				tileMap[i][j].setTexture(tileTexture[level[i][j]]);
				window.draw(tileMap[i][j]);
			}
		}
	}
	Vector2<int> mousePos = MouseTile(window);
	tileMap[mousePos.y][mousePos.x].setTexture(tileTexture[tileSelected]);
	window.draw(tileMap[mousePos.y][mousePos.x]);
	window.display();
	return;
}

//Grid
Vector2<int> MouseTile(RenderWindow& window) {
	int x, y;
	x = (int)(Mouse::getPosition(window).x - (window.getSize().x % 70) / 2) / 70;
	x = max(0, x);
	x = min(x, 9);
	y = (int)(Mouse::getPosition(window).y - (window.getSize().y % 70) / 2) / 70;
	y = max(0, y);
	y = min(y, 9);
	return Vector2<int>(x, y);

}

//Save to file
void SaveFile() {
	ofstream levelFile;
	string fileName = "level.txt";

	levelFile.open(fileName);

	if (!levelFile.is_open()) 
	{
		cout << "Failed to save the level to file " << fileName << endl;
		return;
	}

	for (int i = 0; i < 10; i++) 
	{
		for (int j = 0; j < 10; j++) {
			levelFile << level[i][j] << " ";
		}
		levelFile << endl;
	}
	levelFile.close();

	cout << "Level saved to file " << fileName << endl;
	return;
}

//Load from file
void LoadFile() 
{
	ifstream levelFile;
	string fileName = "level.txt";

	levelFile.open(fileName);

	if (!levelFile.is_open()) {
		cout << "Failed to load from " << fileName << endl;
		return;
	}

	int fileLevel[10][10];

	for (int i = 0; i < 10; i++) 
	{
		for (int j = 0; j < 10; j++) 
		{
			levelFile >> fileLevel[i][j];
		}
	}

	levelFile.close();

	cout << "Level loaded from " << fileName << endl;
	return;
}

void LoadTiles() 
{

	for (int i = 0; i < 21; i++) 
	{
		if (!tileTexture[i].loadFromFile("Tilesets/Platformer-" + to_string(i) + ".png")) 
		{
			cout << "Failed to load tiles" << endl;
			return;
		}
	}
	
}