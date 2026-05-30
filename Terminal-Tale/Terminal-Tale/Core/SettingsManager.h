#pragma once
#include <string>

struct KeyBindings
{
	// VK 코드 (Windows Virtual Key)
	// 문자 키: 대문자 ASCII 값  ('I'=0x49, 'Z'=0x5A, ...)
	// 기능 키: VK_F5=0x74, VK_F9=0x78
	int inventoryKey = 'I';
	int waitKey      = 'Z';
	int sleepKey     = 'S';
	int logKey       = 'L';
	int journalKey   = 'J';
	int quickSaveKey = 0x74;  // VK_F5
	int quickLoadKey = 0x78;  // VK_F9
};

struct Settings
{
	int masterVolume = 100;
	int bgmVolume    = 100;
	int sfxVolume    = 100;
	int textSpeed    = 3;
	bool autoSave = true;

	std::string language = "ko";

	int targetFPS = 30;
	bool vsync = false;
	bool showFPS = false;

	bool fullScreen = true;
	bool screenTransition = true;   // 화면 전환 슬라이드 애니메이션

	KeyBindings keyBindings;
};

class SettingsManager
{
public:
	bool Load(const std::string& path);
	bool Save(const std::string& path);

	Settings settings;
};