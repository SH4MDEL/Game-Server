#include "pch.h"
#include "BufferReader.h"

namespace GameServer::Core
{
	BufferReader::BufferReader(std::span<byte> bufferView)
		: m_bufferView(bufferView), m_offset(0)
	{}

	BufferReader& BufferReader::operator>>(std::string& outStr)
	{
		uint16 len = 0;
		*this >> len;

		if (IsValid(len) == false)
			return *this;

		outStr.assign(reinterpret_cast<char*>(&m_bufferView[m_offset]), len);
		m_offset += len;
		return *this;
	}
}
