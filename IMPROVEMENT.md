# Improvement Notes

개선 사항을 기록하는 문서입니다.

---

# Framework 개선 사항

---

## 렌더링 백엔드 추상화 인터페이스

### [현재] RenderManager가 렌더링 구현체에 직접 의존
- `RenderManager.cpp`가 Raylib API(`DrawCircleV`, `DrawRectanglePro` 등)를 직접 호출
- 렌더링 백엔드를 교체하려면 RenderManager 전체를 수정해야 함

### [개선안]
- `IRenderer` 인터페이스 정의 — RenderManager는 인터페이스만 사용
  ```cpp
  class IRenderer {
    virtual void DrawCircle(FVec2 pos, float radius, FVec4 color) = 0;
    virtual void DrawRect(FVec2 pos, FVec2 size, float rotate, FVec4 color) = 0;
    virtual void DrawText(const std::string& text, FVec2 pos, float fontSize, FVec4 color) = 0;
    // ...
  };
  ```
- 구현체(`RaylibRenderer`, `OpenGLRenderer` 등)만 교체하면 렌더링 백엔드 전환 가능
- SDL3 전환, 스프라이트 렌더링 추가 모두 이 인터페이스 확장으로 처리

---

## 스프라이트/텍스처 렌더링

> **NOTE:** SDL3 전환 시 함께 구현 예정. 별도 항목으로 추적하지 않고 입력 시스템 SDL3 전환 작업에 포함.
> 현재 렌더 타입(원, 사각형, 캡슐, 텍스트, 파티클)에 `ERenderType::SPRITE` 추가 및 에셋 매니저와 연동 필요.

---

## 2D 카메라 시스템 및 월드 좌표계

### [현재] 스크린 좌표계 직접 사용, 카메라 없음
- `IRenderableModel`의 모든 `GetPosition()`이 Raylib 스크린 좌표 (좌상단 원점, Y-하향) 직접 사용
- 월드 좌표 개념 없음 — 액터 위치 = 화면 픽셀 위치
- 카메라 이동/줌/회전 불가
- 렌더링 컬링 없음 — 화면 밖 오브젝트도 모두 렌더링

### [개선안]

**1. 월드 좌표계 정의**
```
     +y
      ^
      |
------+------> +x
      |
      |
```
- 카메라가 월드 원점(0, 0)에 있을 때 화면 중앙이 원점
- Y축 상향이 양수 (수학적 표준 좌표계)
- 액터의 모든 위치는 월드 좌표 기준으로 저장

**2. 2D 카메라**
- `Camera2D` : 월드 좌표 내 위치, 줌, 회전 보유
- `CameraManager` 또는 `RenderManager`가 카메라 상태 관리

**3. RenderManager의 월드 → 스크린 변환**
- 렌더링 시 카메라 기반으로 월드 좌표를 스크린 좌표로 변환
  ```
  screen_x = screen_center_x + (world_x - camera_x) * zoom
  screen_y = screen_center_y - (world_y - camera_y) * zoom  // Y 축 반전
  ```
- UI 액터는 변환 적용 제외 (스크린 좌표 고정)

**4. 뷰 프러스텀 컬링**
- 카메라 영역(뷰포트)을 벗어난 오브젝트는 렌더링 스킵
- AABB 기반으로 뷰포트와 교차 여부 판단

**5. 물리 엔진 좌표계 통일**
- Box2D는 기본적으로 Y-up 좌표계를 사용하므로 정의한 월드 좌표계와 자연스럽게 일치
- `PhysicManager`의 `b2World` 좌표계를 월드 좌표계와 동일하게 유지
- 물리 바디 위치(b2Body)와 액터 월드 좌표 간 변환 없이 1:1 대응
- 렌더링 시에만 RenderManager가 월드 → 스크린 변환 수행 (물리/게임 로직은 항상 월드 좌표 기준)

---

## 디버그 렌더링

### [현재] 디버그 시각화 없음
- 충돌 바운드, 월드 좌표축, 그리드 등을 화면에 표시하는 수단이 없음
- Box2D 통합 이후 물리 바디 시각화가 없으면 디버깅이 매우 어려움

