# Application

## 개요

- Framework 위에서 동작하는 간단한 2D 게임.
- 플레이어는 화면 하단 고정 레인에서 좌우로 이동하며 위에서 떨어지는 적 블록을 피함.
- 생존 시간이 길어질수록 레벨이 오르고, 플레이어 속도와 적 스폰 속도가 함께 증가.
- 적 블록에 닿으면 즉시 게임 오버이며, 최고 기록(생존 시간)을 경신하는 방식으로 반복 플레이.
- **조작**: `SPACE` — 이동 방향 전환 (좌↔우) / `ESC` — 게임 종료

## 목적

- Framework의 액터 시스템, 씬 전환, 충돌 감지, 데이터 파이프라인, 이벤트 시스템을 실제 게임에서 검증
- Framework와 Application 레이어 분리 원칙을 실제로 적용 → 게임 로직이 엔진 코드 수정 없이 구현될 수 있음을 검증

## 목표

| 목표 | 설명 |
|------|------|
| **단일 조작** | `SPACE` 하나로 플레이 가능한 단순한 입력 구조 |
| **데이터 주도 밸런싱** | 레벨별 속도·스폰 간격이 코드가 아닌 XLSX 데이터로 관리됨 |
| **오브젝트 재사용** | 적 액터를 풀(Pool) 방식으로 관리하여 동적 할당 최소화 |
| **이벤트 기반 UI** | UI 액터가 게임 로직 액터를 직접 참조하지 않고 이벤트로 반응 |
| **무한 재시작** | 게임 오버 후 씬 재진입으로 완전 초기화, 루프 플레이 지원 |

## 디렉토리 구조

```
Application/
└── Source/
    ├── Inc/
    │   ├── Actor/
    │   │   ├── Enemy/              # 적 액터 (EnemyActor, EnemySpawnActor)
    │   │   ├── Player/             # 플레이어 액터
    │   │   ├── Physics/            # 이동 경계 모델 (MoveBoundModel)
    │   │   └── UI/                 # UI 액터 (GameOver, GameResult, PlayTime, EffectText, Title)
    │   ├── App/                    # App (IApp 구현체), AppDef
    │   ├── Config/                 # GameConfig
    │   ├── Context/                # PlayerContext
    │   └── Scene/                  # TitleScene, GameScene
    └── Src/
        ├── Actor/
        │   ├── Enemy/
        │   ├── Player/
        │   └── UI/
        ├── App/                    # App.cpp, Main.cpp
        ├── Config/
        ├── Context/
        └── Scene/
```

## 씬 구성 및 전환 흐름

```
[앱 시작]
    │
    ▼
┌─────────────┐   SPACE   ┌─────────────┐
│  TitleScene │ ────────► │  GameScene  │
└─────────────┘           └─────────────┘
                               │     ▲
                    충돌 → 게임오버   │ SPACE (재시작)
                               │     │
                          ┌────▼─────┐
                          │ GameOver │
                          │   UI 표시 │
                          └──────────┘
```

- **TitleScene → GameScene**: `TitleActorController`에서 SPACE 입력 감지 → `Transition<GameScene>()`
- **게임 오버 후 재시작**: `GameOverActorController`에서 2초 대기 + SPACE 입력 감지 → `Transition<GameScene>()` → `OnExit()`/`OnEnter()` 재실행으로 완전 초기화


## 액터 목록

### PlayerActor

| 항목 | 내용 |
|------|------|
| 모델 | `PlayerModel` (원형, `ICircleModel` + `CollidableModel`) |
| 컨트롤러 | `PlayerActorController` |
| 역할 | 좌우 이동, 레벨업 모니터링, 충돌 시 게임 오버 처리 |

**이동 로직**:
- `SPACE` 입력 시 이동 방향(±X) 전환
- 이동 범위(`PlayerMoveRangeMinX` ~ `PlayerMoveRangeMaxX`)에서 클램프
- 경계 도달 시 자동 방향 전환
- Y 좌표는 `PlayerMoveRangeY`에 고정 (수평 레인)

