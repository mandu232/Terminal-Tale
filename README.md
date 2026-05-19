# Terminal Tale 

> A team-developed text-based RPG built in C++ using the console interface.

콘솔(프롬프트) 창 환경에서 플레이 가능한 텍스트 기반 RPG 게임입니다.
JSON 데이터로 정의된 분기형 스토리를 플레이어가 선택지를 통해 탐색하며,
플레이어의 능력치와 플래그에 따라 선택지 가용 여부가 결정됩니다.

---
## 언어

 - [English](README.en.md)
 - [한국어](README.md)

---

## 목차

- [개발 환경](#개발-환경)
- [주요 기능](#주요-기능)
- [아키텍처](#아키텍처)
- [프로젝트 구조](#프로젝트-구조)
- [스토리 데이터 형식](#스토리-데이터-형식)
- [아이템 데이터 형식](#아이템-데이터-형식)
- [플레이어 능력치](#플레이어-능력치)
- [설정 파일](#설정-파일)
- [언어파일 지원](#언어파일-지원)
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
- **능력치 & 플래그 시스템** — 체력·명성·재화 등 개인 수치, 도시 질서·시민 신뢰·오염도 등 세계 수치, 공감·냉정 등 5종 성향으로 스토리 조건을 판별합니다.
- **이펙트 시스템** — 선택지 또는 노드 진입 시 능력치 증감, 플래그 추가/제거, 성향·도시 수치 변화 효과를 적용합니다.
- **조건부 선택지** — `require` 조건을 만족하지 못하는 선택지는 표시되지 않습니다.
- **저장 / 불러오기** — 슬롯 3개 기반 세이브 시스템. `Data/saves/`에 JSON으로 전체 게임 상태를 저장합니다.
- **인벤토리 & 아이템** — JSON으로 아이템을 정의하고 인벤토리에서 사용 및 효과를 적용합니다.
- **처리 문서 (Journal)** — 케이스 처리 기록을 저널에 저장하고 게임 내에서 열람할 수 있습니다.
- **활동 로그 (Log)** — 게임 내 주요 이벤트를 날짜·시각과 함께 기록합니다.
- **타이프라이터 효과** — `UITypewriter`를 통해 텍스트가 한 글자씩 출력됩니다.
- **콘솔 UI** — UIButton, UILabel, UIImage, UITypewriter로 구성된 커스텀 콘솔 UI 시스템.
- **사운드** — miniaudio 기반의 BGM 및 효과음 재생.
- **설정 저장/불러오기** — `Data/settings.json`을 통해 게임 설정이 영속됩니다.
- **다국어 지원** — 한국어, 영어, 일본어, 중국어, 프랑스어 (JSON 기반 로컬라이제이션).
- **FPS 제한** — `targetFPS` 설정에 따라 게임 루프 속도를 제어합니다.

---

## 아키텍처

```
Application
└── GameLoop
    ├── StateMachine (스택 기반)
    │   ├── TitleState       — 타이틀 화면
    │   ├── StoryState       — 스토리 진행 화면
    │   ├── SlotSelectState  — 저장 슬롯 선택
    │   ├── LoadSlotState    — 게임 불러오기
    │   ├── PauseMenuState   — 일시정지 메뉴
    │   ├── InventoryState   — 인벤토리
    │   ├── JournalState     — 처리 문서 열람
    │   ├── LogState         — 활동 로그 열람
    │   ├── SettingState     — 설정 화면
    │   ├── SleepState       — 수면 / 시간 경과
    │   └── WaitState        — 대기 행동
    ├── InputManager
    │   └── ConsoleInputSource
    └── Context (공유 상태)
        ├── ConsoleDisplay (192×54 콘솔 버퍼)
        ├── EventBus (Pub/Sub 이벤트 버스)
        ├── SettingsManager
        ├── SoundSystem (miniaudio)
        ├── LocalizationManager
        ├── PlayerStats + flags
        ├── std::vector<LogEntry>
        └── std::vector<JournalEntry>
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
│   ├── audio/              # BGM / 효과음 (.wav)
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
│   ├── saves/              # 세이브 슬롯 (slot_1~3 .json)
│   ├── items.json          # 아이템 정의
│   └── settings.json       # 사용자 설정
├── external/
│   ├── json/json.hpp       # nlohmann/json (헤더 온리)
│   └── sound/miniaudio.h   # miniaudio (헤더 온리)
├── Game/
│   ├── Effect/             # 이펙트 타입 정의 및 적용
│   ├── Events/             # 게임 이벤트 (GameStartEvent, PlaySoundEvent)
│   ├── Item/               # 아이템 정의 및 파싱
│   ├── Journal/            # JournalEntry 구조체
│   ├── Log/                # LogEntry 구조체
│   ├── Player/             # PlayerStats 구조체
│   ├── States/             # 게임 상태 (TitleState, StoryState 등 11종)
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

### 필드 설명

| 필드 | 타입 | 설명 |
|---|---|---|
| `id` | string | 노드 고유 식별자 (파일명과 일치) |
| `sfx` | string | 노드 진입 시 재생할 효과음 경로 (선택) |
| `bgm` | string | 노드 진입 시 변경할 BGM 경로 (선택, 생략 시 유지) |
| `bgImage` | string | 좌측 패널에 표시할 아스키 아트 경로 (선택) |
| `text` | string[] | 순서대로 출력될 내러티브 텍스트 로컬라이제이션 키 배열 |
| `choices` | Choice[] | 플레이어 선택지 목록 |
| `effects` | Effect[] | 노드 진입 시 즉시 적용되는 이펙트 (선택) |
| `require` | Condition[] | 노드 진입 조건 (선택) |

### 이펙트 타입 (Effect)

| `type` | 추가 필드 | 설명 |
|---|---|---|
| `vitality` | `value` | 체력 증감 |
| `reputation` | `value` | 명성 증감 |
| `wealth` | `value` | 재화 증감 |
| `day` | `value` | 날짜 증가 |
| `time` | `value` | 시간 증가 (상대값) |
| `set_time` | `value` | 시간 설정 (절댓값) |
| `city_order` | `value` | 도시 질서 증감 |
| `citizen_trust` | `value` | 시민 신뢰 증감 |
| `corruption` | `value` | 오염도 증감 |
| `tendency` | `key`, `value` | 성향 수치 증감 (`key`: `empathy` / `coldness` / `justice` / `compliance` / `suspicion`) |
| `give_item` | `key`, `value` | 아이템 지급 (`key`: 아이템 id, `value`: 수량) |
| `remove_item` | `key`, `value` | 아이템 제거 |
| `flag_add` | `key` | 플래그 추가 |
| `flag_remove` | `key` | 플래그 제거 |
| `case_record` | `key`, `title`, `outcome`, `content` | 처리 문서에 케이스 기록 추가 (모두 로컬라이제이션 키) |

### 조건 타입 (Condition)

| `type` | `op` 필요 | `key` 필요 | 설명 |
|---|---|---|---|
| `vitality` | O | - | 체력 비교 |
| `reputation` | O | - | 명성 비교 |
| `wealth` | O | - | 재화 비교 |
| `day` | O | - | 날짜 비교 |
| `time` | O | - | 시각 비교 |
| `city_order` | O | - | 도시 질서 비교 |
| `citizen_trust` | O | - | 시민 신뢰 비교 |
| `corruption` | O | - | 오염도 비교 |
| `tendency` | O | O | 성향 수치 비교 (`key`: 성향 이름) |
| `has_item` | O | O | 인벤토리 내 아이템 수량 비교 (`key`: 아이템 id) |
| `flag` | X | O | 플래그 보유 여부 확인 |

### 조건 연산자 (ConditionOp)

| `op` | 의미 |
|---|---|
| `gt` | 초과 (>) |
| `lt` | 미만 (<) |
| `eq` | 동일 (==) |
| `gte` | 이상 (>=) |
| `lte` | 이하 (<=) |

`flag` 조건은 `op` 없이 `{ "type": "flag", "key": "flag_name" }` 형식으로 사용합니다.

### 키 네이밍 규칙

| 종류 | 패턴 | 예시 |
|---|---|---|
| 본문 텍스트 | `story.{NodeId}.{num}` | `story.case_1042.0` |
| 선택지 | `story.{NodeId}.choice.{num}` | `story.case_1042.choice.0` |

---

## 아이템 데이터 형식

### 기본 구조

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

### 필드 설명

| 필드 | 타입 | 설명 |
|---|---|---|
| `id` | string | 아이템 고유 식별자 (`give_item` / `has_item` 등에서 사용) |
| `name` | string | 인게임 표시 이름 (로컬라이제이션 키) |
| `desc` | string | 아이템 설명 (로컬라이제이션 키) |
| `usable` | bool | 인벤토리에서 사용 가능 여부 |
| `effects` | Effect[] | 사용 시 즉시 적용되는 이펙트 (선택) |

### 이펙트 타입 (Effect)

스토리 이펙트와 동일한 타입을 사용합니다. 주로 쓰이는 타입:

| `type` | 설명 |
|---|---|
| `vitality` | 체력 증감 |
| `time` | 시간 경과 (상대값) |
| `reputation` | 명성 증감 |
| `wealth` | 재화 증감 |

### 키 네이밍 규칙

| 종류 | 패턴 | 예시 |
|---|---|---|
| 이름 | `item.{ItemId}.name` | `item.coffee.name` |
| 설명 | `item.{ItemId}.desc` | `item.coffee.desc` |

---

## 플레이어 능력치

### 개인 수치

| 능력치 | 기본값 | 설명 |
|---|---|---|
| `vitality` | 10 | 체력 / 피로도 |
| `reputation` | 0 | 명성 |
| `wealth` | 0 | 재화 (돈) |

### 세계 수치 (도시 현황)

| 능력치 | 기본값 | 설명 |
|---|---|---|
| `cityOrder` | 50 | 도시 질서 |
| `citizenTrust` | 50 | 시민 신뢰 |
| `corruption` | 0 | 오염도 (기록 조작 누적) |

### 시간

| 능력치 | 기본값 | 설명 |
|---|---|---|
| `day` | 1 | 현재 날짜 |
| `time` | 8 | 현재 시각 (0~23) |

### 성향 (누적 증가, 감소 없음)

| 능력치 | 기본값 | 설명 |
|---|---|---|
| `empathy` | 0 | 공감 |
| `coldness` | 0 | 냉정 |
| `justice` | 0 | 정의 |
| `compliance` | 0 | 순응 |
| `suspicion` | 0 | 의심 |

### 기타

| 항목 | 설명 |
|---|---|
| `flags` | 스토리 플래그 문자열 집합 |
| `inventory` | 아이템 id → 수량 맵 |

---

## 설정 파일

`Data/settings.json`에서 게임 설정을 확인하고 수정할 수 있습니다.

```json
{
    "autoSave": false,
    "autoSave": true,
    "bgmVolume": 100,
    "fullScreen": true,
    "language": "ko",
    "masterVolume": 100,
    "sfxVolume": 100,
    "showFPS": false,
    "targetFPS": 15,
    "textSpeed": 1,
    "targetFPS": 30,
    "textSpeed": 3,
    "vsync": false
    "keyBindings": {
        "inventory": 73,
        "journal": 74,
        "log": 76,
        "quickLoad": 120,
        "quickSave": 116,
        "sleep": 83,
        "wait": 90
    }
}
```

| 키 | 타입 | 설명 |
|---|---|---|
| `autoSave` | bool | 자동 저장 여부 |
| `fullScreen` | bool | 전체화면 모드 |
| `language` | string | 언어 코드 (`ko` / `en` / `ja` / `zh` / `fr`) |
| `masterVolume` | int | 마스터 볼륨 (0 ~ 100) |
| `bgmVolume` | int | BGM 볼륨 (0 ~ 100) |
| `sfxVolume` | int | 효과음 볼륨 (0 ~ 100) |
| `showFPS` | bool | FPS 표시 여부 |
| `targetFPS` | int | 목표 FPS (`0` = 무제한) |
| `textSpeed` | int | 텍스트 출력 속도 |
| `vsync` | bool | 수직 동기화 여부 |
| `keyBindings` | int | 인게임 키 바인딩 |
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
| UX / UI / 번역 | 홍성우 |
| 콘텐츠 / 데이터 | 김세민 |
