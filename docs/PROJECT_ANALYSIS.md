# 프로젝트 분석 및 유지보수성 검토

노션 요청: "저장소의 코드는 Direct3D 를 이용하여 개발한 간단한 게임이야. 먼저 이 프로젝트를 분석하고 유지보수가 가능한지 검토해줘"

> 이 문서는 소스 **읽기 기반 정적 분석**입니다. 빌드 환경이 Linux 라서 Windows/Direct3D 9 빌드와 실행은 하지 못했습니다.

## 1. 한눈에 보기

| 항목 | 내용 |
|---|---|
| 그래픽 API | Direct3D 9 (`d3d9.h`, `d3dx9.h`). `Include/`에 D3D10/11, XAudio2 등 헤더도 있으나 코드에서는 D3D9만 사용 |
| 언어 / 툴체인 | C++ (C++11 이전 스타일), Win32 API, Visual Studio 솔루션 (`PlatformToolset v143`, MultiByte) |
| 구성 | 독립된 3개 샘플 프로젝트 (`Airplane`, `Cube`, `Galaxy`) + 공용 SDK 헤더/라이브러리 (`Include/`, `Lib/`) |
| 실제 소스 규모 | 약 3,500줄 (SDK 헤더 제외, `_backup_cp949` 중복 제외) |
| 커밋 이력 | `InitRepo` 1개 (이력 없음) |
| 테스트 / CI | 없음 |

### 프로젝트별 요약

- **Airplane (`02_ShootingGame`)** – 2D 슈팅. 삼각형 하나를 플레이어/적/총알로 재사용. 방향키 이동·회전, 스페이스 발사, 적 50기, 총알 20발 풀, 좌상단 HP 바(RHW 정점). `GameMgr.cpp` 734줄에 거의 전부 집중.
- **Cube (`04_DrawCube`)** – 정육면체 그리기. `BASE_Object`/`Cube` 로 객체화, 버텍스/인덱스 버퍼 래퍼(`SH_Buffer`, `SH_VertexBuffer`) 도입.
- **Galaxy (`07_Planet`)** – 태양계 행성 렌더링. 텍스처(`Resources/`), 메쉬, 스프라이트, 자유/FPS 카메라(`SH_FreeCam`, `SH_FpsCam`), 그리드(`SH_Grid`), 행성 클래스(`Planet`). `GameMgr.cpp` 748줄.

세 프로젝트는 학습 순서대로 기능이 누적된 구조로 보이며, 공용 파일(`Singleton`, `SH_UTIL`, `ImageNode`, `defines.h`, `stdafx.*` 등)이 **폴더마다 복사**되어 있습니다.

### 공통 구조

```
winmain.cpp  ── 창 등록/생성, GAME_MGR->init / msgLoop / release
   └─ GameMgr (Singleton<GameMgr>)   : D3D 장치, 입력, update/render, WndProc 를 모두 소유
        ├─ WM_TIMER(10ms) → update() → render()   (고정 타이머 루프, deltaTime 미사용)
        ├─ SH_UTIL (math / collision / window / keyInput)  ← 정적 재구현
        └─ (Cube/Galaxy) BASE_Object 계층, SH_Buffer, MemManager, VoidList
```

## 2. 유지보수 가능성 판단

**결론: "유지보수 가능(조건부)". 학습/포트폴리오용 소규모 샘플로서는 읽기 쉽고 기능 추가도 가능하지만, 지금 상태로 계속 기능을 얹으면 비용이 빠르게 늘어납니다. 아래 "선결 과제"만 정리하면 안전하게 확장할 수 있습니다.**

### 강점
- 규모가 작고(≈3.5K줄) 함수 단위 주석(`//<`)이 촘촘해 흐름 파악이 쉽다.
- `GameMgr` 가 init/update/render/release 로 생애주기가 명확하다.
- `SH_UTIL.dll` 을 정적 소스(`SH_UTIL.cpp`)로 재구현하여 디버그 CRT 의존으로 인한 실행 불가(0xC0000135) 문제를 이미 해소했다 (`SH_UTIL_STATIC`).
- `.vcxproj` 가 v143/Windows 10 SDK 로 최신 VS 에서 열리도록 갱신되어 있다 (Airplane 확인).
- Galaxy 는 `OnLostDevice/Reset` 훅(`lostDevice/resetDevice`), `MemoryLeak.h`(CRT 누수 체크) 등 방어 장치를 일부 갖췄다.

### 위험 요소 (우선순위순)

