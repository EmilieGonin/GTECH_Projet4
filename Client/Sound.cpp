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

	soundBuffer.loadFromFile("clic.mp3");
	mSound.setBuffer(soundBuffer);
	mSound.play();

}