# Dodge-Ball Framework

**Version: 0.0**

---

## 개요

Dodge-Ball Framework는 2D 아케이드 게임 *Dodge Ball*을 위해 제작된 C++20 기반 경량 게임 엔진입니다.
렌더링에 raylib, 설정 파싱에 yaml-cpp, 바이너리 데이터 직렬화에 msgpack-cxx, 수학 연산에 GLM을 사용합니다.
물리 엔진으로 Box2D가 포함되어 있으나 현재 버전(0.0)에서는 사용되지 않습니다.

Application 레이어(게임 로직)와 Framework 레이어(엔진 공통 기능)를 명확히 분리하여,
게임 로직을 추가·변경할 때 엔진 코드를 직접 수정하지 않아도 되는 구조를 지향합니다.

---

## 목적

- 게임 개발에 필요한 반복적인 인프라(렌더링, 입력, 씬 전환, 충돌 감지, 데이터 로드 등)를 Framework에서 일괄 제공하여, Application은 **게임 규칙 구현에만 집중**할 수 있도록 합니다.
- 동일한 Framework 코드 베이스 위에서 다른 게임 프로젝트도 만들 수 있는 **재사용 가능한 기반**을 확보합니다.

---

## 목표

| 목표 | 설명 |
|------|------|
| **관심사 분리** | Actor(개체) = Model(데이터) + Controller(로직)으로 분리 |
| **명시적 에러 처리** | 예외 대신 `Result<T>` 패턴으로 성공/실패를 타입으로 표현 |
| **안전한 씬 전환** | 게임 루프 중 메모리 문제를 방지하는 지연 씬 전환 |
| **확장 가능한 인터페이스** | IActor, IScene, IApp, IConfig 등을 상속하여 게임별로 확장 |
| **통일된 매니저 시스템** | 8개 매니저가 게임의 모든 서브시스템을 관리, Service Locator로 접근 |

---

## 디렉토리 구조

```
Framework/
├── CMakeLists.txt
└── Source/
    ├── Inc/                        # 헤더 (인터페이스 및 선언)
    │   ├── Actor/                  # IActor, IActorModel, IActorController
    │   ├── App/                    # AppHost, IApp
    │   ├── Config/                 # IConfig, CoreConfig
    │   ├── Context/                # IContext
    │   ├── Error/                  # Error, Result<T>
    │   ├── Event/                  # Event<Args...>
    │   ├── Locator/                # ManagerLocator
    │   ├── Macro/                  # GameAssert, DebugBreak, Macro
    │   ├── Manager/                # 8개 매니저 헤더
    │   ├── Particle/               # 파티클 액터 시스템
    │   ├── Physic/                 # CollidableModel
    │   ├── Render/                 # IRenderableModel 계층
    │   ├── Scene/                  # IScene
    │   ├── Text/                   # TextActor, TextModel
    │   └── Utils/                  # GameTimer, LogUtils, MathUtils
    └── Src/                        # 구현 (.cpp)
        ├── App/
        ├── Config/
        ├── Locator/
        ├── Manager/
        ├── Particle/
        ├── Physic/
        ├── Text/
        └── Utils/
```

---

## 의존성

| 라이브러리 | 버전/비고 | 용도 |
|-----------|----------|------|
| **raylib** | - | 2D 렌더링, 창 관리, 입력 폴링 |
| **yaml-cpp** | - | 설정 파일(.yaml) 파싱 |
| **msgpack-cxx** | - | 바이너리 데이터(.bytes) 직렬화 |
| **GLM** | - | 벡터/색상 수학 타입 |
| **box2d** | 포함만, 미사용 | 향후 물리 엔진 통합 예정 |
| **DataPack** | 로컬 모듈 | 게임 데이터 구조 공유 |

---

## 기능 목록

### 1. 애플리케이션 호스트 (`AppHost`)

게임 루프와 모든 매니저의 초기화·해제를 담당합니다.

