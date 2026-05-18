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
- **Stats & Flag System** — Story conditions are evaluated using personal stats (vitality, reputation, wealth), world stats (city order, citizen trust, corruption), five tendency traits, and string-based flags.
- **Effect System** — Applies stat changes, flag add/remove, tendency shifts, and world stat changes when a choice is selected or a node is entered.
- **Conditional Choices** — Choices that do not meet `require` conditions are hidden from the player.
- **Save / Load** — 3-slot save system. The full game state is persisted to JSON files in `Data/saves/`.
- **Inventory & Items** — Items are defined in JSON. Players can carry and use items from the inventory screen.
- **Case Journal** — Case handling records are saved to a journal and can be reviewed in-game.
- **Activity Log** — Key in-game events are recorded with day and time stamps.
- **Typewriter Effect** — Text is printed character by character via `UITypewriter`.
- **Console UI** — A custom console UI system consisting of UIButton, UILabel, UIImage, and UITypewriter.
- **Sound** — BGM and sound effect playback powered by miniaudio.
- **Settings Save/Load** — Game settings are persisted via `Data/settings.json`.
- **Multilingual Support** — Korean, English, Japanese, Chinese, and French (JSON-based localization).
- **FPS Limiter** — Controls game loop speed according to the `targetFPS` setting.

---

## Architecture

```
Application
└── GameLoop
    ├── StateMachine (stack-based)
    │   ├── TitleState       — Title screen
    │   ├── StoryState       — Story progression screen
    │   ├── SlotSelectState  — Save slot selection
    │   ├── LoadSlotState    — Load game screen
    │   ├── PauseMenuState   — Pause menu
    │   ├── InventoryState   — Inventory screen
    │   ├── JournalState     — Case journal viewer
    │   ├── LogState         — Activity log viewer
    │   ├── SettingState     — Settings screen
    │   ├── SleepState       — Sleep / time progression
    │   └── WaitState        — Wait action
    ├── InputManager
    │   └── ConsoleInputSource
    └── Context (shared state)
        ├── ConsoleDisplay (192×54 console buffer)
        ├── EventBus (Pub/Sub event bus)
        ├── SettingsManager
        ├── SoundSystem (miniaudio)
        ├── LocalizationManager
        ├── PlayerStats + flags
        ├── std::vector<LogEntry>
        └── std::vector<JournalEntry>
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
│   ├── audio/              # BGM / sound effects (.wav)
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
│   ├── saves/              # Save slots (slot_1~3 .json)
│   ├── items.json          # Item definitions
│   └── settings.json       # User settings
├── external/
│   ├── json/json.hpp       # nlohmann/json (header-only)
│   └── sound/miniaudio.h   # miniaudio (header-only)
├── Game/
│   ├── Effect/             # Effect type definitions and application
│   ├── Events/             # Game events (GameStartEvent, PlaySoundEvent)
│   ├── Item/               # Item definitions and parsing
│   ├── Journal/            # JournalEntry struct
│   ├── Log/                # LogEntry struct
│   ├── Player/             # PlayerStats struct
│   ├── States/             # Game states (TitleState, StoryState, etc. — 11 total)
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
  "id": "case_1042",
  "sfx": "Assets/audio/node_paper.wav",
  "bgm": "Assets/audio/bgm_office.wav",
  "bgImage": "Assets/ui/office.txt",

  "require": [
    { "type": "vitality", "op": "gt", "value": 0 }
  ],

  "effects": [
    { "type": "city_order", "value": 5 },
    { "type": "citizen_trust", "value": -3 },
    { "type": "tendency", "key": "justice", "value": 1 },
    {
      "type": "case_record",
      "key": "case_1042",
      "title": "story.case_1042.0",
      "outcome": "story.case_1042.choice.0",
      "content": "story.case_1042_a.0"
    }
  ],

  "text": [
    "story.case_1042.0",
    "story.case_1042.1",
    "story.case_1042.2"
  ],

  "choices": [
    {
      "text": "story.case_1042.choice.0",
      "next": "case_1042_a",
      "require": [
        { "type": "tendency", "key": "justice", "op": "gte", "value": 3 }
      ],
      "effects": [
        { "type": "flag_add", "key": "reported_case_1042" }
      ]
    },
    {
      "text": "story.case_1042.choice.1",
      "next": "case_1042_b",
      "require": [
        { "type": "has_item", "key": "access_card", "op": "gte", "value": 1 }
      ]
    },
    {
      "text": "story.case_1042.choice.2",
      "next": "case_1042_c"
    }
  ]
}
```

### Field Descriptions

