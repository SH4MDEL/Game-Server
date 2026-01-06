#pragma once
#include "Types.h"

namespace GameServer::Core::Network
{
	struct PacketHeader
	{
		uint16 size; // 패킷 전체 크기 (헤더 + 바디)
		uint16 id;   // 패킷 ID (프로토콜 ID)
	};
}