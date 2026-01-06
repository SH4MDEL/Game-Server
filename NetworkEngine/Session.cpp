#include "pch.h"
#include "Session.h"

namespace GameServer::Network
{
	Session::Session(boost::asio::ip::tcp::socket socket)
		: m_socket(std::move(socket))
	{
	}

	Session::~Session()
	{
		// 소멸 시점 확인용 로그
		// std::cout << "[Session] Destroyed." << std::endl;
	}

	void Session::Start()
	{
		// Non-blocking I/O 및 비동기 프로그래밍 모델 적용
		// Co_spawn을 통해 코루틴 실행. shared_from_this()로 생명주기 유지.
		boost::asio::co_spawn(
			m_socket.get_executor(),
			[self = shared_from_this()] { return self->ProcessEcho(); },
			boost::asio::detached
		);
	}

	boost::asio::awaitable<void> Session::ProcessEcho()
	{
		try
		{
			for (;;)
			{
				std::size_t bytesTransferred = co_await m_socket.async_read_some(
					boost::asio::buffer(m_buffer),
					boost::asio::use_awaitable
				);

				std::string_view receivedData(m_buffer.data(), bytesTransferred);
				std::cout << "[Session] Received: " << receivedData << std::endl;

				co_await boost::asio::async_write(
					m_socket,
					boost::asio::buffer(m_buffer, bytesTransferred),
					boost::asio::use_awaitable
				);
			}
		}
		catch (const std::exception& e)
		{
			// 클라이언트 연결 종료 또는 에러 발생
			std::cerr << "[Session] Error: " << e.what() << std::endl;
			m_socket.close();
		}
	}
}