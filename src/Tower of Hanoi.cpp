#include <cmath> 
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "Game.h"
#include "SimpleTimer.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#define height 500
#define width 500
using namespace std;
void CreateShapes(map<string, sf::RectangleShape>& rectShapes, map<string, sf::CircleShape>& circleShapes); //Creates maps of sf::Shapes
void DrawBoard(sf::RenderWindow& window, map<string, sf::RectangleShape>& rectShapes, map<string, sf::CircleShape>& circleShapes); //Draws the board (base and sticks)
void DrawDisplay(sf::RenderWindow& window, Game* game, sf::Vector2i mousePosition); //Draws everything else: blocks, menu, arrows, moves, etc.


int main()
{
	//Create the window and board
	sf::RenderWindow window(sf::VideoMode(width, height), "Tower of Hanoi");
	Game* game = new Game();

	/* ==== DEFINE/CREATE SHAPES ==== */
	map<string, sf::RectangleShape> rectShapes;
	map<string, sf::CircleShape> circleShapes;
	CreateShapes(rectShapes, circleShapes); //Create two maps -- one for rectangular shapes (base and sticks) and one for circular shapes (rounded edges)

	sf::Vector2i mousePosition = sf::Mouse::getPosition();
	sf::Clock gameClock;
	bool welcome = true; //Used to open/close welcome screen

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {	//If user EXITS
				window.close();
			}
			if (welcome) { //Welcome screen
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					mousePosition = sf::Mouse::getPosition(window);
					if ((mousePosition.x > 148) && (mousePosition.x < 237) && (mousePosition.y > 459) && (mousePosition.y < 486)) { //Open/close rules
						game->OpenRules();
					}
					if ((mousePosition.x > 278) && (mousePosition.x < 361) && (mousePosition.y > 459) && (mousePosition.y < 486)) { //Start the game
						welcome = false;
					}
				}
			}
			if (!welcome && event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {  //If user LEFT clicks
					//Get mouse position
					mousePosition = sf::Mouse::getPosition(window); //Get mouse position
					//cout << "x: " << mousePosition.x << "  and y: " << mousePosition.y << endl;

					//Increase the number of blocks by 1
					if ((mousePosition.x > 10) && (mousePosition.x < 37) && (mousePosition.y > 42) && (mousePosition.y < 64)) {
						game->ResizeGame(true);
					}

					//Decrease the number of blocks by 1
					if ((mousePosition.x > 37) && (mousePosition.x < 67) && (mousePosition.y > 42) && (mousePosition.y < 64)) {
						game->ResizeGame(false);
					}

					//Reset game
					if ((mousePosition.x > (width - 95)) && (mousePosition.x < (width - 10)) && (mousePosition.y > 5) && (mousePosition.y < 30)) {
						game->GameReset();
					}

					//Open rule display
					if ((mousePosition.x > (width - 210)) && (mousePosition.x < (width - 125)) && (mousePosition.y > 5) && (mousePosition.y < 30)) {
						game->OpenRules();
					}
					else { game->rulesStatus = false; }

					//Allows for user to make a move by clicking blocks instead of dragging and dropping
					game->MakeMove(mousePosition.x, mousePosition.y);
				}
			}
			if (!welcome && sf::Mouse::isButtonPressed(sf::Mouse::Left) && gameClock.getElapsedTime().asMilliseconds() >= 100) { //If user holds onto their mouse button
				mousePosition = sf::Mouse::getPosition(window);
				game->SelectBlock(mousePosition.x, mousePosition.y);

			}

			if (!welcome && event.type == sf::Event::MouseButtonReleased) { //If user releases their mouse button
				mousePosition = sf::Mouse::getPosition(window);
				game->MakeMove(mousePosition.x, mousePosition.y);
			}
		}

		//Sets the board's background to grey
		window.clear(sf::Color(19, 23, 28));

		//Draw everything in the game if the user is not on the welcome screen
		if (!welcome) {
			DrawBoard(window, rectShapes, circleShapes);
			DrawDisplay(window, game, mousePosition);
		}

		//Draw the welcome screen if the user has not started the game yet
		if (welcome) {
			sf::Sprite welcome(TextureManager::GetTexture("welcome screen"));
			welcome.setPosition(0, 0);
			window.draw(welcome);

			if (game->rulesStatus) {
				sf::Sprite rules(TextureManager::GetTexture("rules list"));
				rules.setPosition(90, 120);
				window.draw(rules);
			}
		}

		//Display the window
		window.display();
	}
	return 0;
}