### [개선안]
- 디버그 전용 렌더 패스를 별도로 추가 (Release 빌드에서는 완전 제거)
- 표시 항목: 충돌 바운드, Box2D 피직스 바디, 월드 좌표축, 카메라 뷰포트 경계, 그리드
- 항목별 토글 가능 (키 입력으로 런타임 on/off)

---

## GUI 전용 액터 시스템

### [현재] UI 액터와 게임 액터가 동일한 IActor를 공유
- `GameOverActor`, `PlayTimeActor`, `GameResultActor` 등 UI성 액터가 `PlayerActor`, `EnemyActor`와 동일한 `IActor` 기반
- 같은 월드 좌표계 사용 — 해상도 변경 시 UI 위치를 수동으로 재계산해야 함
- 충돌 처리에서 UI 액터를 명시적으로 제외해야 함
- 렌더 순서를 `ActorKey.order`로 수동 관리

### [개선안]
- `IUIActor` (또는 `ICanvasActor`) 기반 클래스 분리
  - 스크린 좌표계 사용 (월드 좌표와 독립)
  - 충돌 처리 대상에서 자동 제외
  - 월드 액터보다 항상 나중에 렌더링 (레이어 분리)
- 앵커 시스템 도입 — 해상도 변화에 자동 대응 (Unity RectTransform 유사)
- UI 전용 렌더 패스를 별도로 두어 월드/UI 렌더링 순서를 명확히 분리

---

## 텍스트 및 로컬라이즈 시스템

### [현재]
- 텍스트 렌더링은 `ITextModel`을 상속받아 각 액터마다 직접 문자열을 들고 있음
- 하드코딩된 문자열 ("GAME OVER", "Press SPACE to continue" 등)이 각 액터/모델에 산재
- 다국어 지원 구조 없음

### [개선안]

**1. 텍스트 전용 UI 액터 (Framework)**
- 문자열 표시에 특화된 `TextUIActor` (또는 `LabelActor`)를 Framework에 추가
- 폰트 크기, 색상, 정렬, 앵커 등 텍스트에 필요한 속성을 일관되게 관리

**2. 로컬라이즈 테이블 (Framework)**
- 키-값 기반 로컬라이즈 테이블 도입
  ```
  GAME_OVER_TEXT_KEY       → "GAME OVER"
  PRESS_SPACE_HINT_KEY     → "Press SPACE to continue"
  ```
- Application에서는 키 상수만 사용, 실제 문자열은 로컬라이즈 테이블이 변환
- 데이터는 기존 YAML/CSV 파이프라인으로 관리 가능

**3. TextUIActor + 로컬라이즈 테이블 연동**
- `TextUIActor`에 키를 넘기면 내부적으로 로컬라이즈 테이블을 조회해 렌더링
- 언어 변경 시 키는 그대로, 출력 문자열만 교체되는 구조

**4. 로컬라이즈 테이블 데이터 관리 — DataChunk 파이프라인 통합**
- 기존 XLSX → CSV → `.bytes` 파이프라인을 그대로 활용
  ```
  Localize.xlsx → Localize.csv → Localize.bytes
  ```
- `LocalizeDataChunk`를 Framework의 DataChunk로 추가, `DataChunkManager`가 로드
- 데이터 작업 흐름이 Enemy/Player 데이터와 동일하게 유지되어 파이프라인 일관성 확보

---

## 수학 자료구조 및 GLM 의존성

### [현재] GLM이 전체 레이어에 걸쳐 일관성 없이 사용됨
- Framework/Application: `glm::vec2` (위치, 방향), `glm::vec4` (색상) 사용
- DataPack: GLM 의존 불가 → `Color`를 `std::vector<float>`로 우회 저장
- yaml-cpp (GameConfig): `glm::vec4` 파싱을 수동으로 처리 (직렬화 지원 없음)
- GLM은 3D/OpenGL 중심 라이브러리로 2D 전용 프레임워크에서는 과도한 의존