**매니저 초기화 순서** (의존성 순서):
```
ConfigManager → DataChunkManager → ContextManager → RenderManager
→ InputManager → ActorManager → SceneManager → PhysicManager
```

**매니저 해제 순서**: 초기화의 역순

**게임 루프**:
```
while (!quit) {
    GameTimer::Tick()
    App::OnPreTick()
    App::OnTick()
    App::OnPostTick()
    App::OnRender()
    SceneManager::FlushTransitionEvent()   ← 지연 씬 전환 실행
}
```

---

### 2. 액터 시스템 (`IActor` / `IActorModel` / `IActorController`)

게임 월드의 모든 개체(Entity)를 표현하는 Model-Controller 구조입니다.

- **Model**: 위치·색상·충돌체 등 데이터 보유 (`IActorModel` 상속)
- **Controller**: 매 프레임 로직 수행 (`IActorController` 상속)
- 한 액터에 여러 Model과 여러 Controller를 조합 가능
- `IRenderableModel`을 상속한 모델은 자동으로 렌더링 대상이 됨
- `CollidableModel`을 상속한 모델은 자동으로 충돌 감지 대상이 됨

```cpp
// 액터에 모델/컨트롤러 추가 (order = 실행 순서)
actor->AddModel<MyModel>(order, args...);
actor->AddController<MyController>(order, args...);

// 조회
auto result = actor->GetModel<MyModel>();
```

---

### 3. 씬 시스템 (`IScene` / `SceneManager`)

씬(Level/State)의 등록, 전환, 생명주기를 관리합니다.

- **지연 전환**: `Transition<T>()` 등록 후 게임 루프 끝에서 `FlushTransitionEvent()` 실행 (루프 중 메모리 문제 방지)
- **즉시 전환**: `TransitionImmediate<T>()` (루프 외부에서만 사용)

```cpp
// 씬 등록
sceneManager->RegisterScene<TitleScene>();
sceneManager->RegisterScene<GameScene>();

// 씬 전환 (지연)
sceneManager->Transition<GameScene>();
```

씬 구현 시 `OnEnter()` / `OnExit()`를 오버라이드하여 액터 생성·정리를 수행합니다.

---

### 4. 렌더링 시스템 (`RenderManager` / `IRenderableModel`)

raylib 기반의 2D 렌더링을 제공합니다.

**지원 렌더 타입**:

| 타입 | 인터페이스 | raylib 함수 |
|------|-----------|------------|
| `CIRCLE` | `ICircleModel` | `DrawCircleV` |
| `RECTANGLE` | `IRectModel` | `DrawRectanglePro` |
| `CAPSULE` | `ICapsuleModel` | `DrawCircleV` × 2 + Rect |
| `PARTICLE` | `ParticleModel` | `DrawCircleV` × N |
| `TEXT` | `ITextModel` | `DrawTextEx` |

렌더링 순서는 `AddModel(order)` 에서 지정한 `order` 값으로 결정됩니다.

---

### 5. 물리/충돌 시스템 (`PhysicManager` / `CollidableModel`)

현재 원형(Circle) 충돌체 기반의 O(n²) 전수 검사를 수행합니다.

```cpp
// CollidableModel에 충돌체 정보 설정
model->SetBound(center, radius);
model->SetCollidable(true);

// 충돌 발생 시 IActor::OnCollision() 호출됨
void MyActorController::OnCollision(IActor* other) { ... }
```

---

### 6. 입력 시스템 (`InputManager`)

raylib 폴링 위에 이전/현재 프레임 상태를 비교하여 4가지 입력 상태를 제공합니다.

| 상태 | 의미 |
|------|------|
| `NONE` | 미입력 |
| `PRESSED` | 방금 눌림 (1프레임) |
| `HELD` | 계속 누르고 있음 |
| `RELEASED` | 방금 뗌 (1프레임) |

