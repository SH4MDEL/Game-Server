#include "pch.h"
#include "Server.h"
#include "Session.h"

namespace GameServer::Network
{
	Server::Server(Core::IoContextPool& ioContextPool, uint16 port)
		: m_ioContextPool(ioContextPool)
		, m_acceptor(ioContextPool.GetIoContext(), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	{
	}

	void Server::Start()
	{
		// Accept 작업을 수행할 전용 코루틴 시작
		boost::asio::co_spawn(
			m_acceptor.get_executor(),
			[this] { return AcceptConnections(); },
			boost::asio::detached
		);
	}

	boost::asio::awaitable<void> Server::AcceptConnections()
	{
		for (;;)
		{
			// [Goals 1] I/O Context Pool 활용
			// 새 연결을 처리할 IO Context를 가져옴
			boost::asio::io_context& ioContext = m_ioContextPool.GetIoContext();

			// 새 소켓 생성
			boost::asio::ip::tcp::socket socket(ioContext);

			// 연결 대기 (Non-blocking)
			co_await m_acceptor.async_accept(socket, boost::asio::use_awaitable);

			// 세션 생성 및 시작
			// [Rules 4] Session 내부에서 shared_from_this를 사용하므로 make_shared 필수
			std::make_shared<Session>(std::move(socket))->Start();

			std::cout << "[Server] Client Accepted." << std::endl;
		}
	}
}