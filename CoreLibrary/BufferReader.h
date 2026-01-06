#pragma once
#include "CoreHeader.h"

namespace GameServer::Core
{
	template <typename T>
	concept PrimitiveType = !std::is_pointer_v<T> && std::is_trivially_copyable_v<T>;

	class BufferReader
	{
	public:
		BufferReader(std::span<byte> buffer);
		~BufferReader() = default;

		template <PrimitiveType T>
		BufferReader& operator>>(T& outValue)
		{
			if (IsValid(sizeof(T)) == false)
				return *this; // [TODO] 예외 대신 에러 처리 필요

			outValue = *reinterpret_cast<T*>(&m_bufferView[m_offset]);
			m_offset += sizeof(T);
			return *this;
		}

		template <typename T>
		BufferReader& operator>>(std::vector<T>& data)
		{
			uint16 len = 0;
			*this >> len;
			data.clear();
			data.resize(len);
			for (auto& item : data)
			{
				*this >> item;
			}
			return *this;
		}

		BufferReader& operator>>(std::string& outStr);

		uint32 RemainedSize() const { return static_cast<uint32>(m_bufferView.size()) - m_offset; }

	private:
		bool IsValid(uint32 size)
		{
			return (m_offset + size) <= m_bufferView.size();
		}

	private:
		std::span<byte> m_bufferView;
		uint32 m_offset;
	};
}