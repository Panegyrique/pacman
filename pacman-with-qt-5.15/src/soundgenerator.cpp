#include <iostream>
#include "soundgenerator.hpp"

using namespace std;

SoundGenerator::SoundGenerator() {
    
	FMOD_System_Create(&system, FMOD_VERSION);
    FMOD_System_Init(system, 8, FMOD_INIT_NORMAL, nullptr);
    FMOD_System_CreateChannelGroup(system, "ChannelGroup", &channelgroup);
}

SoundGenerator::~SoundGenerator() {
    //FMOD_System_Close(system);
}

