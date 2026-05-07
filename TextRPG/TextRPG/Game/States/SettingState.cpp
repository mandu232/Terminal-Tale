#include "SettingState.h"
#include "Core/InputManager.h"
#include "Core/Context.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Core/UIButton.h"
#include "Core/UILabel.h"
#include "Core/UIImage.h"
#include <string>
#include <vector>
#include <algorithm>

// ─────────────────────────────────────────────
//  레이아웃 상수
//
//  UILabel  생성자: (x, y, z, width, text, color, align)
//  UIButton 생성자: (x, y, w, h, z, text, callback)
//
//  화면 구성
//  x=10  항목 이름 라벨      width=28
//  x=40  ◀ 버튼              w=5
//  x=47  현재 값 라벨        width=13
//  x=62  ▶ 버튼              w=5
//  (bool 항목은 x=40 토글 버튼 width=16)
// ─────────────────────────────────────────────
namespace Layout
{
    constexpr int Z          = 10;
    constexpr int RowH       = 3;

    // 항목 이름 라벨
    constexpr int LabelX     = 10;
    constexpr int LabelW     = 28;

    // ◀  값  ▶
    constexpr int ArrowLX    = 40;
    constexpr int ValueX     = 47;
    constexpr int ValueW     = 13;
    constexpr int ArrowRX    = 62;
    constexpr int ArrowW     = 5;

    // bool 토글 버튼
    constexpr int ToggleX    = 40;
    constexpr int ToggleLblW = 8;   // "ON"/"OFF" 값 라벨 너비
    constexpr int ToggleBtnX = 50;  // 토글 버튼 x
    constexpr int ToggleBtnW = 10;

    // y 좌표
    constexpr int TitleY        = 4;
    constexpr int FullScreenY   = 13;
    constexpr int MasterVolumeY = 18;
    constexpr int TextSpeedY    = 23;
    constexpr int AutoSaveY     = 28;
    constexpr int LanguageY     = 33;
    constexpr int TargetFPSY    = 38;
    constexpr int ShowFPSY      = 43;

    // 하단 버튼
    constexpr int BottomY    = 50;
    constexpr int ResetX     = 129;
    constexpr int SaveX      = 150;
    constexpr int BackX      = 171;
    constexpr int BtnW       = 16;
}

// ─────────────────────────────────────────────
//  순환 목록 (Enter 에서 람다가 캡처)
// ─────────────────────────────────────────────
static const std::vector<std::string> langList  = { "ko", "en", "ja", "zh"};
static const std::vector<int>         fpsList   = { 15, 30, 60, 120 };

// ─────────────────────────────────────────────
//  헬퍼
// ─────────────────────────────────────────────
std::string SettingState::BoolToStr(bool value)
{
    return value ? L("status.on") : L("status.off");
}

std::string SettingState::LangToStr(const std::string& lang)
{
    if (lang == "ko") return "한국어";
    if (lang == "en") return "English";
	if (lang == "zh") return "中國語";
	if (lang == "ja") return "日本語";
    return lang;
}

// ─────────────────────────────────────────────
//  생성자
// ─────────────────────────────────────────────
SettingState::SettingState(Context& context)
    : State(context)
{
}

