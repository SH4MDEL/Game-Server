#pragma once

namespace GameServer::Network
{
	using boost::asio::ip::tcp;

	class Session : public std::enable_shared_from_this<Session>
	{
	public:
		explicit Session(boost::asio::ip::tcp::socket socket);
		~Session();

		void Start();

	private:
		boost::asio::awaitable<void> ProcessEcho();

	private:
		tcp::socket m_socket;

		static constexpr uint32 MAX_BUFFER_SIZE = 1024;
		// [TODO] std::span 등을 활용한 ZERO-COPY 기법 적용 고려
		std::array<char, MAX_BUFFER_SIZE> m_buffer;
	};
}