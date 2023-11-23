#pragma once
#include <SFML/Audio.hpp>

class Sound
{
public:
	Sound();
	~Sound();
	void playSound();
private:
	sf::Sound mSound;
};

