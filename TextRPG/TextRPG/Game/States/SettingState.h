#pragma once

#include "Core/State.h"
#include "Core/EventSubscription.h"
#include <string>

class UILabel;

class SettingState : public State
{
public:
    SettingState(Context& context);
    void Enter() override;
    void HandleInput(InputManager& input) override;
    void Update() override;
    void Render(ConsoleDisplay& display) override;

private:
    // 각 설정값을 화면에 표시하는 라벨 포인터 (소유권은 uiManager)
    UILabel* fullScreenLabel   = nullptr;
    UILabel* masterVolumeLabel = nullptr;
    UILabel* bgmVolumeLabel    = nullptr;
    UILabel* sfxVolumeLabel    = nullptr;
    UILabel* textSpeedLabel    = nullptr;
    UILabel* autoSaveLabel     = nullptr;
    UILabel* languageLabel     = nullptr;
    UILabel* targetFPSLabel    = nullptr;
    UILabel* showFPSLabel      = nullptr;

    // Save 전 임시 설정값 (Reset 버튼으로 원복 가능)
    // Settings tempSettings;

    // 현재 설정값에 맞게 모든 값 라벨을 갱신
    void RefreshValueLabels();

    // 헬퍼: bool → "ON" / "OFF"
    static std::string BoolToStr(bool value);

    // 헬퍼: language 코드 → 표시 문자열
    static std::string LangToStr(const std::string& lang);
};