```cpp
if (inputManager->GetKeyPress(EKey::SPACE) == EPress::PRESSED) { ... }
if (inputManager->IsAppCloseRequested()) { quit = true; }
```

---

### 7. 데이터 시스템 (`DataChunkManager` / `ConfigManager`)

**DataChunkManager**: msgpack으로 직렬화된 `.bytes` 파일을 로드합니다.
```cpp
dataChunkManager->LoadDataChunk<EnemyDataChunk>("Data/Enemy.bytes");
const auto* data = dataChunkManager->GetDataChunk<EnemyDataChunk>().GetValue();
```

**ConfigManager**: YAML 설정 파일을 로드합니다.
```cpp
configManager->LoadConfig<GameConfig>("Config/Game.yaml");
const auto* cfg = configManager->GetConfig<GameConfig>().GetValue();
```

---

### 8. 이벤트 시스템 (`Event<Args...>`)

옵저버 패턴 기반의 제네릭 이벤트입니다.

```cpp
Event<int, std::string> onScoreChanged;

// 등록
onScoreChanged.RegisterCallback("ui", [](int score, std::string label) { ... });

// 발생
onScoreChanged.Invoke(100, "Player");

// 해제
onScoreChanged.UnregisterCallback("ui");
```

---

### 9. 에러 처리 (`Result<T>` / `Error`)

예외 대신 `Result<T>`로 성공/실패를 명시적으로 표현합니다.

```cpp
Result<MyActor*> result = actorManager->GetActor<MyActor>("player");
if (!result.IsSuccess()) {
    LOG_E("액터를 찾을 수 없습니다: {}", result.GetError().GetMessage());
    return;
}
MyActor* actor = result.GetValue();
```

`MAKE_ERROR(code, message)` 매크로는 파일명·줄 번호·함수명을 자동으로 포함합니다.

**에러 코드 카테고리**:
| 범위 | 카테고리 |
|------|---------|
| `0x0000–0x0006` | 일반 (초기화, 미구현) |
| `0x1000–0x1003` | Config |
| `0x2000–0x2005` | DataChunk |
| `0x3000–0x3005` | Actor |
| `0x4000` | Event |
| `0x7000–0x7001` | Scene |
| `0x8000–0x8001` | Context |
| `0xFFFF` | 내부 에러 |

---

### 10. 기타 유틸리티

**GameTimer**: `Tick()` 호출마다 DeltaTime 및 TotalTime 계산. `Stop()`/`Start()`로 일시정지 가능.

**LogUtils**: `LOG_I`, `LOG_W`, `LOG_E`, `LOG_F` 매크로. 파일명·줄 번호·함수명 자동 포함.
현재 raylib의 `TraceLog`를 래핑하고 있어 Raylib 내부 로그와 섞여 출력됩니다.

**MathUtils**: GLM ↔ raylib 타입 변환(`ToVector2`, `ToColor8Bit` 등), 난수 생성(`GenerateRandomInt/Float`).

**ParticleActor**: 생성 위치, 개수, 크기·속도·수명 범위, 색상을 파라미터로 받아 폭발형 파티클 이펙트를 재생합니다. Framework에 내장된 범용 액터입니다.

**TextActor**: 텍스트, 위치, 색상, 폰트 크기를 파라미터로 받아 화면에 중앙 정렬 텍스트를 표시합니다. Framework에 내장된 범용 액터입니다.

---

## 유지보수 가이드

### 새 액터 추가 방법

1. `IActor`를 상속하는 액터 클래스 생성
2. `IActorModel`을 상속하는 모델 클래스 생성 (필요에 따라 `IRenderableModel`, `CollidableModel` 추가 상속)
3. `IActorController`를 상속하는 컨트롤러 클래스 생성
4. 액터의 생성자에서 `AddModel()`, `AddController()` 호출
5. 씬의 `OnEnter()`에서 `CreateAndAddActor<MyActor>(key, order)` 호출

### 새 씬 추가 방법