### [개선안]
- 프레임워크 자체 경량 수학 타입 정의 (`FVec2`, `FVec4` 등)
  - 외부 의존성 없이 DataPack, yaml-cpp, Framework 모두에서 사용 가능
  - msgpack 직렬화(`MSGPACK_DEFINE`) 및 yaml-cpp 파싱을 기본 지원
  - `EnemyDataPack.Color`의 `std::vector<float>` 우회 불필요
- **GLM 완전 제거 목표** — 프레임워크 자체 수학 타입으로 전면 교체
- **Raylib 완전 제거 목표** — 렌더링/입력 추상화 레이어를 프레임워크가 직접 소유, 내부 구현체만 교체 가능한 구조로
- `MathUtils`의 `ToVector2`, `ToColor8Bit` 등 변환 함수는 렌더링 구현체 내부로 이동 (외부에 노출 불필요)

---

## Box2D 통합

### [현재] Box2D가 추가만 된 상태, 실제 미사용
- `PhysicManager.cpp`에 `#include <box2d/box2d.h>`만 있고 Box2D API는 전혀 사용하지 않음
- 충돌 감지: O(n²) 전수 검사 + 단순 원형 거리 계산 (`CollidableModel::IsCollision`)
- `CollidableModel`이 원형 바운드(`_boundCenter`, `_boundRadius`)만 지원
- 물리 시뮬레이션(반발, 마찰, 중력 등) 없음

### [개선안]
- `b2World` 를 `PhysicManager`가 소유하고 매 프레임 `Step()` 호출
- `CollidableModel` 대신 Box2D `b2Body` / `b2Fixture` 기반으로 물리 바디 관리
- 충돌 감지를 Box2D Contact Listener(`b2ContactListener`)로 대체 → O(n²) 제거
- 원형 외 다각형, 박스 등 다양한 충돌 형태 지원
- 물리가 필요 없는 액터(UI 등)는 Sensor 또는 Static Body로 처리

---

## 오디오 시스템

### [현재] 오디오 없음
- 현재 프로젝트에 오디오 처리가 전혀 없음
- 과거 SDL_mixer 사용 경험 있음

### [개선안] miniaudio 도입
- single-header 라이브러리로 외부 의존성 없이 통합 가능
- SDL_mixer 대비 장점:
  - SDL 의존 없이 독립적으로 동작 (SDL3 미사용 시에도 사용 가능)
  - C 기반 single-header(`miniaudio.h`)로 CMake 연동이 단순
  - 저수준 오디오 제어 가능 (샘플링, 믹싱, 이펙트 등)
  - 크로스플랫폼 (Windows/Mac/Linux/iOS/Android)
- `AudioManager`를 프레임워크 매니저로 추가, `miniaudio` 래핑
- BGM(스트리밍), SFX(단발성 재생) 인터페이스 분리

---

## 크래시 덤프 및 에러/로그 시스템

### [현재] 로그가 Raylib에 종속, 크래시 덤프 없음
- `LogUtils`가 Raylib의 `TraceLog`를 래핑 → Raylib 내부 초기화 로그와 게임 로그가 뒤섞임
- Raylib 제거 목표와 충돌 (로그 시스템이 Raylib에 종속)
- `Error` 클래스에 파일/라인/함수 정보는 있으나 스택 트레이스 없음
- 크래시 덤프 생성 없음 (과거 Windows API `MiniDumpWriteDump` 경험 있음, Crashpad는 과도하다고 판단)
- 로그 파일 출력 없음, AI가 분석할 수 있는 구조화된 포맷 없음

### [개선안]

**1. 로그 시스템 Raylib에서 분리**
- `LogUtils`가 Raylib `TraceLog` 대신 자체 로그 백엔드 사용
- Raylib의 로그 콜백(`SetTraceLogCallback`)으로 Raylib 내부 로그를 별도 채널로 격리하거나 무음 처리
- 로그 레벨 필터링 지원 (Debug/Info/Warn/Error/Fatal)

**2. 구조화된 로그 출력 (AI 분석 가능)**
- JSON Lines 포맷으로 로그 파일 저장
  ```json
  {"timestamp":"2026-04-04T12:00:00","level":"ERROR","code":"0x3003","file":"ActorManager.cpp","line":42,"func":"GetActor","msg":"NOT_FOUND_ACTOR"}
  ```
