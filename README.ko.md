# Terminal Tale

> A team-developed text-based RPG built in C++ using the console interface.

콘솔(프롬프트) 창 환경에서 플레이 가능한 텍스트 기반 RPG 게임입니다.
JSON 데이터로 정의된 분기형 스토리를 플레이어가 선택지를 통해 탐색하며,
플레이어의 능력치와 플래그에 따라 선택지 가용 여부가 결정됩니다.

---
## 언어

 - [English](README.md)
 - [한국어](README.ko.md)

---

## 목차

- [개발 환경](#개발-환경)
- [주요 기능](#주요-기능)
- [아키텍처](#아키텍처)
- [프로젝트 구조](#프로젝트-구조)
- [스토리 데이터 형식](#스토리-데이터-형식)
- [플레이어 능력치](#플레이어-능력치)
- [설정 파일](#설정-파일)
- [다국어 지원](#언어파일-지원)
- [외부 라이브러리](#외부-라이브러리)
- [개발자](#개발자)

---

## 개발 환경

| 항목 | 내용 |
|---|---|
| 언어 | C++ |
| C++ 언어 표준 | ISO C++20 표준(/std:c++20) |
| 개발 도구 | Visual Studio 2026 |
| 플랫폼 도구 집합 | v154 (Microsoft C++ Build Tools) |
| 대상 플랫폼 | x64 |
| 구성 형식 | 응용 프로그램 (.exe) |
| 버전 관리 | Git & GitHub |
| 실행 환경 | Windows 10 이상 (x64) |

---

## 주요 기능

- **분기형 텍스트 스토리** — JSON으로 정의된 노드 기반 스토리. 플레이어의 선택에 따라 다른 경로로 분기됩니다.
- **능력치 & 플래그 시스템** — 체력, 명성, 도덕성, 재화 등 7가지 능력치와 문자열 플래그로 스토리 조건을 판별합니다.
- **이펙트 시스템** — 선택지 또는 노드 진입 시 능력치 증감, 플래그 추가/제거 효과를 적용합니다.
- **조건부 선택지** — `require` 조건을 만족하지 못하는 선택지는 표시되지 않습니다.
- **타이프라이터 효과** — `UITypewriter`를 통해 텍스트가 한 글자씩 출력됩니다.
- **콘솔 UI** — UIButton, UILabel, UIImage, UITypewriter로 구성된 커스텀 콘솔 UI 시스템.
- **사운드** — miniaudio 기반의 효과음 재생.
- **설정 저장/불러오기** — `Data/settings.json`을 통해 게임 설정이 영속됩니다.
- **다국어 지원** — 한국어, 영어, 일본어, 중국어, 프랑스어 (JSON 기반 로컬라이제이션).
- **FPS 제한** — `targetFPS` 설정에 따라 게임 루프 속도를 제어합니다.

---

## 아키텍처

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

매 프레임마다 다음 순서로 실행됩니다:

```
ProcessInput → Update → Render → LimitFPS
```

### 상태 머신 (StateMachine)

스택(stack) 구조로 상태를 관리합니다.

| 메서드 | 동작 |
|---|---|
| `ChangeState(state)` | 스택을 비우고 새 상태로 전환 |
| `PushState(state)` | 현재 상태 위에 새 상태를 올림 (이전 상태 유지) |
| `PopState()` | 현재 상태를 제거하고 이전 상태 재개 |

### 이벤트 버스 (EventBus)

타입 기반 Pub/Sub 패턴을 사용합니다.

```cpp
// 구독
auto sub = context.eventBus.Subscribe<PlaySoundEvent>(
    [this](const PlaySoundEvent& e) { /* ... */ }
);

// 발행
context.eventBus.Emit(PlaySoundEvent{"sound.wav"});
```

### 스토리 화면 레이아웃 (StoryState)

```
x=0 ────────── x=60 ──────────────────────── x=152 ────── x=192
│  좌측 이미지  │     중앙 텍스트 + 선택지    │ 우측 퀵메뉴 │
```

---

## 프로젝트 구조

```
Terminal Tale/
├── Assets/
│   ├── audio/              # 효과음 (.wav)
│   └── ui/                 # 타이틀 아스키 아트
├── Core/                   # 엔진 코어
│   ├── Application         # 앱 진입점, 초기화
│   ├── GameLoop            # 메인 루프 (ProcessInput/Update/Render)
│   ├── StateMachine        # 스택 기반 상태 머신
│   ├── State               # 상태 기반 클래스 (순수 가상)
│   ├── Context             # 전역 공유 컨텍스트
│   ├── EventBus            # 타입 기반 이벤트 버스
│   ├── InputManager        # 입력 수집 및 라우팅
│   ├── ConsoleDisplay      # 콘솔 버퍼 및 렌더링
│   ├── SoundSystem         # miniaudio 래퍼
│   ├── LocalizationManager # 다국어 JSON 로더
│   └── SettingsManager     # 설정 저장/불러오기
├── Data/
│   ├── lang/               # 로컬라이제이션 (ko/en/ja/zh/fr .json)
│   ├── story/              # 스토리 노드 JSON
│   └── settings.json       # 사용자 설정
├── external/
│   ├── json/json.hpp       # nlohmann/json (헤더 온리)
│   └── sound/miniaudio.h   # miniaudio (헤더 온리)
├── Game/
│   ├── Effect/             # 이펙트 타입 정의 및 적용
│   ├── Events/             # 게임 이벤트 (GameStartEvent, PlaySoundEvent)
│   ├── Player/             # PlayerStats 구조체
│   ├── States/             # TitleState, StoryState, GameState, SettingState
│   └── Story/              # StoryNode, StoryLoader (JSON 파싱)
├── Systems/
│   └── Condition / ConditionChecker   # 조건 판별 시스템
├── Ui/
│   ├── UIManager           # UI 요소 컨테이너 및 렌더 디스패치
│   ├── UIElement           # UI 기반 클래스 (순수 가상)
│   ├── UIButton            # 클릭 가능한 버튼
│   ├── UILabel             # 텍스트 라벨
│   ├── UIImage             # 아스키 아트 이미지
│   └── UITypewriter        # 타이프라이터 텍스트 효과
└── Utils/
    ├── ConsoleUtils        # 콘솔 초기화 및 유틸
    ├── EnableMouseInput    # 마우스 입력 활성화
    ├── GetCharWidth        # 문자 단위 폭 계산
    ├── GetVisualWidth      # 시각적 문자열 폭 계산
    └── UTF8ToWide          # UTF-8 ↔ Wide 변환
```

---

## 스토리 데이터 형식

스토리는 `Data/story/` 디렉터리의 JSON 파일로 정의됩니다.
각 파일은 하나의 **StoryNode**를 나타내며, `id` 필드가 노드 식별자로 사용됩니다.

### 기본 구조

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

### 필드 설명

| 필드 | 타입 | 설명 |
|---|---|---|
| `id` | string | 노드 고유 식별자 (파일명과 일치) |
| `bgImage` | string | 좌측 패널에 표시할 아스키 아트 경로 (선택) |
| `text` | string[] | 순서대로 출력될 내러티브 텍스트 배열 |
| `choices` | Choice[] | 플레이어 선택지 목록 |
| `effects` | Effect[] | 노드 진입 시 즉시 적용되는 이펙트 (선택) |
| `require` | Condition[] | 노드 진입 조건 (선택) |

### 이펙트 타입 (Effect)

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

### 조건 연산자 (ConditionOp)

| `op` | 의미 |
|---|---|
| `gt` | 초과 (>) |
| `lt` | 미만 (<) |
| `eq` | 동일 (==) |
| `gte` | 이상 (>=) |
| `lte` | 이하 (<=) |

플래그 조건은 `op` 없이 `{ "type": "flag", "key": "flag_name" }` 형식으로 사용합니다.

### 키 네이밍 규칙

| `type` | 패턴 | 예시 |
|---|---|---|
| 본문 텍스트 | `story.{Nodeid}.{num}` | `story.prologue_000.0` |
| 선택지 | `story.{Nodeid}.choice.{num}` | `story.prologue_000.choice.0` |

---

## 플레이어 능력치

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

## 설정 파일

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

## 언어파일 지원

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

## 외부 라이브러리

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
