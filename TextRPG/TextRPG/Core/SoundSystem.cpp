#define MINIAUDIO_IMPLEMENTATION
#include "external/sound/miniaudio.h"

#include "SoundSystem.h"
#include <iostream>
#include <algorithm>


void SoundSystem::Init()
{
	ma_engine_init(NULL , &engine);
	ma_engine_set_volume(&engine , masterVolume);
}


void SoundSystem::PlaySE(const std::string& path)
{
	ma_engine_play_sound(&engine , path.c_str() , NULL);
}

void SoundSystem::SetMasterVolume(float volume)
{
	masterVolume = std::clamp(volume , 0.0f , 1.0f);

	ma_engine_set_volume(&engine , masterVolume);
}

float SoundSystem::GetMasterVolume() const
{
	return masterVolume;
}