- 세션 단위로 로그 파일 분리 (`Log/session_20260404_120000.jsonl`)
- AI가 로그 파일을 읽어 에러 패턴 분석 가능한 구조

**3. 크래시 덤프 생성**
- Windows API `MiniDumpWriteDump` (`DbgHelp.h`) 사용 — 외부 의존 없음
- `SetUnhandledExceptionFilter`로 미처리 예외 캐치 → `.dmp` 파일 자동 생성
- 덤프 파일에 크래시 직전 로그 버퍼도 함께 저장
- Crashpad는 도입하지 않음 (과도한 의존성)

**4. Error 클래스 보강**
- 에러 체인 지원 (원인 에러를 래핑하는 구조)
- `EErrorCode`를 카테고리별로 명확히 분리 (현재도 잘 되어 있으나 Physics, Audio 등 추가 필요)

---

## 메모리 할당자 및 메모리 추적 툴

### [현재] 기본 할당자 사용, 메모리 추적 없음
- `new`/`delete`가 C++ 기본 할당자를 그대로 사용
- 할당/해제 패턴, 누수, 단편화 등을 런타임에 추적하는 수단이 없음
- 게임 루프 중 핫패스에서의 동적 할당 비용을 측정하거나 줄일 방법이 없음

### [개선안]

**1. mimalloc 도입**
- Microsoft에서 개발한 고성능 범용 메모리 할당자
- `operator new`/`delete` 전역 교체만으로 기존 코드 변경 없이 적용 가능
- jemalloc 대비 단편화가 낮고 스레드 친화적인 구조
- 과거 사용 경험이 있어 CMake 연동 및 API 친숙도 확보됨

**2. 메모리 추적 레이어 구축 (`MemoryTracker`)**
- mimalloc의 `mi_malloc_usable_size`, heap 통계 API(`mi_heap_get_statics`) 활용
- 추적 항목: 누적 할당 횟수, 현재 사용 중인 바이트, 피크 사용량, 해제되지 않은 블록 수
- 프레임 단위로 샘플링하여 디버그 렌더링 또는 JSON Lines 로그로 출력
- Release 빌드에서는 추적 레이어를 완전히 제거 (조건부 컴파일)

**3. 연동 범위**
- Framework의 모든 매니저, Actor/Model/Controller 생성·소멸 경로에 적용
- 크래시 덤프 생성 시 덤프 파일에 메모리 통계 스냅샷도 함께 저장

---

## Config 시스템

### [현재] YAML 필드 하나 추가에 3곳 수정 필요
YAML에 새 필드를 추가하거나 수정하면 아래 세 곳을 모두 수정해야 함:
1. `GameConfig.h` — 멤버 변수 + getter 추가
2. `GameConfig.cpp` — `TryGetValue` 호출 추가
3. `Config/*.yaml` — 실제 값 추가

`CoreConfig`, `GameConfig` 모두 동일한 반복 작업 발생.

### [개선안] 동적 경로 기반 접근으로 전환
YAML 트리를 메모리에 그대로 보관하고, 키 경로로 타입 안전하게 접근:
```cpp
// 기존
gameConfig->GetPlayerRadius()

// 개선
configMgr->Get<float>("GameConfig", "Player/Radius")
configMgr->Get<FVec4>("GameConfig", "Player/Color")
```
- YAML 파일만 수정하면 C++ 코드 변경 없이 즉시 접근 가능
- `CoreConfig`, `GameConfig` 같은 typed wrapper 클래스 불필요 (또는 선택적으로만 유지)
- 존재하지 않는 키 접근 시 기본값 반환 또는 에러 로그 출력
- 중첩 노드는 `/` 구분자로 경로 표현 (`"Spawn/RangeMinX"`)

---

## 씬/액터 데이터 직렬화

### [현재] 씬 구성이 C++ 코드에 하드코딩
- 씬의 액터 구성, 초기 위치, 텍스트 내용 등이 모두 `OnEnter()` 코드에 직접 작성
- 씬 구성 변경 시 코드 수정 + 재빌드 필요