1. `IScene`을 상속하는 씬 클래스 생성
2. `OnEnter()`에서 필요한 액터 생성
3. `OnExit()`에서 `ClearActorMap()` 호출
4. `App::OnStartup()` 등에서 `SceneManager::RegisterScene<MyScene>()` 등록

### 새 Config 추가 방법

1. `IConfig`를 상속하는 Config 클래스 생성
2. `TryParse(const YAML::Node& node)` 구현
3. `ConfigManager::LoadConfig<MyConfig>(path)` 호출

### 새 DataChunk 추가 방법

1. `DataPack/` 에 msgpack 직렬화 구조체 정의 (`MSGPACK_DEFINE` 매크로 사용)
2. `DataChunkManager::LoadDataChunk<MyDataChunk>(path)` 호출

### 씬 전환 시 주의사항

- `OnCollision()` 콜백 내부에서 액터를 즉시 삭제하면 iterator 무효화가 발생할 수 있습니다.
  삭제가 필요하면 플래그를 세워 두고 `OnTick()` 끝에서 처리하거나, 씬 전환을 지연 방식으로 사용하십시오.
- 씬 전환은 반드시 `Transition<T>()`(지연)를 사용하고, `FlushTransitionEvent()`가 게임 루프 끝에서 호출되도록 유지하십시오.

### 매니저 초기화 순서 변경 시 주의사항

`AppHost::Startup()` 의 매니저 초기화 순서는 의존성을 반영합니다.
`RenderManager`는 raylib 창을 생성하므로 `InputManager`보다 먼저 초기화되어야 합니다.
`ConfigManager`는 다른 모든 매니저가 설정값을 읽기 전에 완료되어야 합니다.
순서를 변경할 경우 각 매니저의 `Startup()` 내부 의존성을 반드시 확인하십시오.

### Result<T> 사용 원칙

모든 매니저 메서드의 반환값은 `Result<T>`입니다.
`IsSuccess()` 확인 없이 `GetValue()`를 호출하면 미정의 동작이 발생합니다.
반드시 다음 패턴을 따르십시오:

```cpp
auto result = manager->DoSomething();
if (!result.IsSuccess()) {
    LOG_E("{}", result.GetError().GetMessage());
    return;
}
auto* value = result.GetValue();
```

### 렌더링 순서 제어

`AddModel(order, ...)` 의 `order` 값이 낮을수록 먼저 렌더링됩니다(배경).
값이 높을수록 나중에 렌더링됩니다(전경, UI).

---

## 빌드 방법

`Tool/Batch/` 아래의 배치 파일로 빌드한다.

| 배치 파일 | 설명 | 사용법 |
|-----------|------|--------|
| `Generate.bat` | Solution 생성 (최초 1회) | `Generate.bat` |
| `BuildSolution.bat` | 전체 솔루션 빌드 | `BuildSolution.bat <Debug\|Release> [true\|false]` |
| `Build.bat` | 특정 타겟만 빌드 | `Build.bat <Debug\|Release> <TargetName> [true\|false]` |
| `Package.bat` | 빌드 후 패키징 | `Package.bat <Debug\|Release>` |

세 번째 인자(`true`/`false`)는 리빌드 여부 (기본값: `false`).

## 데이터 파이프라인

XLSX → CSV → DataChunk(`.bytes`) 순서로 변환한다.

| 배치 파일 | 설명 |
|-----------|------|
| `ConvertAllXLSXToCSV.bat` | `XLSX/` 하위 파일 전체를 `CSV/`로 변환 |
| `GenerateAllDataPackHeader.bat` | DataPack 헤더 일괄 생성 |
| `GenerateDataChunk.bat [Debug\|Release] [TargetName]` | DataChunk(`.bytes`) 생성 (빌드 선행 필요) |

---

## v0.0 이후 개선 사항