void CreateShapes(map<string, sf::RectangleShape>& rectShapes, map<string, sf::CircleShape>& circleShapes) {
	sf::RectangleShape base;
	base.setFillColor(sf::Color(164, 116, 73));
	base.setSize(sf::Vector2f(width - 50, 25));
	base.setPosition(25, height - 25 - 50);
	rectShapes.emplace("base", base);

	sf::RectangleShape stick;
	stick.setFillColor(sf::Color(164, 116, 73));
	stick.setSize(sf::Vector2f(15, height - 300));
	rectShapes.emplace("stick", stick);

	sf::CircleShape baseEdge(12.5f);
	baseEdge.setFillColor(sf::Color(164, 116, 73));
	circleShapes.emplace("baseEdge", baseEdge);

	sf::CircleShape topRoundedEdge(7.5f);
	topRoundedEdge.setFillColor(sf::Color(164, 116, 73));
	circleShapes.emplace("topRoundedEdge", topRoundedEdge);
}
void DrawBoard(sf::RenderWindow& window, map<string, sf::RectangleShape>& rectShapes, map<string, sf::CircleShape>& circleShapes) {
	window.draw(rectShapes.at("base"));

	rectShapes.at("stick").setPosition(100, height - 275);
	window.draw(rectShapes.at("stick"));

	rectShapes.at("stick").setPosition(width / 2, height - 275);
	window.draw(rectShapes.at("stick"));

	rectShapes.at("stick").setPosition(width - 100, height - 275);
	window.draw(rectShapes.at("stick"));

	circleShapes.at("topRoundedEdge").setPosition(100, height - 275 - 7.5);
	window.draw(circleShapes.at("topRoundedEdge"));

	circleShapes.at("topRoundedEdge").setPosition(width / 2, height - 275 - 7.5);
	window.draw(circleShapes.at("topRoundedEdge"));

	circleShapes.at("topRoundedEdge").setPosition(width - 100, height - 275 - 7.5);
	window.draw(circleShapes.at("topRoundedEdge"));

	circleShapes.at("baseEdge").setPosition(12.5f, height - 25 - 50);
	window.draw(circleShapes.at("baseEdge"));

	circleShapes.at("baseEdge").setPosition(width - 25 - 12.5, height - 25 - 50);
	window.draw(circleShapes.at("baseEdge"));
}
void DrawDisplay(sf::RenderWindow& window, Game* game, sf::Vector2i mousePosition) {
	//===== DISKS LABEL AND ARROWS =====
	sf::Sprite disksLabel(TextureManager::GetTexture("disksLabel"));
	disksLabel.setPosition(10, 5);
	window.draw(disksLabel);

	sf::Sprite arrows(TextureManager::GetTexture("arrows"));
	arrows.setPosition(10, 40); //(10,40) to (30,60)  and  (30,40) to (50,60)
	window.draw(arrows);

	//===== NUMBER OF DISKS =====
	sf::Sprite numBlocks(TextureManager::GetTexture("digits", game->numBlocks));
	numBlocks.setPosition(85, 8);
	window.draw(numBlocks);

	//==== MOVES LABEL
	sf::Sprite movesLabel(TextureManager::GetTexture("movesLabel"));
	movesLabel.setPosition(130, 5);
	window.draw(movesLabel);


	int first = (game->numMoves) / 100;
	int second = (game->numMoves % 100) / 10;
	int third = game->numMoves % 10;

	if (game->numMoves < 10) {
		sf::Sprite thirdDigitMove(TextureManager::GetTexture("digits", third));
		thirdDigitMove.setPosition(220, 8);
		window.draw(thirdDigitMove);
	}
	else if (game->numMoves < 100) {
		sf::Sprite secondDigitMove(TextureManager::GetTexture("digits", second));
		secondDigitMove.setPosition(220, 8);
		window.draw(secondDigitMove);

		sf::Sprite thirdDigitMove(TextureManager::GetTexture("digits", third));
		thirdDigitMove.setPosition(241, 8);
		window.draw(thirdDigitMove);
	}
	else {
		sf::Sprite firstDigitMove(TextureManager::GetTexture("digits", first));
		firstDigitMove.setPosition(220, 8);
		window.draw(firstDigitMove);

		sf::Sprite secondDigitMove(TextureManager::GetTexture("digits", second));
		secondDigitMove.setPosition(241, 8);
		window.draw(secondDigitMove);

		sf::Sprite thirdDigitMove(TextureManager::GetTexture("digits", third));
		thirdDigitMove.setPosition(262, 8);
		window.draw(thirdDigitMove);
	}


	//===== RESET ======
	sf::Sprite reset(TextureManager::GetTexture("reset")); //(width - 95, 5) to (490,30)
	reset.setPosition(width - 100, 0);
	window.draw(reset);

	//===== MINIMUM MOVES =====
	sf::Sprite minmoves(TextureManager::GetTexture("minmoves"));
	minmoves.setPosition(width - 280, height - 37);
	window.draw(minmoves);

	first = (exp2(game->numBlocks) - 1) / 100;
	second = (((int)exp2(game->numBlocks) - 1) % 100) / 10;
	third = ((int)exp2(game->numBlocks) - 1) % 10;

	sf::Sprite firstDigit(TextureManager::GetTexture("digits", first));
	firstDigit.setPosition(width - 72, height - 35);
	window.draw(firstDigit);

	sf::Sprite secondDigit(TextureManager::GetTexture("digits", second));
	secondDigit.setPosition(width - 51, height - 35);
	window.draw(secondDigit);

	sf::Sprite thirdDigit(TextureManager::GetTexture("digits", third));
	thirdDigit.setPosition(width - 30, height - 35);
	window.draw(thirdDigit);

	//===== BLOCKS =====
	int firstCol = 0;
	int secondCol = 0;
	int thirdCol = 0;
	float xPos = 0;
	int yOffSet = 0;

	for (int i = game->blocks.size() - 1; i >= 0; --i) {
		if (game->blocks.at(i)->active && !game->blocks.at(i)->selected) {  //If its not selected
			if (game->blocks.at(i)->whichStick == 0) {
				xPos = 100 - 15 - 7.5 * (game->blocks.at(i)->size - 1);
				yOffSet = 154 - 22 * firstCol;
				++firstCol;
			}
			else if (game->blocks.at(i)->whichStick == 1) {
				xPos = width / 2 - 15 - 7.5 * (game->blocks.at(i)->size - 1);
				yOffSet = 154 - 22 * secondCol;
				++secondCol;
			}
			else {
				xPos = width - 100 - 15 - 7.5 * (game->blocks.at(i)->size - 1);
				yOffSet = 154 - 22 * thirdCol;
				++thirdCol;
			}
			sf::Sprite block1(TextureManager::GetTexture(game->blocks.at(i)->fileName));
			block1.setPosition(xPos, height - 251 + yOffSet);
			window.draw(block1);
		}
		if (game->blocks.at(i)->selected) {
			sf::Sprite block1(TextureManager::GetTexture(game->blocks.at(i)->fileName));
			block1.setPosition(mousePosition.x, mousePosition.y);
			window.draw(block1);
		}
	}

	//==== Is game won? ====
	if (game->VictoryCheck()) {
		sf::Sprite won(TextureManager::GetTexture("won"));
		won.setPosition(175, 100);
		window.draw(won);
	}

	//===== PRINT TIME ====
	if (game->GetCurrentTime(game->VictoryCheck()) < 998) {
		first = (game->GetCurrentTime(game->VictoryCheck())) / 100;
		second = ((int)(game->GetCurrentTime(game->VictoryCheck())) % 100) / 10;
		third = ((int)game->GetCurrentTime(game->VictoryCheck())) % 10;
	}
	else {
		first = 9;
		second = 9;
		third = 9;
	}

	sf::Sprite hundreds(TextureManager::GetTexture("digits", first));
	hundreds.setPosition(75, height - 35);
	window.draw(hundreds);

	sf::Sprite tens(TextureManager::GetTexture("digits", second));
	tens.setPosition(97, height - 35);
	window.draw(tens);

	sf::Sprite ones(TextureManager::GetTexture("digits", third));
	ones.setPosition(119, height - 35);
	window.draw(ones);

	sf::Sprite time(TextureManager::GetTexture("time"));
	time.setPosition(10, height - 35);
	window.draw(time);

	//===== RULES =========
	sf::Sprite rules(TextureManager::GetTexture("rules"));
	rules.setPosition(width - 210, 0); //(width - 210,0) to (width - 125, 30)
	window.draw(rules);

	if (game->rulesStatus) {
		//PRINT RULES HERE
		sf::Sprite listOfRules(TextureManager::GetTexture("rules list"));
		listOfRules.setPosition(75, 50);
		window.draw(listOfRules);
	}
}