### [개선안]
- 씬 구성, 액터 초기 상태, UI 텍스트 등을 외부 파일(YAML 또는 자체 포맷)로 정의
  ```yaml
  # Scene/GameScene.yaml
  actors:
    - key: "PlayerActor"
      order: 10
      position: [0, 0]
    - key: "EnemySpawnActor"
      order: 20
  ui:
    - key: "PlayTimeActor"
      order: 100
      anchor: top-right
  ```
- `SceneManager` 또는 `IScene`이 파일을 읽어 액터를 자동 생성
- 기존 DataChunk 파이프라인(XLSX → CSV → .bytes)과 연계하거나 YAML 직접 사용
- **향후 비주얼 에디터 구축 시 이 파일 포맷이 에디터의 저장/불러오기 기반이 됨**

---

## 씬 스택 (Scene Stack)

### [현재] 단일 씬 flat 구조
- `SceneManager`가 현재 씬 하나만 관리
- 일시정지 메뉴, 확인 다이얼로그 같은 오버레이 씬을 기존 씬 위에 올리는 구조 불가능
- 씬 전환 트랜지션 효과(페이드 인/아웃 등) 없음

### [개선안]
- 씬 스택(push/pop) 방식 도입
  - `PushScene<T>()` : 현재 씬을 유지한 채 새 씬을 위에 올림 (오버레이)
  - `PopScene()` : 오버레이 씬 제거, 이전 씬 복귀
- 씬 전환 트랜지션 효과를 프레임워크 레벨에서 지원

---

## 게임 일시정지 (Pause)

### [현재] 게임 루프 수준의 일시정지 없음
- `AppHost`가 매 프레임 무조건 `OnTick`을 호출
- 일시정지 구현 시 각 액터/컨트롤러가 개별적으로 상태를 확인해야 함

### [개선안]
- `AppHost` 또는 `IApp`에 Pause/Resume 인터페이스 추가
- 일시정지 시 `OnTick` 호출 자체를 건너뛰거나, 일시정지 전용 tick만 호출하는 구조
- UI 액터는 일시정지 중에도 업데이트되어야 하므로 월드/UI tick 분리와 연동

---

## 세이브/로드 시스템

### [현재] 영속화 인프라 없음
- `PlayerContext`의 `_bestPlayTime` 등 유지되어야 할 데이터가 메모리에만 존재, 게임 종료 시 소멸
- Application이 직렬화/파일 관리까지 직접 책임져야 하는 구조

### [개선안]
- Framework에 `SaveManager` 추가 — 키-값 기반 영속화 인프라 제공
  ```cpp
  // Framework 제공
  SaveManager::Set("BestPlayTime", 123.4f);
  SaveManager::Get<float>("BestPlayTime");
  SaveManager::Save();   // 파일 직렬화 (MessagePack 활용)
  SaveManager::Load();   // 파일 역직렬화
  ```
- Application은 저장할 키 이름만 정의, 직렬화/파일 관리는 프레임워크가 담당
- Unity의 `PlayerPrefs`와 유사한 역할

---

## 오브젝트 풀 (Object Pool)

### [현재] 풀 관리가 Application에 종속
- `EnemySpawnActorController`가 직접 적 풀을 관리
- Framework에 범용 오브젝트 풀 없어서 다음 프로젝트에서도 동일 코드 반복 필요

### [개선안]
- Framework에 범용 `ObjectPool<T>` 추가
- 액터 재사용 패턴을 프레임워크 레벨에서 표준화

---

## 에셋 매니저

### [현재] 그래픽 에셋 관리 시스템 없음
- `.bytes` 파일(DataChunk) 외에 텍스처, 폰트, 스프라이트 관리 구조 없음
- 현재는 Raylib가 내부적으로 처리하고 있어 표면에 드러나지 않지만, Raylib 제거 시 필수

### [개선안]
- `AssetManager`를 Framework 매니저로 추가
- 텍스처, 폰트 등 그래픽 리소스의 로드/언로드/캐싱 담당
- 렌더링 백엔드(OpenGL 등)와 분리된 추상화 인터페이스로 제공

---

## 타이머 유틸리티

