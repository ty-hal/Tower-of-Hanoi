#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "TextureManager.h"
#include "SimpleTimer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;
class Game
{
public:
	struct Block {
		bool active;										//True if the block is included in the current game 
		int size;											//Size of the block (1 to 8)
		int whichStick = 0;									//Which stick the block is on (0 = leftmost, 1 = middle, 2 = rightmost)
		string fileName;									//File name of block ("block#.png")
		bool selected = false;								//True if user selects the block with mouse click
		Block(int size, bool active, string fileName) {
			this->size = size;
			this->active = active;
			this->fileName = fileName;
		}
	};
	int height = 500;												//Height of the window
	int width = 500;												//Width of the window
	int numMoves = 0;												//Number of moves used in the current game
	int numBlocks = 3;												//Number of blocks in the current game
	bool gameWon = false;											//True if game is won
	bool rulesStatus = false;										//True if rules display is open
	vector<Block*> blocks;											//Vector of all 8 blocks 
	vector<Block*> firstStick;										//Vector of blocks in the leftmost stick
	vector<Block*> secondStick;										//Vector of blocks in the middle stick
	vector<Block*> thirdStick;										//Vector of blocks in the rightmost stick
	SimpleTimer* timer = new SimpleTimer("Tower of Hanoi timer");	//Game timer object
	chrono::duration<double> currentTime;							//Current time

	//Default constructor
	Game() {
		Block* block1 = new Block(1, true, "block1");
		Block* block2 = new Block(2, true, "block2");
		Block* block3 = new Block(3, true, "block3");
		Block* block4 = new Block(4, false, "block4");
		Block* block5 = new Block(5, false, "block5");
		Block* block6 = new Block(6, false, "block6");
		Block* block7 = new Block(7, false, "block7");
		Block* block8 = new Block(8, false, "block8");
		blocks = { block1, block2, block3, block4, block5, block6, block7, block8 };
		GameReset();
	}

	//Resets game
	void GameReset() {
		firstStick = {};
		secondStick = {};
		thirdStick = {};
		for (int i = blocks.size() - 1; i >= 0; --i) {
			blocks.at(i)->whichStick = 0;
			blocks.at(i)->selected = false;
			if (blocks.at(i)->active) { firstStick.push_back(blocks.at(i)); }
		}
		gameWon = false;
		numMoves = 0;
		rulesStatus = false;
		timer->ResetTime();
	}

	//Determines if the mouse is within the boundaries of a block, and, if so, selects that block
	bool SelectBlock(int xPos, int yPos) {
		for (auto i : blocks) {
			if (i->selected) { return true; }
		}
		if (firstStick.size() != 0) {
			if ((yPos > (height - 251 + 22 * (8 - firstStick.size()))) && (yPos < (height - 251 + 22 * (9 - firstStick.size()))) && (xPos > (100 - 15 - 7.5 * (firstStick.at(firstStick.size() - 1)->size - 1))) && (xPos < (145 + 15 * (firstStick.at(firstStick.size() - 1)->size - 1)))) {
				firstStick.at(firstStick.size() - 1)->selected = true;
				return true;
			}
		}
		if (secondStick.size() != 0) {
			if ((yPos > (height - 251 + 22 * (8 - secondStick.size()))) && (yPos < (height - 251 + 22 * (9 - secondStick.size()))) && (xPos > (width / 2 - 15 - 7.5 * (secondStick.at(secondStick.size() - 1)->size - 1))) && (xPos < (width / 2 + 45 + 15 * (secondStick.at(secondStick.size() - 1)->size - 1)))) {
				secondStick.at(secondStick.size() - 1)->selected = true;
				return true;
			}
		}
		if (thirdStick.size() != 0) {
			if ((yPos > (height - 251 + 22 * (8 - thirdStick.size()))) && (yPos < (height - 251 + 22 * (9 - thirdStick.size()))) && (xPos > (width - 100 - 15 - 7.5 * (thirdStick.at(thirdStick.size() - 1)->size - 1))) && (xPos < (width - 200 + 145 + 15 * (thirdStick.at(thirdStick.size() - 1)->size - 1)))) {
				thirdStick.at(thirdStick.size() - 1)->selected = true;
				return true;
			}
		}


		return false;
	}

