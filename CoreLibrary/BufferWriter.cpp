#include "pch.h"
#include "BufferWriter.h"

namespace GameServer::Core
{
	BufferWriter::BufferWriter()
	{
		m_buffer.reserve(DEFAULT_SIZE);
	}

	BufferWriter& BufferWriter::operator<<(const std::string& str)
	{
		uint16 len = static_cast<uint16>(str.size());
		*this << len;
		const byte* ptr = reinterpret_cast<const byte*>(str.data());
		m_buffer.insert(m_buffer.end(), ptr, ptr + len);
		return *this;
	}

	byte* BufferWriter::ReserveHeader()
	{
		if (m_buffer.size() < sizeof(Network::PacketHeader))
		{
			m_buffer.resize(sizeof(Network::PacketHeader));
		}
		return m_buffer.data();
	}
}