아래 항목들은 현재 버전에서 구현되지 않은 개선 과제입니다.
상세 배경 및 개선 방향은 루트 경로의 `IMPROVEMENT.md`를 참고하십시오.

---

### Framework 개선

#### 렌더링 백엔드 추상화
`RenderManager`가 raylib API를 직접 호출합니다. `IRenderer` 인터페이스를 정의하고 `RaylibRenderer` 같은 구현체만 교체하면 렌더링 백엔드를 전환할 수 있는 구조가 필요합니다.
- GLM 완전 제거, 프레임워크 자체 수학 타입(`FVec2`, `FVec4`) 도입 목표와 함께 진행
- Raylib 완전 제거, 렌더링/입력을 프레임워크가 직접 소유하는 구조 목표

#### 스프라이트/텍스처 렌더링
현재 도형(원, 사각형, 캡슐)과 텍스트만 렌더링할 수 있습니다. `ERenderType::SPRITE` 추가 및 에셋 매니저 연동이 필요합니다. SDL3 전환 시 함께 구현 예정입니다.

#### 2D 카메라 시스템 및 월드 좌표계
현재 액터 위치 = 화면 픽셀 좌표입니다. 수학적 표준 Y-up 월드 좌표계와 `Camera2D`를 도입하고, 렌더링 시에만 월드 → 스크린 변환을 수행하는 구조가 필요합니다. 뷰 프러스텀 컬링도 함께 구현합니다. Box2D의 Y-up 좌표계와 자연스럽게 일치합니다.

#### 디버그 렌더링
충돌 바운드, Box2D 피직스 바디, 월드 좌표축, 그리드 등을 화면에 표시하는 디버그 전용 렌더 패스가 필요합니다. Release 빌드에서는 완전히 제거되어야 합니다.

#### GUI 전용 액터 시스템(`IUIActor`)
UI 액터와 게임 액터가 동일한 `IActor` 기반을 사용합니다. 스크린 좌표계를 사용하고 충돌 처리에서 자동 제외되는 `IUIActor`(또는 `ICanvasActor`) 기반 클래스 분리가 필요합니다. 앵커 시스템(Unity RectTransform 유사)도 함께 도입합니다.

#### 텍스트 및 로컬라이즈 시스템
게임 내 문자열이 각 액터에 하드코딩되어 있습니다. 텍스트에 특화된 `TextUIActor`(Framework 내장)와 키-값 기반 로컬라이즈 테이블을 도입합니다. 기존 DataChunk 파이프라인(XLSX → CSV → `.bytes`)으로 로컬라이즈 데이터를 관리합니다.

#### 수학 자료구조 및 GLM 의존성 제거
GLM이 Framework/Application/DataPack 전 레이어에 혼재하며 DataPack에서는 GLM을 사용할 수 없어 `std::vector<float>`로 우회합니다. 자체 경량 수학 타입(`FVec2`, `FVec4`)을 정의하여 msgpack 및 yaml-cpp 직렬화를 기본 지원하고 GLM을 완전 제거합니다.

#### Box2D 통합
Box2D가 포함만 된 상태입니다. `PhysicManager`에 `b2World`를 도입하고, `b2ContactListener`로 충돌 감지를 교체하여 현재의 O(n²) 전수 검사를 제거합니다. 원형 외 다양한 충돌 형태도 지원합니다.

#### 오디오 시스템(`AudioManager`)
현재 오디오 처리가 없습니다. single-header 라이브러리 `miniaudio`를 도입하여 `AudioManager`를 Framework 매니저로 추가합니다. BGM(스트리밍)과 SFX(단발성 재생) 인터페이스를 분리합니다.

#### 크래시 덤프 및 로그 시스템 개선
로그가 raylib `TraceLog`에 종속되어 있고 크래시 덤프가 없습니다.
- `LogUtils`를 raylib에서 분리, 자체 로그 백엔드 사용
- JSON Lines 포맷으로 로그 파일 저장 (AI 분석 가능)
- Windows API `MiniDumpWriteDump`로 크래시 덤프 자동 생성 (Crashpad 미사용)
- 에러 체인 지원 및 `EErrorCode`에 Physics, Audio 카테고리 추가