	//Checks if user tries to place block on a stick that it cannot go on or on the same stick it is currently on
	void CheckIfBiggest(int xPos, int yPos) {
		for (auto i : blocks) {
			if ((xPos > 60) && (xPos < 150) && (yPos > 200) && (yPos < 420)) {
				if (firstStick.size() != 0) {
					if (firstStick.back()->size <= i->size) {
						i->selected = false;
					}
				}
			}
			if ((xPos > 215) && (xPos < 300) && (yPos > 200) && (yPos < 420)) {
				if (secondStick.size() != 0) {
					if (secondStick.back()->size <= i->size) {
						i->selected = false;
					}
				}
			}
			if ((xPos > 365) && (xPos < 450) && (yPos > 200) && (yPos < 420)) {
				if (thirdStick.size() != 0) {
					if (thirdStick.back()->size <= i->size) {
						i->selected = false;
					}
				}
			}
		}
	}

	//Checks if block is attempted to be place outside of one of the three sticks and then places it back on the stick it was currently on
	void CheckIfOutOfRange(int xPos, int yPos) {
		for (auto i : blocks) {
			if (i->selected == true) {
				if ((xPos < 60) || (xPos > 450) || (yPos > 420) || (yPos < 200)) {
					i->selected = false;
				}
			}
		}
	}

	//Moves the selected block if the user clicks within the boundaries of another stick and it follows the rules of the game
	void MakeMove(int xPos, int yPos) {
		SelectBlock(xPos, yPos);
		CheckIfBiggest(xPos, yPos);
		CheckIfOutOfRange(xPos, yPos);

		for (auto i : blocks) {
			if (i->selected) {
				if ((xPos > 30) && (xPos < 175) && (yPos > 250) && (yPos < 420) && i->whichStick != 0) {
					if (firstStick.size() != 0) {
						if (firstStick.back()->size < i->size) {
							continue;
						}
					}
					if (i->whichStick == 1) { secondStick.pop_back(); }
					else if (i->whichStick == 2) { thirdStick.pop_back(); }
					firstStick.push_back(i);
					i->whichStick = 0;
					++numMoves;
					for (auto j : blocks) {
						j->selected = false;
					}
				}
				if ((xPos > 180) && (xPos < 325) && (yPos > 250) && (yPos < 420) && i->whichStick != 1) {
					if (secondStick.size() != 0) {
						if (secondStick.back()->size < i->size) {
							continue;
						}
					}
					if (i->whichStick == 0) { firstStick.pop_back(); }
					else if (i->whichStick == 2) { thirdStick.pop_back(); }
					secondStick.push_back(i);
					i->whichStick = 1;
					++numMoves;
					for (auto j : blocks) {
						j->selected = false;
					}
				}
				if ((xPos > 330) && (xPos < 475) && (yPos > 250) && (yPos < 420) && i->whichStick != 2) {
					if (thirdStick.size() != 0) {
						if (thirdStick.back()->size < i->size) {
							continue;
						}
					}if (i->whichStick == 0) { firstStick.pop_back(); }
					else if (i->whichStick == 1) { secondStick.pop_back(); }
					thirdStick.push_back(i);
					i->whichStick = 2;
					++numMoves;
					for (auto j : blocks) {
						j->selected = false;
					}
				}
			}
		}
	}

	//Increases or decreases the number of blocks accordingly
	void ResizeGame(bool upOrDown) {
		//If upOrDown = true, then increase the size
		//Else, decrease the size
		if (upOrDown) {
			if (numBlocks < 8) {
				blocks.at(numBlocks)->active = true;
				firstStick.push_back(blocks.at(numBlocks));
				++numBlocks;
			}
		}
		else {
			if (numBlocks > 3) {
				blocks.at(numBlocks - 1)->active = false;
				firstStick.pop_back();
				--numBlocks;
			}
		}
		GameReset();
	}

	//Gets the current time of the game
	double GetCurrentTime(bool gameWon) {
		if (!gameWon) {
			currentTime = timer->GetTime(); //If game is still going, return current time
		}
		return currentTime.count();    //If game is won, return previous time.
	}

	//Checks if game is won
	bool VictoryCheck() {
		bool userWon;
		for (auto i : blocks) {
			if (i->active) {
				if (i->whichStick != 2) { return false; }
			}
		}
		return true;
	}

	//Opens rules
	void OpenRules() {
		if (rulesStatus) { rulesStatus = false; }
		else { rulesStatus = true; }
	}
};