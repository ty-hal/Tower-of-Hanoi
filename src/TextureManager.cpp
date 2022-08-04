#include "TextureManager.h"
#include <string>
using namespace std;
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName) {
	string path = "images/" + fileName + ".png";
	textures[fileName].loadFromFile(path);
}

void TextureManager::LoadTexture(string fileName, int num) {
	string path = "images/" + fileName + ".png";
	fileName = fileName + to_string(num);
	textures[fileName].loadFromFile(path, sf::IntRect(21 * num, 0, 21, 32));
}

sf::Texture& TextureManager::GetTexture(string textureName) {
	//if the texture DOESN'T exist...
	if (textures.find(textureName) == textures.end()) {
		LoadTexture(textureName);
	}
	return textures[textureName];
}

sf::Texture& TextureManager::GetTexture(string textureName, int num) {
	//if the texture DOESN'T exist..
	if (textures.find((textureName + to_string(num))) == textures.end()) {
		LoadTexture(textureName, num);
	}
	return textures[textureName + to_string(num)];
}

void TextureManager::Clear() {
	textures.clear(); //Get rid of all stored objects
}