### [현재] 각 컨트롤러가 타이머를 직접 관리
- `EnemySpawnActorController`, `GameOverActorController`, `EnemyActorController` 등이 각자 `_elapsedTime` 변수를 들고 중복 관리
- Framework에 공통 `Timer` 클래스 없음

### [개선안]
- Framework에 `Timer` 클래스 추가 (카운트다운, 반복, 일회성 등 모드 지원)
- 컨트롤러는 `Timer`를 멤버로 보유하고 `Tick(deltaSeconds)` 호출만 하면 됨

---

## 트윈/이징 시스템

### [현재] 이동/페이드 효과를 각 컨트롤러가 수동 처리
- `EffectTextActorController` 등에서 위치 이동, 알파 페이드를 직접 계산
- 이징 함수(ease-in, ease-out 등) 없이 선형 보간만 사용

### [개선안]
- Framework에 `Tween` 클래스 추가
  ```cpp
  Tween::To(value, targetValue, duration, EEasing::EaseOutQuad)
       .OnUpdate([](float v) { model->SetAlpha(v); })
       .OnComplete([](){ actor->SetVisible(false); });
  ```
- 기본 이징 함수 내장 (Linear, EaseIn/Out/InOut Quad/Cubic 등)
- 타이머 시스템과 연동하여 매 프레임 자동 업데이트

---

## 액션 기반 입력 매핑

### [현재] 키 코드 하드코딩
- 컨트롤러마다 `EKey::LEFT`, `EKey::RIGHT` 등 키 코드를 직접 사용
- 키 변경 시 코드를 직접 수정해야 하며, 게임패드 지원 추가 시 전면 수정 필요

### [개선안]
- 액션 이름과 키를 분리하는 입력 매핑 레이어 추가
  ```cpp
  // Config 또는 코드에서 매핑 정의
  inputMgr->BindAction("MoveLeft",  EKey::LEFT);
  inputMgr->BindAction("MoveRight", EKey::RIGHT);

  // 컨트롤러에서는 액션 이름으로만 조회
  inputMgr->GetActionPress("MoveLeft")
  ```
- 매핑 정보를 Config(YAML)로 관리해 코드 수정 없이 키 변경 가능
- 게임패드 버튼도 동일한 액션에 바인딩 가능한 구조로 확장 용이

---

## 입력 시스템

### [현재] Raylib 위에 수동 상태 추적
- `GetKeyStateEx()`로 이전/현재 프레임 상태를 비교해 PRESSED/RELEASED/HELD를 판단
- Raylib의 폴링 API 위에 이벤트 방식을 억지로 얹은 구조
- `EKey` 열거형 값이 Raylib 키코드에 종속되어 있음

### [개선안] 다음 프로젝트에서 SDL3로 교체 검토
- SDL3의 이벤트 큐(`SDL_PollEvent`)는 현재 PRESSED/RELEASED/HELD 상태 머신과 자연스럽게 맞음
- 게임패드, 텍스트 입력, 마우스 휠 등 복잡한 입력 처리가 체계적
- 오디오 시스템도 SDL 생태계로 통합 가능
- 프레임워크 확장성을 고려하면 SDL3가 더 적합한 기반

### [렌더링 선택지 비교]
| 조합 | 셰이더 | 구현 비용 | 비고 |
|---|---|---|---|
| Raylib | GLSL 내장 지원 | 낮음 | 셰이더 필요 시에도 Raylib로 충분 |
| SDL3 + OpenGL | GLSL 직접 구현 | 높음 | 기존 경험 활용 가능 |
| SDL3 + SDL_GPU | SPIR-V 등 현대적 | 중간 | SDL3 신규 API, 레퍼런스 적음 |

- Raylib도 `LoadShader()`, `BeginShaderMode()` 등 셰이더 API를 내장하고 있어 "Raylib = 셰이더 불가"는 오해
- 렌더링 레이어 직접 구축이 목표라면 SDL3 + OpenGL, 시스템 설계에 집중하고 싶으면 Raylib 유지 권장

---

## Framework / Application 분리 원칙

