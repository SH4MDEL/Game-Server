#pragma once
#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "Debug\\CoreLibrary.lib")
#else
#pragma comment(lib, "Release\\CoreLibrary.lib")
#endif

#include "CoreHeader.h"

#endif //PCH_H