#### 메모리 할당자 및 메모리 추적 툴
현재 `new`/`delete` 기본 할당자를 사용하며 런타임 메모리 추적 수단이 없습니다.

- **mimalloc 도입**: Microsoft의 고성능 범용 메모리 할당자. `operator new`/`delete` 전역 교체만으로 기존 코드 변경 없이 적용 가능. 단편화가 낮고 스레드 친화적이며, 과거 사용 경험으로 CMake 연동 및 API 친숙도가 확보되어 있습니다.
- **메모리 추적 레이어(`MemoryTracker`) 구축**: mimalloc의 heap 통계 API(`mi_heap_get_statics`)를 활용하여 누적 할당 횟수, 현재 사용량, 피크 사용량, 미해제 블록 수를 프레임 단위로 샘플링합니다. 디버그 렌더링 또는 JSON Lines 로그로 출력하며, Release 빌드에서는 조건부 컴파일로 완전히 제거합니다.
- **연동 범위**: Framework 매니저 및 Actor/Model/Controller 생성·소멸 경로 전체에 적용. 크래시 덤프 생성 시 메모리 통계 스냅샷도 함께 저장합니다.

#### Config 시스템 개선
새 YAML 필드를 추가하면 헤더, 구현, YAML 파일 세 곳을 수정해야 합니다. YAML 트리를 메모리에 그대로 보관하고 키 경로로 타입 안전하게 접근하는 방식으로 전환합니다.
```cpp
configMgr->Get<float>("GameConfig", "Player/Radius")
```

#### 씬/액터 데이터 직렬화
씬의 액터 구성과 초기 상태가 `OnEnter()` 코드에 하드코딩되어 있습니다. YAML 파일로 씬 구성을 외부화하면 재빌드 없이 씬 구조를 수정할 수 있고, 향후 비주얼 에디터 구축 시 파일 포맷 기반이 됩니다.

#### Lua 스크립팅 시스템
ActorController의 동작(`OnInitialize`, `OnTick`, `OnCollision`)이 C++ 코드에 하드코딩되어 있어 수정 시 재빌드가 필요합니다. `IActorController`가 Lua 스크립트를 로드하고 각 콜백 시점에 해당 함수를 호출하는 구조를 Framework에 추가합니다. `IActor`·`IActorModel`·`EventSystem`·`ContextManager`의 주요 API를 sol2(header-only, C++17)로 바인딩하여 Lua에 노출합니다. 매니저 직접 접근은 노출하지 않아 바인딩 범위를 제한합니다. Debug 빌드에서는 핫 리로드를 지원하며, 씬/액터 데이터 직렬화 개선안과 연계하면 씬 YAML에서 액터별 스크립트 파일을 지정하는 구조로 확장할 수 있습니다.

#### 씬 스택 (Scene Stack)
현재 `SceneManager`는 씬 하나만 관리합니다. `PushScene<T>()`/`PopScene()`으로 일시정지 메뉴나 다이얼로그를 기존 씬 위에 올리는 오버레이 씬 구조와 트랜지션 효과(페이드 인/아웃)를 추가합니다.

#### 게임 일시정지 (Pause)
`AppHost`가 매 프레임 무조건 `OnTick`을 호출합니다. `IApp`에 `Pause()`/`Resume()` 인터페이스를 추가하고, UI 액터는 일시정지 중에도 업데이트되도록 월드/UI tick을 분리합니다.

#### 세이브/로드 시스템(`SaveManager`)
`PlayerContext`의 베스트 기록 등 영속 데이터가 메모리에만 존재합니다. Unity `PlayerPrefs`와 유사한 `SaveManager`를 Framework 매니저로 추가하여 키-값 기반 영속화와 MessagePack 직렬화를 제공합니다.