### [현재] 재사용 가능한 액터가 Application에 혼재
- `ParticleActor`는 이미 Framework에 위치 (잘 된 사례)
- `EffectTextActor` (떠오르는 텍스트 이펙트) 같은 범용 UI 액터가 Application에 위치
- Framework에는 `IActor`, `IActorModel`, `IActorController` 인터페이스만 존재하고 구체적인 액터가 없음

### [개선안]
- 분리 기준: **"게임 로직과 무관하게 파라미터만 바꾸면 다른 게임에서도 쓸 수 있는가"**
  - Framework에 위치해야 하는 것: 파티클, 떠오르는 텍스트 이펙트, 범용 UI 액터들 (버튼, 레이블 등)
  - Application에 위치해야 하는 것: 게임 규칙에 종속된 액터 (GameOver, PlayTime, Enemy 등)
- `IUIActor` 기반 클래스가 Framework에 정립되면, 범용 UI 액터들도 자연스럽게 Framework에 귀속

---

## 테스트 자동화

### [현재] 체계적인 테스트 없음
- `Sandbox/`가 존재하지만 수동 실험 용도이며 자동화된 검증 수단이 없음
- Framework 핵심 로직(Result, Event, Actor 생명주기, 충돌 판정 등) 변경 시 회귀 오류를 잡을 방법이 없음

### [개선안]
- Google Test(gtest) 등 C++ 테스트 프레임워크 도입 — 이미 ThirdParty에 yaml-cpp 테스트용으로 googletest가 포함되어 있음
- 테스트 대상: `Result<T>`, `Event<>`, `CollidableModel::IsCollision`, `SceneManager` 전환 로직 등 Framework 순수 로직
- CI/CD 파이프라인(GitHub Actions 등)과 연동하여 PR 시 자동 실행

---

## XLSX 데이터 검증

### [현재] 데이터 유효성 검증 없음
- XLSX → CSV → `.bytes` 파이프라인에서 데이터 값의 유효성을 검사하는 단계가 없음
- 잘못된 값(범위 초과, 빈 셀, 타입 불일치 등)이 그대로 `.bytes`로 변환되어 런타임에서야 문제 발견
- 컬럼 추가/삭제 시 파서 코드와의 불일치를 사전에 감지할 수단이 없음

### [개선안]
- `data-pipeline`에 검증 단계 추가 (XLSX → CSV 변환 직후)
- 검증 규칙을 별도 스키마 파일로 정의
  ```yaml
  # Schema/Enemy.yaml
  columns:
    Num:        { type: int,   required: true, min: 1 }
    MoveSpeed:  { type: int,   required: true, min: 1, max: 1000 }
    Color:      { type: float_array, length: 4, min: 0.0, max: 1.0 }
  ```
- 검증 실패 시 파이프라인 즉시 중단 + 어느 행/컬럼이 문제인지 명시
- `--output-format json`으로 AI가 검증 결과를 파싱해 자동 수정 제안 가능

---

# 빌드/파이프라인 개선 사항

---

## DataChunk 생성 파이프라인

### [현재] 4단계 배치 파일을 수동으로 순서대로 실행
엑셀 데이터 하나를 수정하면 아래 4개를 순서대로 실행해야 함:
1. `ConvertAllXLSXToCSV.bat` → XLSX → CSV
2. `GenerateAllDataPackHeader.bat` → CSV → `.generated.h`
3. `Build.bat Debug DataChunkGenerator` → `DataChunkGenerator.exe` 빌드
4. `GenerateDataChunk.bat` → `DataChunkGenerator.exe` 실행 → `.bytes`

`cli.py`에 각 단계별 커맨드는 이미 있으나 전체를 묶는 pipeline 커맨드가 없음.
배치 파일에 `pause`가 있어 자동화 시 blocking 발생.

### [개선안]
- `cli.py`에 `data-pipeline` 커맨드 추가 — 4단계를 순서대로 실행, 실패 시 즉시 중단 및 실패 단계 명시
- `--output-format json` 옵션 추가 — AI가 파싱 가능한 구조화된 출력
  ```json
  {"step": "convert-xlsx", "status": "ok"}
  {"step": "generate-header", "status": "failed", "error": "..."}
  ```
