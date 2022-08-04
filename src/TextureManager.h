#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;

class TextureManager
{
	static unordered_map<string, sf::Texture> textures;
public:
	static sf::Texture& GetTexture(string textureName);
	static sf::Texture& GetTexture(string textureName, int num);
	static void LoadTexture(string fileName);
	static void LoadTexture(string fileName, int num);

	static void Clear();
};