**레벨업 처리**:
- `PlayerContext.AddPlayTime()` 호출 → 레벨업 조건 충족 시 `LevelUpEvent` 발생
- `LevelUpEvent` 수신 → `PlayerDataChunk`에서 새 속도 갱신 + "LEVEL UP!" 이펙트 표시

**충돌 처리**:
- `EnemyActor`와 충돌 시 플레이어 숨김 → `PlayerContext.SetGameOver(true)` → 파티클 이펙트 생성

---

### EnemyActor / EnemySpawnActor

| 항목 | 내용 |
|------|------|
| 모델 | `EnemyModel` (직사각형, `IRectModel` + `CollidableModel`) |
| 컨트롤러 | `EnemyActorController` |
| 상태 | `NONE` → `MOVE` → `FADE_OUT` → `DEAD` |

**EnemyActor 상태 전이**:

```
[NONE] ──스폰 설정──► [MOVE] ──deadZoneY 도달──► [FADE_OUT] ──알파=0──► [DEAD]
```

- `MOVE`: 매 프레임 아래 방향(`+Y`)으로 이동하며 회전
- `FADE_OUT`: `EnemyFadeOutTime` 동안 알파값 0으로 감소
- `DEAD`: 오브젝트 풀에 반환 대기 상태

**EnemySpawnActor**:
- `_timeSinceLastSpawn` 누적 → 스폰 시간 도달 시 적 생성
- 생성 전 풀에서 `DEAD` 상태 모델 재사용 시도 → 없으면 새 `EnemyActor` 생성
- `EnemyDataChunk`에서 무작위 데이터 선택 후 적용
- `LevelUpEvent` 수신 → `PlayerDataChunk`의 레벨별 `EnemySpawnTime`으로 간격 갱신

### UI 액터

| 액터 | 역할 | 트리거 |
|------|------|--------|
| `PlayTimeActor` | 현재 플레이 시간 실시간 표시 | 항상 표시 |
| `GameOverActor` | "GAME OVER" 텍스트 표시 및 재시작 입력 처리 | `GameOverEvent` |
| `GameResultActor` | 최고 기록 시간 + 점멸 힌트 텍스트 표시 | `GameOverEvent` |
| `EffectTextActor` | "TAB!" / "LEVEL UP!" 텍스트 이펙트 | SPACE 입력, `LevelUpEvent` |
| `TitleActor` | "DODGE BALL" 타이틀 + 힌트 텍스트 표시 | TitleScene 진입 |

**EffectTextActor** 는 풀 방식으로 관리.
`PlayerActorController`가 풀을 소유하며, `DEAD` 상태인 인스턴스를 재활성화.
텍스트는 생성 위치에서 위로 떠오르며(`MoveSpeed`) `LifeTime` 경과 후 소멸.

## 데이터 구조

### PlayerDataChunk (`Resource/Player.bytes`)

레벨별 플레이어 수치와 적 스폰 설정 보유.

| 필드 | 타입 | 설명 |
|------|------|------|
| `Level` | int | 레벨 번호 |
| `Speed` | int | 플레이어 이동 속도 |
| `LevelUpTime` | int | 해당 레벨 도달에 필요한 누적 생존 시간(초) |
| `EnemySpawnTime` | float | 해당 레벨에서의 적 스폰 간격(초) |

레벨업 조건: `PlayerContext._currentPlayTime >= 다음 레벨의 LevelUpTime`

### EnemyDataChunk (`Resource/Enemy.bytes`)

적 타입별 외형과 움직임 수치 보유.

| 필드 | 타입 | 설명 |
|------|------|------|
| `Num` | int | 적 번호 |
| `MoveSpeed` | int | 하강 속도 |
| `RotationSpeed` | int | 회전 속도 |
| `Color` | float[4] | RGBA 색상 (0.0~1.0) |

스폰 시 `EnemyDataChunk`에서 무작위로 하나를 선택.

원본 데이터는 `DataPack/XLSX/` 하위 엑셀 파일에서 관리하며, 데이터 파이프라인을 통해 `.bytes`로 변환.
(파이프라인 사용법은 루트 경로의 `cli.py` 참고)

## 설정 파일 (`Config/GameConfig.yaml`)

게임 밸런스와 외형에 관련된 수치를 코드 수정 없이 YAML로 조정 가능.