| Field | Type | Description |
|---|---|---|
| `id` | string | Unique node identifier (matches the filename) |
| `sfx` | string | Sound effect path played on node entry (optional) |
| `bgm` | string | BGM path to switch to on node entry (optional; omit to keep current) |
| `bgImage` | string | Path to ASCII art displayed in the left panel (optional) |
| `text` | string[] | Array of narrative text localization keys printed in order |
| `choices` | Choice[] | List of player choices |
| `effects` | Effect[] | Effects applied immediately upon entering the node (optional) |
| `require` | Condition[] | Conditions required to enter the node (optional) |

### Effect Types (Effect)

| `type` | Extra Fields | Description |
|---|---|---|
| `vitality` | `value` | Increases/decreases vitality |
| `reputation` | `value` | Increases/decreases reputation |
| `wealth` | `value` | Increases/decreases wealth |
| `day` | `value` | Advances the day counter |
| `time` | `value` | Advances time (relative) |
| `set_time` | `value` | Sets time to an absolute value |
| `city_order` | `value` | Increases/decreases city order |
| `citizen_trust` | `value` | Increases/decreases citizen trust |
| `corruption` | `value` | Increases/decreases corruption |
| `tendency` | `key`, `value` | Changes a tendency stat (`key`: `empathy` / `coldness` / `justice` / `compliance` / `suspicion`) |
| `give_item` | `key`, `value` | Grants an item (`key`: item id, `value`: quantity) |
| `remove_item` | `key`, `value` | Removes an item |
| `flag_add` | `key` | Adds a flag |
| `flag_remove` | `key` | Removes a flag |
| `case_record` | `key`, `title`, `outcome`, `content` | Adds a case entry to the journal (all localization keys) |

### Condition Types (Condition)

| `type` | Needs `op` | Needs `key` | Description |
|---|---|---|---|
| `vitality` | O | - | Compare vitality |
| `reputation` | O | - | Compare reputation |
| `wealth` | O | - | Compare wealth |
| `day` | O | - | Compare current day |
| `time` | O | - | Compare current time |
| `city_order` | O | - | Compare city order |
| `citizen_trust` | O | - | Compare citizen trust |
| `corruption` | O | - | Compare corruption |
| `tendency` | O | O | Compare a tendency stat (`key`: tendency name) |
| `has_item` | O | O | Compare item quantity in inventory (`key`: item id) |
| `flag` | X | O | Check whether a flag is set |

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

| Type | Pattern | Example |
|---|---|---|
| Body text | `story.{NodeId}.{num}` | `story.case_1042.0` |
| Choices | `story.{NodeId}.choice.{num}` | `story.case_1042.choice.0` |

---

## Item Data Format

### Basic Structure

```json
[
    {
        "id": "coffee",
        "name": "item.coffee.name",
        "desc": "item.coffee.desc",
        "usable": true,
        "effects": [
            { "type": "vitality", "value": 10 }
        ]
    },
    {
        "id": "access_card",
        "name": "item.access_card.name",
        "desc": "item.access_card.desc",
        "usable": false,
        "effects": []
    }
]
```

### Field Descriptions

| Field | Type | Description |
|---|---|---|
| `id` | string | Unique item identifier (used in `give_item` / `has_item` effects) |
| `name` | string | In-game display name (localization key) |
| `desc` | string | Item description (localization key) |
| `usable` | bool | Whether the item can be used from the inventory |
| `effects` | Effect[] | Effects applied immediately upon use (optional) |

### Effect Types (Effect)

Uses the same effect types as story nodes. Commonly used types:

| `type` | Description |
|---|---|
| `vitality` | Increases/decreases vitality |
| `time` | Advances time (relative) |
| `reputation` | Increases/decreases reputation |
| `wealth` | Increases/decreases wealth |

### Key Naming Convention

| Type | Pattern | Example |
|---|---|---|
| Name | `item.{ItemId}.name` | `item.coffee.name` |
| Description | `item.{ItemId}.desc` | `item.coffee.desc` |

---

## Player Stats

### Personal Stats

| Stat | Default | Description |
|---|---|---|
| `vitality` | 10 | Health / fatigue |
| `reputation` | 0 | Reputation |
| `wealth` | 0 | Wealth (money) |

### World Stats (City Conditions)

| Stat | Default | Description |
|---|---|---|
| `cityOrder` | 50 | City order |
| `citizenTrust` | 50 | Citizen trust |
| `corruption` | 0 | Corruption (accumulated from record manipulation) |

### Time

| Stat | Default | Description |
|---|---|---|
| `day` | 1 | Current day |
| `time` | 8 | Current time (0–23) |

### Tendencies (accumulate only — never decrease)

| Stat | Default | Description |
|---|---|---|
| `empathy` | 0 | Empathy |
| `coldness` | 0 | Coldness |
| `justice` | 0 | Justice |
| `compliance` | 0 | Compliance |
| `suspicion` | 0 | Suspicion |

### Other

| Item | Description |
|---|---|
| `flags` | Set of story flag strings |
| `inventory` | Map of item id → quantity |

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
