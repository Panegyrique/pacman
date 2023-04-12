#ifndef SOUNDGENERATOR_HPP
#define SOUNDGENERATOR_HPP

#include <fmod.h>
#include <fmod_errors.h>

class SoundGenerator {
public:
    SoundGenerator();
    ~SoundGenerator();

    FMOD_SYSTEM *system;
	FMOD_RESULT result;
	FMOD_CHANNELGROUP *channelgroup = 0;
};

#endif // SOUNDGENERATOR_HPP
