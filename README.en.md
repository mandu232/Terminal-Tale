# Terminal Tale

> A team-developed text-based RPG built in C++ using the console interface.

Terminal Tale is a text-based RPG playable entirely within a console (prompt) environment.
Players explore a branching story defined in JSON data by selecting choices, while the availability of choices is dynamically determined by player stats and flags.

---
## Language

 - [English](README.en.md)
 - [한국어](README.md)

---

## Table of Contents

- [Development Environment](#development-environment)
- [Key Features](#key-features)
- [Architecture](#architecture)
- [Project Structure](#project-structure)
- [Story Data Format](#story-data-format)
- [Player Stats](#player-stats)
- [Configuration File](#configuration-file)
- [Localization Support](#localization-support)
- [External Libraries](#external-libraries)
- [Developers](#developers)

---

## Development Environment

| Item | Content |
|---|---|
| Language | C++ |
| C++ Standard | ISO C++20 Standard (/std:c++20) |
| IDE | Visual Studio 2026 |
| Platform Toolset | v154 (Microsoft C++ Build Tools) |
| Target Platform | x64 |
| Configuration Type | Application (.exe) |
| Version Control | Git & GitHub |
| Execution Environment | Windows 10 or higher (x64) |

---

## Key Features

- **Branching Text Story** — Node-based story defined in JSON. The path branches based on player choices.
- **Stats & Flag System** — Determines story conditions using 7 types of stats (Health, Reputation, Morality, Wealth, etc.) and string-based flags.
- **Effect System** — Applies stat changes or flag additions/removals when entering a node or selecting a choice.
- **Conditional Choices** — Choices that do not meet the `require` conditions are hidden from the player.
- **Typewriter Effect** — Text is rendered character-by-character via the `UITypewriter` component.
- **Console UI** — A custom console UI system consisting of UIButton, UILabel, UIImage, and UITypewriter.
- **Sound** — Sound effect playback powered by the miniaudio library.
- **Save/Load Settings** — Game configurations are persisted via `Data/settings.json`.
- **Localization** — Support for Korean, English, Japanese, Chinese, and French (JSON-based localization).
- **FPS Limiter** — Controls the game loop speed according to the `targetFPS` setting.

---

## Architecture

```
Application
└── GameLoop
    ├── StateMachine (스택 기반)
    │   ├── TitleState     — 타이틀 화면
    │   ├── StoryState     — 스토리 진행 화면
    │   ├── GameState      — 인게임 화면 (보류)
    │   └── SettingState   — 설정 화면
    ├── InputManager
    │   └── ConsoleInputSource
    └── Context (공유 상태)
        ├── ConsoleDisplay (192×54 콘솔 버퍼)
        ├── EventBus (Pub/Sub 이벤트 버스)
        ├── SettingsManager
        ├── SoundSystem (miniaudio)
        ├── LocalizationManager
        └── PlayerStats + flags
```

### 게임 루프

Executed every frame in the following order:

```
ProcessInput → Update → Render → LimitFPS
```

### State Machine (StateMachine)

Manages states using a stack structure.

| Method | Behavior |
|---|---|
| `ChangeState(state)` | Clears the stack and transitions to a new state |
| `PushState(state)` | Pushes a new state onto the stack (preserving the previous state) |
| `PopState()` | Removes the current state and resumes the previous one |

### Event Bus (EventBus)

Uses a type-based Pub/Sub pattern.

```cpp
// Subscription
auto sub = context.eventBus.Subscribe<PlaySoundEvent>(
    [this](const PlaySoundEvent& e) { /* ... */ }
);

// Emission
context.eventBus.Emit(PlaySoundEvent{"sound.wav"});
```

### Story Screen Layout (StoryState)

```
x=0 ────────── x=60 ──────────────────────── x=152 ─────────── x=192
│  Left Image  │    Center Text + Choices     │ Right Quick Menu │
```

---

## Project Structure

```
Terminal Tale/
├── Assets/
│   ├── audio/              # Sound effects (.wav)
│   └── ui/                 # Title ASCII Art
├── Core/                   # Engine Core
│   ├── Application         # App entry point, initialization
│   ├── GameLoop            # Main loop (ProcessInput/Update/Render)
│   ├── StateMachine        # Stack-based state machine
│   ├── State               # Base state class (Pure Virtual)
│   ├── Context             # Globally shared context
│   ├── EventBus            # Type-based event bus
│   ├── InputManager        # Input collection and routing
│   ├── ConsoleDisplay      # Console buffer and rendering
│   ├── SoundSystem         # miniaudio wrapper
│   ├── LocalizationManager # Localization JSON loader
│   └── SettingsManager     # Save/Load settings
├── Data/
│   ├── lang/               # Localization (ko/en/ja/zh/fr .json)
│   ├── story/              # Story node JSON files
│   └── settings.json       # User settings
├── external/
│   ├── json/json.hpp       # nlohmann/json (Header-only)
│   └── sound/miniaudio.h   # miniaudio (Header-only)
├── Game/
│   ├── Effect/             # Effect type definitions and application
│   ├── Events/             # Game events (GameStartEvent, PlaySoundEvent)
│   ├── Player/             # PlayerStats structure
│   ├── States/             # TitleState, StoryState, GameState, SettingState
│   └── Story/              # StoryNode, StoryLoader (JSON parsing)
├── Systems/
│   └── Condition / ConditionChecker   # Condition evaluation system
├── Ui/
│   ├── UIManager           # UI element container and render dispatch
│   ├── UIElement           # UI base class (Pure Virtual)
│   ├── UIButton            # Clickable buttons
│   ├── UILabel             # Text labels
│   ├── UIImage             # ASCII art images
│   └── UITypewriter        # Typewriter text effects
└── Utils/
    ├── ConsoleUtils        # Console initialization and utilities
    ├── EnableMouseInput    # Mouse input activation
    ├── GetCharWidth        # Character width calculation
    ├── GetVisualWidth      # Visual string width calculation
    └── UTF8ToWide          # UTF-8 ↔ Wide conversion
```

---

## Story Data Format

Stories are defined as JSON files in the `Data/story/` directory.
Each file represents a single StoryNode, with the `id` field serving as the node identifier.

### Basic Structure

```json
{
  "id": "forest_001",
  "bgImage": "Assets/ui/forest.txt",

  "require": [
    { "type": "vitality", "op": "gt", "value": 5 }
  ],

  "effects": [
    { "type": "vitality", "value": -1 }
  ],

  "text": [
    "story.prologue_000.0",
    "story.prologue_000.1",
    "story.prologue_000.2",
    "story.prologue_000.3",
    "story.prologue_000.4",
    "story.prologue_000.5"
  ],

  "choices": [
    {
      "text": "story.prologue_000.choice.0",
      "next": "forest_creature_001",
      "require": [
        { "type": "karma", "op": "gte", "value": 50 }
      ],
      "effects": [
        { "type": "flag_add", "key": "approached_creature" }
      ]
    },
    {
      "text": "story.prologue_000.choice.1",
      "next": "forest_002"
    }
  ]
}
```

### Field Descriptions

| 필드 | 타입 | 설명 |
|---|---|---|
| `id` | string | 노드 고유 식별자 (파일명과 일치) |
| `bgImage` | string | 좌측 패널에 표시할 아스키 아트 경로 (선택) |
| `text` | string[] | 순서대로 출력될 내러티브 텍스트 배열 |
| `choices` | Choice[] | 플레이어 선택지 목록 |
| `effects` | Effect[] | 노드 진입 시 즉시 적용되는 이펙트 (선택) |
| `require` | Condition[] | 노드 진입 조건 (선택) |

### Effect Types

| `type` | 설명 |
|---|---|
| `vitality` | 체력 증감 |
| `appearance` | 외형/신뢰도 증감 |
| `reputation` | 명성 증감 |
| `karma` | 도덕성 증감 |
| `wealth` | 재화 증감 |
| `day` | 날짜 증가 |
| `time` | 시간 증가 |
| `flag_add` | 플래그 추가 (`key` 필드 필요) |
| `flag_remove` | 플래그 제거 (`key` 필드 필요) |

### Condition Operators (ConditionOp)

| `op` | 의미 |
|---|---|
| `gt` | 초과 (>) |
| `lt` | 미만 (<) |
| `eq` | 동일 (==) |
| `gte` | 이상 (>=) |
| `lte` | 이하 (<=) |

플래그 조건은 `op` 없이 `{ "type": "flag", "key": "flag_name" }` 형식으로 사용합니다.

### Key Naming Conventions

| `type` | 패턴 | 예시 |
|---|---|---|
| 본문 텍스트 | `story.{Nodeid}.{num}` | `story.prologue_000.0` |
| 선택지 | `story.{Nodeid}.choice.{num}` | `story.prologue_000.choice.0` |

---

## Player Stats

| 능력치 | 기본값 | 설명 |
|---|---|---|
| `vitality` | 10 | 체력 / 피로도 |
| `appearance` | 100 | 외형 / 신뢰도 |
| `reputation` | 0 | 명성 |
| `karma` | 100 | 도덕성 (선악) |
| `wealth` | 0 | 재화 (돈) |
| `day` | 0 | 경과 날짜 |
| `time` | 0 | 경과 시간 |
| `flags` | (빈 집합) | 스토리 플래그 문자열 집합 |

---

## Configuration File

`Data/settings.json`에서 게임 설정을 확인하고 수정할 수 있습니다.

```json
{
    "autoSave": false,
    "fullScreen": true,
    "language": "ko",
    "masterVolume": 100,
    "showFPS": false,
    "targetFPS": 15,
    "textSpeed": 1,
    "vsync": false
}
```

| 키 | 타입 | 설명 |
|---|---|---|
| `autoSave` | bool | 자동 저장 여부 |
| `fullScreen` | bool | 전체화면 모드 |
| `language` | string | 언어 코드 (`ko` / `en` / `ja` / `zh` / `fr`) |
| `masterVolume` | int | 마스터 볼륨 (0 ~ 100) |
| `showFPS` | bool | FPS 표시 여부 |
| `targetFPS` | int | 목표 FPS (`0` = 무제한) |
| `textSpeed` | int | 텍스트 출력 속도 |
| `vsync` | bool | 수직 동기화 여부 |

---

## Localization Support

`Data/lang/` 디렉터리에 언어 코드별 JSON 파일을 추가하면 새 언어를 등록할 수 있습니다.

| 파일 | 언어 |
|---|---|
| `ko.json` | 한국어 |
| `en.json` | 영어 |
| `ja.json` | 일본어 |
| `zh.json` | 중국어 |
| `fr.json` | 프랑스어 |

코드 내에서는 `L("key")` 매크로로 현재 언어의 문자열을 가져옵니다.

```cpp
L("ui.new_game")   // → "새 게임" (ko) / "New Game" (en) / ...
```

---

## External Libraries

| 라이브러리 | 버전 | 라이선스 | 용도 |
|---|---|---|---|
| [nlohmann/json](https://github.com/nlohmann/json) | - | MIT | JSON 파싱 |
| [miniaudio](https://miniaud.io/) | - | MIT / Public Domain | 오디오 재생 |

두 라이브러리 모두 헤더 온리(header-only)이며 `external/` 디렉터리에 포함되어 있습니다.

---

## 개발자

**Team: 산 넘어 산**

| 역할 | 이름 |
|---|---|
| 팀장 / Core 시스템 | 이형원 |
| 저장 / 로드 | 노민지 |
| UX / UI | 홍성우 |
| 콘텐츠 / 데이터 | 김세민 |
