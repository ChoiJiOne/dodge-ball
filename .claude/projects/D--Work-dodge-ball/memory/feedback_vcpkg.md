---
name: vcpkg 미사용 의도
description: ThirdParty를 소스째 포함하는 것은 의도된 결정 — vcpkg 제안 금지
type: feedback
---

ThirdParty 라이브러리를 소스 코드째 레포에 포함하는 것은 명확하게 의도된 설계다. vcpkg나 Conan 같은 패키지 매니저를 제안하지 않는다.

**Why:** vcpkg를 약 한 달 전에 사용해봤으나 불안정했음. 목표는 CMake + Visual Studio + Python만 있으면 누구든 바로 개발할 수 있는 환경. 외부 패키지 매니저 설치를 요구하지 않는 것이 핵심 조건.

**How to apply:** 의존성 관리 개선을 제안할 때 vcpkg, Conan 등 패키지 매니저는 언급하지 않는다.
