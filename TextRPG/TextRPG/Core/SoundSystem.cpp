#define MINIAUDIO_IMPLEMENTATION
#include "external/sound/miniaudio.h"

#include "SoundSystem.h"
#include <algorithm>

void SoundSystem::Init()
{
    ma_engine_init(NULL, &engine);
    ma_sound_group_init(&engine, 0, NULL, &bgmGroup);
    ma_sound_group_init(&engine, 0, NULL, &sfxGroup);

    ma_engine_set_volume(&engine, masterVolume);
    ma_sound_group_set_volume(&bgmGroup, bgmVolume);
    ma_sound_group_set_volume(&sfxGroup, sfxVolume);
}

void SoundSystem::PlaySE(const std::string& path)
{
    ma_engine_play_sound(&engine, path.c_str(), &sfxGroup);
}

void SoundSystem::PlayBGM(const std::string& path)
{
    if (bgmLoaded)
    {
        ma_sound_stop(&bgmSound);
        ma_sound_uninit(&bgmSound);
        bgmLoaded = false;
    }

    ma_result result = ma_sound_init_from_file(
        &engine, path.c_str(),
        MA_SOUND_FLAG_STREAM, &bgmGroup, NULL, &bgmSound);

    if (result != MA_SUCCESS)
        return;

    ma_sound_set_looping(&bgmSound, MA_TRUE);
    ma_sound_start(&bgmSound);
    bgmLoaded = true;
}

void SoundSystem::StopBGM()
{
    if (!bgmLoaded) return;
    ma_sound_stop(&bgmSound);
    ma_sound_uninit(&bgmSound);
    bgmLoaded = false;
}

void SoundSystem::SetMasterVolume(float volume)
{
    masterVolume = std::clamp(volume, 0.0f, 1.0f);
    ma_engine_set_volume(&engine, masterVolume);
}

void SoundSystem::SetBGMVolume(float volume)
{
    bgmVolume = std::clamp(volume, 0.0f, 1.0f);
    ma_sound_group_set_volume(&bgmGroup, bgmVolume);
}

void SoundSystem::SetSFXVolume(float volume)
{
    sfxVolume = std::clamp(volume, 0.0f, 1.0f);
    ma_sound_group_set_volume(&sfxGroup, sfxVolume);
}

float SoundSystem::GetMasterVolume() const { return masterVolume; }
float SoundSystem::GetBGMVolume()    const { return bgmVolume; }
float SoundSystem::GetSFXVolume()    const { return sfxVolume; }