#### 오브젝트 풀(`ObjectPool<T>`)
현재 `EnemySpawnActorController`가 직접 풀을 관리합니다. Framework에 범용 `ObjectPool<T>`를 추가하여 액터 재사용 패턴을 표준화합니다.

#### 에셋 매니저(`AssetManager`)
텍스처, 폰트 등 그래픽 리소스 관리 구조가 없습니다. 렌더링 백엔드와 분리된 추상화 인터페이스로 `AssetManager`를 Framework 매니저로 추가합니다.

#### 타이머 유틸리티(`Timer`)
각 컨트롤러가 `_elapsedTime` 변수를 직접 관리합니다. 카운트다운·반복·일회성 모드를 지원하는 `Timer` 클래스를 Framework에 추가합니다.

#### 트윈/이징 시스템(`Tween`)
이동, 페이드 등 보간 효과를 각 컨트롤러가 수동 처리합니다. 이징 함수(EaseIn/Out/InOut 등)와 콜백을 지원하는 `Tween` 클래스를 Framework에 추가하고 타이머 시스템과 연동합니다.

#### 액션 기반 입력 매핑
컨트롤러마다 키 코드를 직접 사용합니다. 액션 이름과 키를 분리하는 매핑 레이어를 추가하고, 매핑 정보를 YAML로 관리하여 코드 수정 없이 키 변경 및 게임패드 확장이 가능한 구조가 필요합니다.

#### 입력 시스템 — SDL3 전환 검토
현재 raylib 폴링 위에 상태를 수동 추적합니다. `EKey` 열거형 값이 raylib 키 코드에 종속되어 있습니다. SDL3 이벤트 큐(`SDL_PollEvent`)는 현재 PRESSED/RELEASED/HELD 상태 머신과 자연스럽게 맞으며, 게임패드·텍스트 입력·마우스 휠 처리도 체계적입니다. 렌더링 선택지(Raylib 유지 vs SDL3+OpenGL vs SDL3+SDL_GPU)와 함께 다음 프로젝트에서 검토합니다.

#### Framework / Application 분리 원칙 강화
`EffectTextActor`처럼 게임 로직과 무관하게 파라미터만 바꾸면 다른 게임에서도 쓸 수 있는 범용 UI 액터가 Application에 위치합니다. `IUIActor` 기반이 Framework에 정립되면 이런 액터들을 Framework으로 이동합니다.

#### 테스트 자동화
체계적인 자동화 테스트가 없습니다. ThirdParty에 이미 포함된 `googletest`를 활용하여 `Result<T>`, `Event<>`, `CollidableModel::IsCollision`, `SceneManager` 전환 로직 등 Framework 순수 로직에 대한 단위 테스트를 작성하고 GitHub Actions와 연동합니다.

#### XLSX 데이터 유효성 검증
데이터 파이프라인에 유효성 검사 단계가 없어 잘못된 값이 런타임에서야 발견됩니다. XLSX → CSV 변환 직후에 스키마 파일 기반 검증 단계를 추가하고, 실패 시 행/컬럼을 명시하여 즉시 중단합니다.

---

### 빌드/파이프라인 개선

#### DataChunk 생성 파이프라인 자동화
XLSX 수정 후 4개 배치 파일을 순서대로 수동 실행해야 합니다. `cli.py`에 `data-pipeline` 커맨드를 추가하여 4단계를 자동 순서 실행하고, 실패 시 즉시 중단 및 단계 명시, `--output-format json` 옵션으로 AI 파싱 가능한 구조화된 출력을 지원합니다.

#### 인스톨러 (WiX v4)
현재 CPack ZIP 패키징만 있습니다. CMake CPack의 `CPACK_GENERATOR "WIX"`를 활용하여 `.msi` 인스톨러를 빌드 파이프라인에 통합합니다. 향후 Microsoft Store 제출 시 MSIX 전환 경로도 확보됩니다.
