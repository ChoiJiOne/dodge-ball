# DodgeBall
- 간단한 2D 피하기 게임 프로젝트.

## 프로젝트 구조

```
dodge-ball/
├── Application/     # 게임 로직 (씬, 액터 등)
├── Framework/       # 엔진 코어 (매니저 시스템)
├── DataPack/        # 게임 데이터 패키징 도구
├── Config/          # 설정 파일 (CoreConfig.yaml)
├── Tool/            # 빌드/데이터 자동화 배치 스크립트
└── ThirdParty/      # 외부 라이브러리
```

## 참고 문서

| 문서 | 내용 |
|------|------|
| [`Framework/README.md`](Framework/README.md) | 프레임워크 구조, 매니저 시스템, 빌드 방법, 데이터 파이프라인 |
| [`Application/README.md`](Application/README.md) | 게임 로직, 씬 구성, 액터 목록, 밸런스 수치 |
