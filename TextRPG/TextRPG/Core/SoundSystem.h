#pragma once
#include "external/sound/miniaudio.h"
#include <string>

class SoundSystem
{
public:
    void Init();

    // SFX (효과음) — fire-and-forget
    void PlaySE(const std::string& path);

    // 타이프라이터 클릭 전용 — sfxVolume의 25%로 재생
    void PlayTypewriterSE(const std::string& path);

    // BGM (배경음악) — 루프 재생, 한 곡만 유지
    void PlayBGM(const std::string& path);
    void StopBGM();

    // 볼륨 (0.0 ~ 1.0)
    void SetMasterVolume(float volume);
    void SetBGMVolume(float volume);
    void SetSFXVolume(float volume);

    float GetMasterVolume() const;
    float GetBGMVolume()    const;
    float GetSFXVolume()    const;

private:
    ma_engine      engine{};
    ma_sound_group bgmGroup{};
    ma_sound_group sfxGroup{};
    ma_sound_group typewriterGroup{};   // 타이프라이터 클릭 전용 (sfxVolume의 25%)
    ma_sound       bgmSound{};
    bool           bgmLoaded = false;
    std::string    currentBgmPath;

    float masterVolume = 1.0f;
    float bgmVolume    = 1.0f;
    float sfxVolume    = 1.0f;
};
