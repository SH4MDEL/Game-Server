#pragma once
#include "pch.h"
#include "IoContextPool.h"

namespace GameServer::Network
{
	class Server
	{
	public:
		Server(Core::IoContextPool& ioContextPool, uint16 port);
		void Start();

	private:
		boost::asio::awaitable<void> AcceptConnections();

	private:
		Core::IoContextPool& m_ioContextPool;
		boost::asio::ip::tcp::acceptor m_acceptor;
	};
}