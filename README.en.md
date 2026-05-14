# Terminal Tale

> A team-developed text-based RPG built in C++ using the console interface.

A text-based RPG playable in a console (command prompt) environment.
Players navigate a branching story defined by JSON data through a series of choices,
with choice availability determined by the player's stats and flags.

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
- [Item Data Format](#item-data-format)
- [Player Stats](#player-stats)
- [Settings File](#settings-file)
- [Localization Support](#localization-support)
- [External Libraries](#external-libraries)
- [Developers](#developers)

---

## Development Environment

| Item | Details |
|---|---|
| Language | C++ |
| C++ Standard | ISO C++20 (/std:c++20) |
| Development Tool | Visual Studio 2026 |
| Platform Toolset | v154 (Microsoft C++ Build Tools) |
| Target Platform | x64 |
| Configuration Type | Application (.exe) |
| Version Control | Git & GitHub |
| Runtime Environment | Windows 10 or later (x64) |

---

## Key Features

- **Branching Text Story** — A node-based story defined in JSON. Branches into different paths based on the player's choices.
- **Stats & Flag System** — Story conditions are evaluated using 7 stats (vitality, appearance, reputation, karma, wealth, day, time) and string-based flags.
- **Effect System** — Applies stat increases/decreases and flag add/remove effects when a choice is selected or a node is entered.
- **Conditional Choices** — Choices that do not meet `require` conditions are hidden from the player.
- **Item Addition & Effects** — Items can be added via JSON and assigned effects.
- **Typewriter Effect** — Text is printed character by character via `UITypewriter`.
- **Console UI** — A custom console UI system consisting of UIButton, UILabel, UIImage, and UITypewriter.
- **Sound** — Sound effect playback powered by miniaudio.
- **Settings Save/Load** — Game settings are persisted via `Data/settings.json`.
- **Multilingual Support** — Korean, English, Japanese, Chinese, and French (JSON-based localization).
- **FPS Limiter** — Controls game loop speed according to the `targetFPS` setting.

---

## Architecture

```
Application
└── GameLoop
    ├── StateMachine (stack-based)
    │   ├── TitleState     — Title screen
    │   ├── StoryState     — Story progression screen
    │   ├── GameState      — In-game screen (pending)
    │   └── SettingState   — Settings screen
    ├── InputManager
    │   └── ConsoleInputSource
    └── Context (shared state)
        ├── ConsoleDisplay (192×54 console buffer)
        ├── EventBus (Pub/Sub event bus)
        ├── SettingsManager
        ├── SoundSystem (miniaudio)
        ├── LocalizationManager
        └── PlayerStats + flags
```

### Game Loop

Executed each frame in the following order:

```
ProcessInput → Update → Render → LimitFPS
```

### State Machine (StateMachine)

States are managed using a stack structure.

| Method | Behavior |
|---|---|
| `ChangeState(state)` | Clears the stack and transitions to a new state |
| `PushState(state)` | Pushes a new state on top of the current one (previous state is preserved) |
| `PopState()` | Removes the current state and resumes the previous one |

### Event Bus (EventBus)

Uses a type-based Pub/Sub pattern.

```cpp
// Subscribe
auto sub = context.eventBus.Subscribe<PlaySoundEvent>(
    [this](const PlaySoundEvent& e) { /* ... */ }
);

// Publish
context.eventBus.Emit(PlaySoundEvent{"sound.wav"});
```

### Story Screen Layout (StoryState)

```
x=0 ────────── x=60 ──────────────────────── x=152 ────── x=192
│  Left Image  │   Center Text + Choices     │ Right Menu  │
```

---

## Project Structure

```
Terminal Tale/
├── Assets/
│   ├── audio/              # Sound effects (.wav)
│   └── ui/                 # Title ASCII art
├── Core/                   # Engine core
│   ├── Application         # App entry point, initialization
│   ├── GameLoop            # Main loop (ProcessInput/Update/Render)
│   ├── StateMachine        # Stack-based state machine
│   ├── State               # State base class (pure virtual)
│   ├── Context             # Global shared context
│   ├── EventBus            # Type-based event bus
│   ├── InputManager        # Input collection and routing
│   ├── ConsoleDisplay      # Console buffer and rendering
│   ├── SoundSystem         # miniaudio wrapper
│   ├── LocalizationManager # Multilingual JSON loader
│   └── SettingsManager     # Settings save/load
├── Data/
│   ├── lang/               # Localization files (ko/en/ja/zh/fr .json)
│   ├── story/              # Story node JSON files
│   └── settings.json       # User settings
├── external/
│   ├── json/json.hpp       # nlohmann/json (header-only)
│   └── sound/miniaudio.h   # miniaudio (header-only)
├── Game/
│   ├── Effect/             # Effect type definitions and application
│   ├── Events/             # Game events (GameStartEvent, PlaySoundEvent)
│   ├── Player/             # PlayerStats struct
│   ├── States/             # TitleState, StoryState, GameState, SettingState
│   └── Story/              # StoryNode, StoryLoader (JSON parsing)
├── Systems/
│   └── Condition / ConditionChecker   # Condition evaluation system
├── Ui/
│   ├── UIManager           # UI element container and render dispatch
│   ├── UIElement           # UI base class (pure virtual)
│   ├── UIButton            # Clickable button
│   ├── UILabel             # Text label
│   ├── UIImage             # ASCII art image
│   └── UITypewriter        # Typewriter text effect
└── Utils/
    ├── ConsoleUtils        # Console initialization and utilities
    ├── EnableMouseInput    # Mouse input activation
    ├── GetCharWidth        # Per-character width calculation
    ├── GetVisualWidth      # Visual string width calculation
    └── UTF8ToWide          # UTF-8 ↔ Wide string conversion
```

---

## Story Data Format

Stories are defined in JSON files located in the `Data/story/` directory.
Each file represents a single **StoryNode**, with the `id` field used as the node identifier.

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

| Field | Type | Description |
|---|---|---|
| `id` | string | Unique node identifier (matches the filename) |
| `bgImage` | string | Path to ASCII art displayed in the left panel (optional) |
| `text` | string[] | Array of narrative text keys printed in order |
| `choices` | Choice[] | List of player choices |
| `effects` | Effect[] | Effects applied immediately upon entering the node (optional) |
| `require` | Condition[] | Conditions required to enter the node (optional) |

### Effect Types (Effect)

| `type` | Description |
|---|---|
| `vitality` | Increases/decreases vitality |
| `appearance` | Increases/decreases appearance/credibility |
| `reputation` | Increases/decreases reputation |
| `karma` | Increases/decreases morality |
| `wealth` | Increases/decreases wealth |
| `day` | Advances the day counter |
| `time` | Advances the time counter |
| `give_item` | Grants an item |
| `remove_item` | Removes an item |
| `flag_add` | Adds a flag (requires `key` field) |
| `flag_remove` | Removes a flag (requires `key` field) |

### Condition Operators (ConditionOp)

| `op` | Meaning |
|---|---|
| `gt` | Greater than (>) |
| `lt` | Less than (<) |
| `eq` | Equal to (==) |
| `gte` | Greater than or equal to (>=) |
| `lte` | Less than or equal to (<=) |

Flag conditions are used without `op`, in the format `{ "type": "flag", "key": "flag_name" }`.

### Key Naming Convention

| `type` | Pattern | Example |
|---|---|---|
| Body text | `story.{NodeId}.{num}` | `story.prologue_000.0` |
| Choices | `story.{NodeId}.choice.{num}` | `story.prologue_000.choice.0` |

---

## Item Data Format

### Basic Structure

```json
[
    {
        "id": "health_potion",
        "name": "item.health_potion.name",
        "desc": "item.health_potion.desc",
        "usable": true,
        "effects": [
            { "type": "vitality", "value": 2 }
        ]
    }
]
```

### Field Descriptions

| Field | Type | Description |
|---|---|---|
| `id` | string | Unique item identifier (used as the tag when granting via effects) |
| `name` | string | In-game display name of the item (parsed from the language file) |
| `desc` | string[] | Item description details (parsed from the language file) |
| `usable` | bool | Whether the item can be used by the player |
| `effects` | Effect[] | Effects applied immediately upon use (optional) |

### Effect Types (Effect)

| `type` | Description |
|---|---|
| `vitality` | Increases/decreases vitality |
| `appearance` | Increases/decreases appearance/credibility |
| `reputation` | Increases/decreases reputation |
| `karma` | Increases/decreases morality |
| `wealth` | Increases/decreases wealth |
| `day` | Advances the day counter |
| `time` | Advances the time counter |
| `give_item` | Grants an item |
| `remove_item` | Removes an item |
| `flag_add` | Adds a flag (requires `key` field) |
| `flag_remove` | Removes a flag (requires `key` field) |

### Key Naming Convention

```json
[
    {
    "item.health_potion.name": "Health Potion",
    "item.health_potion.desc": "Drink to restore 30 vitality.\nUse when fatigued or injured.",

    "item.travel_ration.name": "Travel Ration",
    "item.travel_ration.desc": "Simple hardtack and dried meat.\nRestores 10 vitality, but consumes 1 time unit.",
    }
]
```

| `type` | Pattern | Example |
|---|---|---|
| Name | `item.{ItemId}.name` | `item.health_potion.name` |
| Description | `item.{ItemId}.desc` | `item.health_potion.desc` |

---

## Player Stats

| Stat | Default | Description |
|---|---|---|
| `vitality` | 10 | Health / fatigue |
| `appearance` | 100 | Appearance / credibility |
| `reputation` | 0 | Reputation |
| `karma` | 100 | Morality (good/evil alignment) |
| `wealth` | 0 | Wealth (money) |
| `day` | 0 | Days elapsed |
| `time` | 0 | Time elapsed |
| `flags` | (empty set) | Set of story flag strings |

---

## Settings File

Game settings can be viewed and modified in `Data/settings.json`.

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

| Key | Type | Description |
|---|---|---|
| `autoSave` | bool | Enable auto-save |
| `fullScreen` | bool | Fullscreen mode |
| `language` | string | Language code (`ko` / `en` / `ja` / `zh` / `fr`) |
| `masterVolume` | int | Master volume (0 ~ 100) |
| `showFPS` | bool | Show FPS counter |
| `targetFPS` | int | Target FPS (`0` = unlimited) |
| `textSpeed` | int | Text output speed |
| `vsync` | bool | Vertical sync |

---

## Localization Support

New languages can be registered by adding a JSON file named with the language code to the `Data/lang/` directory.

| File | Language |
|---|---|
| `ko.json` | Korean |
| `en.json` | English |
| `ja.json` | Japanese |
| `zh.json` | Chinese |
| `fr.json` | French |

In code, strings for the current language are retrieved using the `L("key")` macro.

```cpp
L("ui.new_game")   // → "새 게임" (ko) / "New Game" (en) / ...
```

---

## External Libraries

| Library | Version | License | Purpose |
|---|---|---|---|
| [nlohmann/json](https://github.com/nlohmann/json) | - | MIT | JSON parsing |
| [miniaudio](https://miniaud.io/) | - | MIT / Public Domain | Audio playback |

Both libraries are header-only and included in the `external/` directory.

---

## Developers

**Team: 산 넘어 산 (Mountain After Mountain)**

| Role | Name |
|---|---|
| Team Lead / Core Systems | Lee Hyeong-won |
| Save / Load | No Min-ji |
| UX / UI | Hong Seong-u |
| Content / Data | Kim Se-min |
