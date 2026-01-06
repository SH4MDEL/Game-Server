# Game Server Framework (C++23/Boost.Asio)

## 🎯 Project Goals

### 1. Boost.Asio 기반 고성능 멀티쓰레드 게임 서버 개발
   - Boost.Asio의 I/O Context Pool 또는 Thread-per-Core 모델을 활용하여 동시성 처리를 극대화합니다.
   - Non-blocking I/O 및 비동기 프로그래밍 모델을 적용하며, Callback Hell을 방지하기 위해 C++20 Coroutine을 적극 활용합니다.
   - 패킷 처리 시 Zero-copy 메커니즘을 지향하여 불필요한 메모리 복사를 최소화합니다.
   - 최종 목표는 Linux(epoll) 및 Windows(IOCP)에서 모두 고성능을 발휘하는 플랫폼 독립적 아키텍처입니다.

### 2. Modern C++ (C++20/23) 심화 적용
   - C++20 Concepts를 활용하여 템플릿 코드의 타입 안전성을 보장하고, 컴파일 오류 메시지의 가독성을 높입니다.
   - 런타임 다형성(Virtual function) 대신, CRTP(Curiously Recurring Template Pattern)나 Concepts를 활용한 정적 다형성(Static Polymorphism)을 우선 고려하여 성능 오버헤드를 줄입니다.
   - C++23의 `std::expected`, `std::move_only_function`, `Deducing this` 등 최신 기능을 실무 레벨에서 검증하고 적용합니다.

### 3. 지속 가능하고 유연한 설계 (Solid & Scalable)
   - 단일 책임 원칙(SRP)을 준수하되, 과도한 엔지니어링을 피하고 YAGNI(You Aren't Gonna Need It) 원칙을 고려합니다.
   - 핵심 로직과 네트워크 계층을 분리하여, 비즈니스 로직의 독립성을 보장합니다.

### 4. AI 활용
   - 개발에 AI를 적극 활용하여, 불필요한 작업을 줄입니다.

## 📏 Coding Conventions & Rules

### 1. 들여쓰기 및 포맷팅
   - 들여쓰기는 탭(Tab) 문자를 사용하며, 탭 크기는 4 space로 가정합니다.

### 2. 명명 규칙 (Naming Convention)
   - 변수명: camelCase (예: `userCount`)
   - 멤버 변수: m_ 접두사 사용 (예: `m_userCount`)
   - 전역 변수: g_ 접두사 사용 (예: `g_instance`)
   - 함수/클래스/인터페이스: PascalCase (예: `CreateUser`, `NetworkSession`)
   - 상수: SCREAMING_SNAKE_CASE (예: `MAX_BUFFER_SIZE`)
   - 헝가리안 표기법 금지.

### 3. 코드 구조 및 객체지향 설계
   - 상속보다는 컴포지션(Composition)을 선호합니다.
   - 인터페이스 구현 목적 외의 다중 상속은 금지합니다.
   - 런타임 성능을 위해 가상 함수(Virtual Function) 사용을 신중히 결정하며, 가능하면 템플릿 기반의 정적 다형성을 활용합니다.
   - 클래스는 단일 책임 원칙(SRP)을 준수합니다.

### 4. 메모리 관리 및 스마트 포인터
   - 기본적으로 스마트 포인터(`std::unique_ptr`) 사용을 원칙으로 하여 메모리 누수를 방지합니다.
   - `std::shared_ptr`은 소유권 공유가 '반드시' 필요한 경우에만 제한적으로 사용합니다. (참조 카운팅 오버헤드 주의)
   - 함수 인자로 객체를 전달할 때는 소유권 이전이 없다면 Raw Pointer(*)나 Reference(&)를 사용하여 불필요한 참조 카운트 연산을 피합니다.
   - `std::unique_ptr::get()`은 소유권이 이전되지 않음을 보장할 때만 사용합니다.

### 5. 동시성 및 쓰레드 안전성 (Concurrency)
   - Mutex 사용 시 `std::lock_guard` 또는 `std::scoped_lock`을 사용하여 RAII 패턴으로 관리합니다.
   - 데드락 방지를 위해 락의 범위를 최소화하고, 중첩 락 사용을 지양합니다.
   - Singleton 패턴은 꼭 필요한 경우(예: 전역 매니저)에만 Thread-Safe 하게 구현하여 사용합니다.

### 6. 에러 처리 (Error Handling)
   - 로직 흐름 제어를 위해 Exception(try-catch)을 사용하는 것을 지양합니다.
   - 실패 가능성이 있는 함수는 `std::optional` 또는 C++23의 `std::expected`를 반환 타입으로 사용합니다.

### 7. 파일 구조 및 헤더 정책
   - 원칙적으로 선언(Header)과 구현(Source)을 분리합니다. 
   - 단, 템플릿 함수, 인라인이 필요한 짧은 함수(Getter/Setter 등), 성능에 민감한 코드는 헤더 파일 내 구현을 허용합니다.
   - 헤더 파일 간 의존성을 줄이기 위해 전방 선언(Forward Declaration)을 적극 활용합니다.
   - 프로젝트 공통 헤더는 PCH(Precompiled Header)를 활용합니다.

### 8. Primitive Type
   - types.h에 정의된 고정 폭 정수형을 사용합니다.
   - `int8`, `uint8`, `int16`, `uint16`, `int32`, `uint32`, `int64`, `uint64`, `float32`, `float64`