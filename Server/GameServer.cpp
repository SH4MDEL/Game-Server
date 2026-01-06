#include "pch.h"
#include "GameServer.h"
#include "Server.h"

int main()
{
	try
	{
		// 1. Thread Pool 초기화
		GameServer::Core::IoContextPool ioContextPool(4); // 4 Threads

		// 2. 서버 구동
		GameServer::Network::Server server(ioContextPool, 8080);
		server.Start();

		std::cout << "Game Server Started on port 8080..." << std::endl;

		// 3. 쓰레드 풀 실행 (Blocking)
		ioContextPool.Start();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Fatal Error: " << e.what() << std::endl;
	}

	return 0;
}