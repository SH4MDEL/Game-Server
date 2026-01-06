#pragma once
#include "CoreHeader.h"
#include "PacketHeader.h"

namespace GameServer::Core
{
	template <typename T>
	concept PrimitiveType = !std::is_pointer_v<T> && std::is_trivially_copyable_v<T>;

	class BufferWriter
	{
	public:
		BufferWriter();
		~BufferWriter() = default;

		std::span<byte> GetBuffer() { return m_buffer; }
		void Clear() { m_buffer.clear(); }

		template <PrimitiveType T>
		BufferWriter& operator<<(const T& value)
		{
			const byte* ptr = reinterpret_cast<const byte*>(&value);
			m_buffer.insert(m_buffer.end(), ptr, ptr + sizeof(T));
			return *this;
		}

		template <typename T>
		BufferWriter& operator<<(const std::vector<T>& data)
		{
			uint16 len = static_cast<uint16>(data.size());
			*this << len;
			for (const auto& item : data)
			{
				*this << item;
			}
			return *this;
		}

		BufferWriter& operator<<(const std::string& str);

		byte* ReserveHeader();

		uint16 Size() const { return static_cast<uint16>(m_buffer.size()); }

	private:
		static constexpr uint32 DEFAULT_SIZE = 4096;
		std::vector<byte> m_buffer;
	};
}