| # | 위험 | 근거 | 영향 |
|---|---|---|---|
| 1 | **빌드 산출물/바이너리가 저장소에 커밋됨** | `Cube/Source/04_DrawCube/{Debug,Release}` (.obj/.pdb/.ilk/.pch, 약 74MB), `Cube/Source/{Debug,Release}`, 각 폴더 `*.exe`, `SH_UTIL.dll`, `Lib/`(17MB). `.gitignore` 는 `Airplane/Source` 에만 존재, Cube/Galaxy 에는 없음. `.git` 이 50MB | diff 노이즈, 저장소 비대화, 충돌, 출처 불명 바이너리 실행 위험 |
| 2 | **코드 3중 복제** | 같은 파일이 Airplane/Cube/Galaxy 에 각각 존재 (`ImageNode`, `Singleton`, `SH_UTIL`, `MemManager`, `VoidList`, `SH_Buffer` …). 심지어 각 프로젝트의 `_backup_cp949/` 에 소스 전체가 또 복제됨 (Cube/Galaxy) | 버그 수정 시 최대 6곳을 손봐야 하고 서로 어긋나기 쉬움. `Cube.cpp` 와 backup 은 이미 내용이 다름 |
| 3 | **`GameMgr` 신(God) 클래스 + 전역 싱글톤** | Airplane `GameMgr` 가 장치 생성, 입력, 플레이어/적/총알 로직, HP UI, 렌더, 메시지 루프를 모두 담당 (734줄) | 기능 추가 때마다 한 파일이 비대해지고 단위 테스트 불가 |
| 4 | **자원/초기화 안전성** | Airplane `GameMgr` 생성자가 `m_player`, `m_enemy`, `m_d3d9`, `m_d3dDevice9`, `m_rhwHPVer`, `m_playerVertex` 를 초기화하지 않음. 소멸자가 `release()` 를 호출하므로 `init` 실패 시 미초기화 포인터를 해제할 위험. `m_playerVertex` 는 `release()` 에서 해제되지 않아 누수. 원시 `new/delete` | 초기화 실패/조기 종료 경로에서 크래시 가능, 누수 |
| 5 | **디바이스 로스트 미처리 (Airplane/Cube)** | `D3DPOOL_DEFAULT` 버텍스 버퍼를 쓰면서 `TestCooperativeLevel/Reset` 처리가 없음 (Galaxy 만 일부) | 화면 잠금/해상도 변경 후 렌더 실패. `Present` 실패 시 `Sleep(500)` 만 수행 |
| 6 | **게임 루프/시간 처리** | `SetTimer(10ms)` + `WM_TIMER` 로 update/render 구동. 이동은 프레임당 고정값(`moveSpeed`), `deltaTime` 인자는 쓰이지 않음. 적 이동(`setEnemyTM`)과 총알 이동(`updateBullet`)이 **render 안에서** 상태를 변경 | 기기/타이머 해상도(실제 ~15.6ms)에 따라 속도 달라짐, 로직과 렌더 분리 불가 |
| 7 | **버그성 코드** | `initProjection`: `WINSIZE_X / WINSIZE_Y` 정수 나눗셈 → 종횡비 1.0(원래 1.33). 적 스폰 반경(≥100)과 비활성 반경(>100)이 같아 경계 의존. HP 바 정점 초기값이 만점 폭 하드코딩. `#ifndef _DEBUG` 일 때만 사망 처리 → 디버그에서는 HP 가 음수가 되어도 게임이 계속됨 | 동작은 하나 의도와 다른 부분 |
| 8 | **인코딩 혼재** | 소스는 UTF-8(BOM) 이고 `_backup_cp949` 는 CP949 원본, 프로젝트는 MultiByte 설정. `MessageBox("죽었습니다.")` 등 한글 리터럴이 컴파일러 코드페이지에 의존. 일부 `ReadMe.txt` 는 인코딩이 깨져 보임 | 로케일이 다른 PC 에서 글자 깨짐/경고(C4819) |
| 9 | **레거시 의존/라이선스** | 오래된 DirectX SDK(June 2010) 헤더/lib 를 `Include/`, `Lib/` 에 동봉. `d3dx9` 는 deprecated. `MemManager.h` 헤더에 "무단 배포 및 수정을 불허합니다" 라는 원저자 문구가 있음 | 공개 저장소로 배포 시 라이선스 확인 필요 |
| 10 | **테스트·문서·CI 부재** | 단위 테스트/빌드 스크립트/README 없음 (`ReadMe.txt` 는 VS 마법사 기본 문구) | 회귀 검출 불가, 신규 참여자 진입 장벽 |

## 3. 권장 조치 (작은 단위, 위험 낮은 순)

1. **저장소 위생**: 루트 `.gitignore` 추가(`Debug/`, `Release/`, `x64/`, `*.obj *.pdb *.ilk *.pch *.tlog *.exe *.dll` 등) 후 `git rm -r --cached` 로 산출물 제거. 배포용 exe 는 GitHub Release 로 분리.
2. **`_backup_cp949/` 제거** (CP949 원본이 꼭 필요하면 태그/브랜치로 보존).
3. **공용 코드 단일화**: `Common/` 폴더(또는 static lib 프로젝트)로 `Singleton`, `SH_UTIL`, `ImageNode`, `defines.h` 이동 후 각 솔루션이 참조.
4. **Airplane 안전성 패치**: 생성자 멤버 초기화(`nullptr`), `m_playerVertex` 해제, 종횡비 `static_cast<float>` 수정. (한 파일, 수 줄)
5. **루프 개선**: `PeekMessage` 루프 + `QueryPerformanceCounter` 기반 `deltaTime`, `render()` 안의 상태 변경을 `update()` 로 이동.
6. **GameMgr 분리**: `Player`, `EnemyPool`, `BulletPool`, `HudRenderer`, `D3DDevice` 래퍼로 책임 분할 (Cube/Galaxy 의 `BASE_Object` 구조를 Airplane 에도 적용).
7. **디바이스 로스트 처리**, 소스 인코딩을 UTF-8 + `/utf-8` 옵션으로 통일.
8. 장기: D3D9/d3dx9 → DirectXMath + D3D11 로 이전 검토, GitHub Actions(Windows 러너, `msbuild`) 로 빌드 검증.

## 4. 이번 작업의 범위

- 요청이 "분석 및 유지보수 가능 여부 검토" 이므로 **게임 코드는 수정하지 않았고** 이 문서만 추가했습니다.
- 위 권장 조치는 별도 개발 요청(노션)으로 나누어 진행하는 것을 제안합니다. 특히 1·2번은 저장소 이력/배포물에 영향을 주므로 승인 후 진행이 바람직합니다.