### 플레이어

| 키 | 기본값 | 설명 |
|----|--------|------|
| `PlayerRadius` | 15.0 | 충돌 반경 |
| `PlayerMoveRangeMinX` | 50 | 이동 가능 최소 X |
| `PlayerMoveRangeMaxX` | 550 | 이동 가능 최대 X |
| `PlayerMoveRangeY` | 550 | 고정 Y 좌표 |
| `PlayerColor` | (1,1,1,1) | 색상 (RGBA) |
| `IsPlayerStartMovePositive` | true | 시작 방향 (+X = true) |

### 적 스폰

| 키 | 기본값 | 설명 |
|----|--------|------|
| `SpawnRangeMinX` | -10 | 스폰 최소 X |
| `SpawnRangeMaxX` | 610 | 스폰 최대 X |
| `SpawnRangeY` | -100 | 스폰 Y (화면 위) |
| `EnemySize` | 50 | 적 정사각형 한 변 길이 |
| `EnemyDeadZoneY` | 700 | 이 Y 좌표 도달 시 페이드아웃 시작 |
| `EnemyFadeOutTime` | 1.0 | 페이드아웃 지속 시간(초) |

### 파티클 (플레이어 사망 이펙트)

| 키 | 기본값 | 설명 |
|----|--------|------|
| `ParticleCount` | 400 | 생성 파티클 수 |
| `ParticleMinSize` / `MaxSize` | 5.0 / 10.0 | 파티클 크기 범위 |
| `ParticleMinSpeed` / `MaxSpeed` | 100.0 / 400.0 | 파티클 속도 범위 |
| `ParticleLifeTime` | 1.0 | 파티클 수명(초) |

### UI 텍스트

`GameConfig.yaml`에서 각 UI 텍스트의 위치, 색상, 폰트 크기, 애니메이션 수치 조정 가능.
항목: `TabText`, `LevelUpText`, `GameOverText`, `GameResultText`, `PlayTimeText`

---

## PlayerContext

게임 진행 중 공유 상태를 담는 컨텍스트. `ContextManager`에 등록되며 여러 액터/컨트롤러에서 참조.

| 필드 | 타입 | 설명 |
|------|------|------|
| `_currentPlayTime` | float | 현재 판의 생존 시간 (초) |
| `_bestPlayTime` | float | 역대 최고 생존 시간 (초, 세션 내 유지) |
| `_level` | int | 현재 레벨 (1~maxLevel) |
| `_isGameOver` | bool | 게임 오버 여부 |

| 이벤트 | 타입 | 발생 조건 |
|--------|------|----------|
| `_levelUpEvent` | `Event<int32_t>` | 레벨 증가 시 (파라미터: 새 레벨) |
| `_gameOverEvent` | `Event<>` | `SetGameOver(true)` 호출 시 |

`Reset()` 호출 시 `_currentPlayTime`, `_level`, `_isGameOver` 초기화. `_bestPlayTime`은 유지.

> 주의: 현재 버전에서 `_bestPlayTime`은 세션 메모리에만 저장. 게임 종료 시 소멸.

---

## 게임 루프에서의 실행 순서 (GameScene 기준)

```
OnPreTick   │ App — ESC 입력 감지, 앱 종료 요청 처리
            │
OnTick      │ PlayerActorController::OnTick()
            │   ├─ PlayerContext.AddPlayTime()     ← 생존 시간 누적 + 레벨업 체크
            │   ├─ UpdateMoveDirection()            ← SPACE 입력 처리
            │   └─ Move() + UpdateDirectionByBounds()
            │
            │ EnemySpawnActorController::OnTick()
            │   └─ SpawnEnemyActor() (스폰 시간 도달 시)
            │
            │ EnemyActorController::OnTick() (적마다)
            │   └─ 상태별 이동/페이드아웃/대기
            │
            │ UI Actor들::OnTick()
            │   ├─ PlayTimeActorController — 시간 텍스트 갱신
            │   ├─ EffectTextActorController — 이펙트 위치 갱신
            │   └─ GameOverActorController — 재시작 입력 감지
            │
OnPostTick  │ PhysicManager::Tick()
            │   └─ PlayerActor ↔ EnemyActor 충돌 감지 → OnCollision() 호출
            │
OnRender    │ 모든 IRenderableModel 렌더링 (order 순서대로)
```