- `--target` 옵션으로 특정 데이터만 처리 가능하게 (`--target Player`)
- 배치 파일의 `pause` 제거 또는 `--no-pause` 옵션 추가

---

## 인스톨러

### [현재] CPack ZIP 패키징만 존재
- `CMakeLists.txt`에서 `CPACK_GENERATOR "ZIP"`으로 바이너리를 ZIP으로만 묶음
- 인스톨러 없음 — 사용자가 직접 압축을 풀어야 함
- 과거 NSIS(1999년) 사용 경험 있으나, 스크립트가 복잡하고 오래된 방식

### [개선안] WiX v4 도입
- CMake CPack이 `CPACK_GENERATOR "WIX"`를 기본 지원 → 기존 빌드 파이프라인과 자연스럽게 통합
- XML 기반 선언적 구성으로 NSIS 대비 가독성 높음
- `.msi` 포맷 출력 — Windows 표준 인스톨러
- 향후 Microsoft Store 제출 시 MSIX로 전환 가능한 경로 확보
- 기존 `Package.bat` → `cli.py package` 파이프라인에 인스톨러 생성 단계 추가

---

# Application 개선 사항

---

## GameConfig 분리

### [현재] 모든 게임 설정이 파일 하나에 집중
- `GameConfig` 하나에 Spawn, Player, Enemy, Particle, UI 텍스트 설정이 모두 집중
- 설정 항목이 늘어날수록 파일이 비대해지고 어느 시스템이 어떤 값을 쓰는지 추적이 어려움

### [개선안]
- 시스템/씬 단위로 Config 분리
  ```
  Config/Player.yaml
  Config/Enemy.yaml
  Config/Particle.yaml
  Config/UI.yaml
  ```
- 씬/액터 데이터 직렬화(Framework 개선안)와 연동하면 씬 파일에 해당 씬 설정을 함께 정의 가능

---

## PlayerContext 역할 분리

### [현재] 게임 상태 전반을 하나의 Context가 담당
- `PlayerContext`가 레벨, 플레이 시간, 베스트 기록, 게임오버 여부, 레벨업/게임오버 이벤트까지 모두 보유
- 게임이 커질수록 단일 Context가 비대해지고 책임 경계가 불명확해짐

### [개선안]
- 역할 기준으로 Context 분리
  - `PlaySessionContext` : 현재 판의 플레이 시간, 레벨, 게임오버 여부 (휘발성)
  - `PlayerRecordContext` : 베스트 기록 등 영속 데이터 → SaveManager와 연동

---

## 적 이동 패턴 다양화

### [현재] 직선 하강 패턴만 존재
- `EnemyActorController`가 매 프레임 Y 방향으로만 이동
- `EnemyDataPack`에 패턴 타입 필드가 없어 모든 적이 동일하게 움직임

### [개선안]
- `EnemyDataPack`에 패턴 타입 필드 추가 (데이터로 제어)
  ```
  PatternType: STRAIGHT / ZIGZAG / HOMING / WAVE
  ```
- `EnemyActorController`가 패턴 타입에 따라 이동 로직을 분기
- 패턴별 파라미터(진폭, 주기 등)도 DataPack으로 관리

---

## 스코어 시스템

### [현재] 생존 시간만 기록
- 게임 결과가 플레이 시간 하나뿐
- 잘 피했는지, 레벨을 얼마나 올렸는지 등 성과 지표가 없음

### [개선안]
- 점수 체계 추가 (피한 적 수, 레벨 보너스, 생존 시간 복합 산정)
- `GameResultActor` 결과 화면에 점수 세부 내역 표시
- SaveManager와 연동하여 최고 점수 영속 저장

---

## 난이도 선택

### [현재] 고정 난이도
- 스폰 간격, 적 속도 등이 PlayerDataPack 기준으로만 조정되며 난이도 선택지 없음

### [개선안]
- TitleScene에 난이도 선택 UI 추가 (Easy / Normal / Hard)
- 난이도별 배율 값을 Config(YAML)로 관리
- 선택된 난이도를 Context에 저장하고 EnemySpawnActorController가 참조

