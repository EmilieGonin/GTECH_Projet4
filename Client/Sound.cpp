#include "Sound.h"

Sound::Sound()
{

}

Sound::~Sound()
{

}

void Sound::playSound()
{
	sf::SoundBuffer soundBuffer;
	soundBuffer.loadFromFile("");
	mSound.setBuffer(soundBuffer);
	mSound.play();

};