// ─────────────────────────────────────────────
//  Enter — UI 전체 구성
// ─────────────────────────────────────────────
void SettingState::Enter()
{
    auto& s = context.settingManager.settings;

    // ── 타이틀 ──────────────────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            Layout::LabelX, Layout::TitleY, Layout::Z,
            Layout::LabelW , Layout::RowH, L("ui.setting"), 14,
			UILabel::TextAlign::Center,
			UILabel::VAlign::Middle)
    );

    // ────────────────────────────────────────────
    //  전체화면  (bool)
    //  [항목명]   ON/OFF  [변경]
    // ────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            Layout::LabelX, Layout::FullScreenY, Layout::Z,
            Layout::LabelW, Layout::RowH, L("ui.fullScreen"), 7,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle)
    );
    {
		auto lbl = std::make_unique<UILabel>(
			Layout::ToggleX , Layout::FullScreenY , Layout::Z ,
			Layout::ToggleLblW , Layout::RowH , BoolToStr(s.fullScreen) , 14 ,
			UILabel::TextAlign::Center,
			UILabel::VAlign::Middle);
        fullScreenLabel = lbl.get();
        uiManager.Add(std::move(lbl));
    }
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ToggleBtnX, Layout::FullScreenY,
            Layout::ToggleBtnW, Layout::RowH, Layout::Z,
            L("ui.toggle"),
            [this]()
            {
                auto& v = context.settingManager.settings.fullScreen;
                v = !v;
                context.ToggleFullscreen(v);
                fullScreenLabel->SetText(BoolToStr(v));
            })
    );

    // ────────────────────────────────────────────
    //  마스터 볼륨  (int 0~100, 스텝 5)
    //  [항목명]   ◀  50  ▶
    // ────────────────────────────────────────────
	uiManager.Add(
		std::make_unique<UILabel>(
			Layout::LabelX , Layout::MasterVolumeY , Layout::Z ,
			Layout::LabelW , Layout::RowH , L("ui.masterVolume") , 7 ,
			UILabel::TextAlign::Center,
			UILabel::VAlign::Middle)
    );
    {
        auto lbl = std::make_unique<UILabel>(
            Layout::ValueX, Layout::MasterVolumeY, Layout::Z,
            Layout::ValueW, Layout::RowH, std::to_string(s.masterVolume), 14,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle);
        masterVolumeLabel = lbl.get();
        uiManager.Add(std::move(lbl));
    }
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ArrowLX, Layout::MasterVolumeY,
            Layout::ArrowW, Layout::RowH, Layout::Z,
            "◀",
            [this]()
            {
                auto& v = context.settingManager.settings.masterVolume;
                if (v > 0) v -= 5;
                masterVolumeLabel->SetText(std::to_string(v));
            })
    );
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ArrowRX, Layout::MasterVolumeY,
            Layout::ArrowW, Layout::RowH, Layout::Z,
            "▶",
            [this]()
            {
                auto& v = context.settingManager.settings.masterVolume;
                if (v < 100) v += 5;
                masterVolumeLabel->SetText(std::to_string(v));
            })
    );

    // ────────────────────────────────────────────
    //  텍스트 속도  (int 1~5, 스텝 1)
    // ────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            Layout::LabelX, Layout::TextSpeedY, Layout::Z,
            Layout::LabelW, Layout::RowH, L("ui.textSpeed"), 7,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle)
    );
    {
        auto lbl = std::make_unique<UILabel>(
            Layout::ValueX, Layout::TextSpeedY, Layout::Z,
            Layout::ValueW, Layout::RowH, std::to_string(s.textSpeed), 14,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle);
        textSpeedLabel = lbl.get();
        uiManager.Add(std::move(lbl));
    }
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ArrowLX, Layout::TextSpeedY,
            Layout::ArrowW, Layout::RowH, Layout::Z,
            "◀",
            [this]()
            {
                auto& v = context.settingManager.settings.textSpeed;
                if (v > 1) --v;
                textSpeedLabel->SetText(std::to_string(v));
            })
    );
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ArrowRX, Layout::TextSpeedY,
            Layout::ArrowW, Layout::RowH, Layout::Z,
            "▶",
            [this]()
            {
                auto& v = context.settingManager.settings.textSpeed;
                if (v < 5) ++v;
                textSpeedLabel->SetText(std::to_string(v));
            })
    );

    // ────────────────────────────────────────────
    //  자동 저장  (bool)
    // ────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            Layout::LabelX, Layout::AutoSaveY, Layout::Z,
            Layout::LabelW, Layout::RowH, L("ui.autoSave"), 7,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle)
    );
    {
        auto lbl = std::make_unique<UILabel>(
            Layout::ToggleX, Layout::AutoSaveY, Layout::Z,
            Layout::ToggleLblW, Layout::RowH, BoolToStr(s.autoSave), 14,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle);
        autoSaveLabel = lbl.get();
        uiManager.Add(std::move(lbl));
    }
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ToggleBtnX, Layout::AutoSaveY,
            Layout::ToggleBtnW, Layout::RowH, Layout::Z,
            L("ui.toggle"),
            [this]()
            {
                auto& v = context.settingManager.settings.autoSave;
                v = !v;
                autoSaveLabel->SetText(BoolToStr(v));
            })
    );

    // ────────────────────────────────────────────
    //  언어  (ko → en 순환)
    // ────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            Layout::LabelX, Layout::LanguageY, Layout::Z,
            Layout::LabelW, Layout::RowH, L("ui.language"), 7,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle)
    );
    {
        auto lbl = std::make_unique<UILabel>(
            Layout::ValueX, Layout::LanguageY, Layout::Z,
            Layout::ValueW, Layout::RowH, LangToStr(s.language), 14,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle);
        languageLabel = lbl.get();
        uiManager.Add(std::move(lbl));
    }
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ArrowLX, Layout::LanguageY,
            Layout::ArrowW, Layout::RowH, Layout::Z,
            "◀",
            [this]()
            {
                auto& v = context.settingManager.settings.language;
                auto it = std::find(langList.begin(), langList.end(), v);
                if (it == langList.begin()) it = langList.end();
                --it;
                v = *it;
                languageLabel->SetText(LangToStr(v));
            })
    );
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ArrowRX, Layout::LanguageY,
            Layout::ArrowW, Layout::RowH, Layout::Z,
            "▶",
            [this]()
            {
                auto& v = context.settingManager.settings.language;
                auto it = std::find(langList.begin(), langList.end(), v);
                if (it != langList.end()) ++it;
                if (it == langList.end()) it = langList.begin();
                v = *it;
                languageLabel->SetText(LangToStr(v));
            })
    );

    // ────────────────────────────────────────────
    //  목표 FPS  (15 / 30 / 60 / 120 순환)
    // ────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            Layout::LabelX, Layout::TargetFPSY, Layout::Z,
            Layout::LabelW, Layout::RowH, L("ui.targetFPS"), 7,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle)
    );
    {
        auto lbl = std::make_unique<UILabel>(
            Layout::ValueX, Layout::TargetFPSY, Layout::Z,
            Layout::ValueW, Layout::RowH, std::to_string(s.targetFPS), 14,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle);
        targetFPSLabel = lbl.get();
        uiManager.Add(std::move(lbl));
    }
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ArrowLX, Layout::TargetFPSY,
            Layout::ArrowW, Layout::RowH, Layout::Z,
            "◀",
            [this]()
            {
                auto& v = context.settingManager.settings.targetFPS;
                auto it = std::find(fpsList.begin(), fpsList.end(), v);
                if (it == fpsList.begin()) it = fpsList.end();
                --it;
                v = *it;
                targetFPSLabel->SetText(std::to_string(v));
            })
    );
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ArrowRX, Layout::TargetFPSY,
            Layout::ArrowW, Layout::RowH, Layout::Z,
            "▶",
            [this]()
            {
                auto& v = context.settingManager.settings.targetFPS;
                auto it = std::find(fpsList.begin(), fpsList.end(), v);
                if (it != fpsList.end()) ++it;
                if (it == fpsList.end()) it = fpsList.begin();
                v = *it;
                targetFPSLabel->SetText(std::to_string(v));
            })
    );

    // ────────────────────────────────────────────
    //  FPS 표시  (bool)
    // ────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UILabel>(
            Layout::LabelX, Layout::ShowFPSY, Layout::Z,
            Layout::LabelW, Layout::RowH, L("ui.showFPS"), 7,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle)
    );
    {
        auto lbl = std::make_unique<UILabel>(
            Layout::ToggleX, Layout::ShowFPSY, Layout::Z,
            Layout::ToggleLblW, Layout::RowH, BoolToStr(s.showFPS), 14,
			UILabel::TextAlign::Center ,
			UILabel::VAlign::Middle);
        showFPSLabel = lbl.get();
        uiManager.Add(std::move(lbl));
    }
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ToggleBtnX, Layout::ShowFPSY,
            Layout::ToggleBtnW, Layout::RowH, Layout::Z,
            L("ui.toggle"),
            [this]()
            {
                auto& v = context.settingManager.settings.showFPS;
                v = !v;
                showFPSLabel->SetText(BoolToStr(v));
            })
    );

    // ────────────────────────────────────────────
    //  하단 버튼: Reset / Save / Back
    // ────────────────────────────────────────────
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::ResetX, Layout::BottomY,
            Layout::BtnW, Layout::RowH, Layout::Z,
            L("ui.reset"),
            [this]()
            {
                // 파일 재로드 후 모든 값 라벨 갱신
                context.settingManager.Load("Data/settings.json");
                RefreshValueLabels();
            })
    );
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::SaveX, Layout::BottomY,
            Layout::BtnW, Layout::RowH, Layout::Z,
            L("ui.save"),
            [this]()
            {
                context.settingManager.Save("Data/settings.json");
            })
    );
    uiManager.Add(
        std::make_unique<UIButton>(
            Layout::BackX, Layout::BottomY,
            Layout::BtnW, Layout::RowH, Layout::Z,
            L("ui.back"),
            [this]()
            {
                context.PopState();
            })
    );
}

