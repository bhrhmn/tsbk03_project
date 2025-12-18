#include <cstdio>
#include "sounds.h"
sf::Sound* fireSound = nullptr;

void initFireplaceSound() {
    static sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("fireplace.wav")) {
        std::printf("Error: Could not load sound file!\n");
        return;
    }

    fireSound = new sf::Sound(buffer);
    fireSound->setLooping(true);
    fireSound->play();
}