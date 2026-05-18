#define NOMINMAX
#include "PauseMenuState.h"
#include "TitleState.h"
#include "SettingState.h"

#include "Core/Context.h"
#include "Core/InputManager.h"
#include "Core/ConsoleDisplay.h"
#include "Core/Localization.h"
#include "Ui/UIButton.h"
#include "Ui/UILabel.h"

#include <cstdlib>

// ─────────────────────────────────────────────
//  레이아웃
// ─────────────────────────────────────────────
namespace PL
{
    constexpr int Z       = 20;
    constexpr int PanelW  = 44;
    constexpr int PanelX  = (192 - PanelW) / 2;   // 74
    constexpr int PanelY  = 14;
    constexpr int PanelH  = 26;

    constexpr int BtnX    = PanelX + 4;
    constexpr int BtnW    = PanelW - 8;
    constexpr int BtnH    = 3;
    constexpr int BtnGap  = 1;

    constexpr int TitleY  = PanelY + 2;
    constexpr int Btn0Y   = TitleY + 4;           // 게임 저장
    constexpr int Btn1Y   = Btn0Y + BtnH + BtnGap; // 설정
    constexpr int Btn2Y   = Btn1Y + BtnH + BtnGap; // 로비로 돌아가기
    constexpr int Btn3Y   = Btn2Y + BtnH + BtnGap; // 게임 종료
    constexpr int ClosY   = Btn3Y + BtnH + 2;
}

// ─────────────────────────────────────────────
PauseMenuState::PauseMenuState(Context& context)
    : State(context)
{}

void PauseMenuState::Enter()
{
    Rebuild();
}

void PauseMenuState::Rebuild()
{
    uiManager.Clear();

    // ── 제목 ──────────────────────────────────
    uiManager.Add(std::make_unique<UILabel>(
        PL::PanelX, PL::TitleY, PL::Z,
        PL::PanelW, PL::BtnH, L("ui.menu"),
        7, UILabel::TextAlign::Center, UILabel::VAlign::Middle));

    // ── 게임 저장 ─────────────────────────────
    auto makBtn = [&](int y, std::string label, std::function<void()> cb)
    {
        auto btn = std::make_unique<UIButton>(
            PL::BtnX, y, PL::BtnW, PL::BtnH, PL::Z, label, cb);
        uiManager.Add(std::move(btn));
    };

    makBtn(PL::Btn0Y, L("ui.save_game"), [this]()
    {
        context.sound.PlaySE("Assets/audio/ui_button_click.wav");
        if (context.activeSlot > 0)
        {
            context.SaveSlot(context.activeSlot);
            context.AddLog(L("system.save.completed"));
        }
        context.PopState();
    });

    // ── 설정 ──────────────────────────────────
    makBtn(PL::Btn1Y, L("ui.setting"), [this]()
    {
        context.sound.PlaySE("Assets/audio/ui_button_click.wav");
        context.PushState(std::make_unique<SettingState>(context));
    });

    // ── 로비로 돌아가기 ───────────────────────
    makBtn(PL::Btn2Y, L("ui.return_lobby"), [this]()
    {
        context.sound.PlaySE("Assets/audio/ui_button_click.wav");
        context.sound.StopBGM();
        context.ResetGameState();
        context.ChangeState(std::make_unique<TitleState>(context));
    });

    // ── 게임 종료 ─────────────────────────────
    makBtn(PL::Btn3Y, L("ui.quit_game"), [this]()
    {
        context.sound.PlaySE("Assets/audio/ui_button_click.wav");
        std::exit(0);
    });

    // ── 닫기 ──────────────────────────────────
    makBtn(PL::ClosY, L("ui.close"), [this]()
    {
        context.sound.PlaySE("Assets/audio/ui_button_click.wav");
        context.PopState();
    });
}

void PauseMenuState::HandleInput(InputManager& input)
{
    while (input.HasAction())
    {
        auto action = input.PopAction();
        if (action == InputAction::Cancel)
        {
            context.sound.PlaySE("Assets/audio/ui_button_click.wav");
            context.PopState();
            return;
        }
    }
}

void PauseMenuState::Update()
{
    uiManager.Update(60.f);
}

void PauseMenuState::Render(ConsoleDisplay& display)
{
    uiManager.Render(display);
}