// ─────────────────────────────────────────────
//  RefreshValueLabels — Reset 후 전체 동기화
// ─────────────────────────────────────────────
void SettingState::RefreshValueLabels()
{
    auto& s = context.settingManager.settings;

    if (fullScreenLabel)   fullScreenLabel->SetText(BoolToStr(s.fullScreen));
    if (masterVolumeLabel) masterVolumeLabel->SetText(std::to_string(s.masterVolume));
    if (textSpeedLabel)    textSpeedLabel->SetText(std::to_string(s.textSpeed));
    if (autoSaveLabel)     autoSaveLabel->SetText(BoolToStr(s.autoSave));
    if (languageLabel)     languageLabel->SetText(LangToStr(s.language));
    if (targetFPSLabel)    targetFPSLabel->SetText(std::to_string(s.targetFPS));
    if (showFPSLabel)      showFPSLabel->SetText(BoolToStr(s.showFPS));
}

// ─────────────────────────────────────────────
//  HandleInput / Update / Render
// ─────────────────────────────────────────────
void SettingState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        // 필요 시 키보드 단축키 처리 가능
        // ex) ESC → context.PopState();
    }
}

void SettingState::Update()
{
    // 콜백에서 즉시 갱신하므로 매 프레임 갱신 불필요
}

void SettingState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
