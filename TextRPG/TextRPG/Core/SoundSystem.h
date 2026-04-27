#pragma once
#include "external/sound/miniaudio.h"
#include <string>

class SoundSystem
{
public:
	void Init();
	void PlaySE(const std::string& path);

	void SetMasterVolume(float volume);
	float GetMasterVolume() const;

private:
	ma_engine engine{};
	float masterVolume = 1.0f;
};