---

## 유지보수 가이드

### 밸런스 수치 변경

- 레벨별 속도, 스폰 간격: `DataPack/XLSX/Player.xlsx` 수정 후 데이터 파이프라인 실행
- 스폰 범위, 적 크기, 파티클 수치, UI 위치: `Config/GameConfig.yaml` 직접 수정 (재빌드 불필요)

### 적 타입 추가

1. `DataPack/XLSX/Enemy.xlsx`에 새 행 추가 (Num, MoveSpeed, RotationSpeed, Color)
2. 데이터 파이프라인 실행 → `Enemy.bytes` 갱신
3. 코드 변경 없이 새 타입이 무작위 스폰 풀에 포함.

### 새 레벨 추가

1. `DataPack/XLSX/Player.xlsx`에 새 레벨 행 추가 (Level, Speed, LevelUpTime, EnemySpawnTime)
2. 데이터 파이프라인 실행 → `Player.bytes` 갱신
3. `PlayerContext._maxLevel`이 DataChunk에서 자동으로 결정.

### 새 씬 추가

1. `IScene`을 상속하는 씬 클래스 작성 (`OnEnter` / `OnExit` 구현)
2. `App::OnStartup()`에서 `sceneManager->RegisterScene<NewScene>()` 등록
3. 전환이 필요한 컨트롤러에서 `sceneManager->Transition<NewScene>()` 호출

### 새 UI 액터 추가

1. `IActor` + `IActorController` + 필요한 모델 클래스 작성
2. 해당 씬의 `OnEnter()`에서 `CreateAndAddActor<NewUIActor>(key, order)` 호출
3. 이벤트 기반 반응이 필요하면 `PlayerContext`의 이벤트에 콜백 등록

### EffectTextActor 텍스트/외형 변경

`GameConfig.yaml`의 `TabText` / `LevelUpText` 섹션에서 색상, 폰트 크기, 이동 속도, 수명 조정 가능.
텍스트 내용 자체를 변경하려면 `PlayerActorController`의 `GenerateTabTextEffect()` / `GenerateLevelUpTextEffect()` 내 문자열을 수정.

### PlayerContext 이벤트 추가

게임 오버 또는 레벨업 외의 새 게임 상태 이벤트가 필요하면 `PlayerContext`에 `Event<>` 멤버를 추가하고,
상태 변경 시점(`AddPlayTime`, `SetGameOver` 등의 메서드)에서 `Invoke()` 호출.
수신 측은 `OnInitialize()`에서 콜백 등록.

### 최고 기록 영속화

현재 `_bestPlayTime`은 세션 메모리에만 존재.
파일에 저장하려면 Framework의 `SaveManager`(미구현, `IMPROVEMENT.md` 참고)가 추가된 후
`PlayerContext::Reset()` 호출 전 `SaveManager::Save()`, 게임 시작 시 `SaveManager::Load()` 연동.

---

## 주의사항

- **충돌 처리 중 액터 삭제 금지**: `OnCollision()` 콜백 내에서 `DestroyActor()`를 직접 호출하면 `PhysicManager`의 iterator가 무효화됨. 상태 플래그(`_isDead`)를 세우고 `OnTick()` 끝에서 처리.
- **씬 재진입 시 초기화 확인**: `GameScene::OnEnter()`는 `PlayerContext::Reset()`을 호출하지 않음. Reset은 `GameOverActorController`의 재시작 흐름에서 호출됨. 새 씬 흐름을 추가할 때 초기화 책임 위치를 명확히 할 것.
- **이펙트 풀 크기**: `EffectTextActor` 풀은 `PlayerActorController` 초기화 시 미리 생성됨. 동시 표시 가능한 이펙트 개수를 늘리려면 풀 초기 크기를 조정.
- **EnemySpawnTime 최솟값**: `PlayerDataChunk`의 `EnemySpawnTime`이 너무 작으면 매 프레임 적이 생성될 수 있음. 하한값 검증이 없으므로 데이터 입력 시 주의.
