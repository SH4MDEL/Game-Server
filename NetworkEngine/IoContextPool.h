#pragma once

namespace GameServer::Core
{
	class IoContextPool
	{
	public:
		explicit IoContextPool(uint32 poolSize);
		~IoContextPool();

		void Start();
		void Stop();

		boost::asio::io_context& GetIoContext();

	private:
		std::vector<std::shared_ptr<boost::asio::io_context>> m_ioContexts;
		std::vector<std::shared_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>> m_workGuards;
		std::vector<std::thread> m_threads;
		uint32 m_nextIoContextIndex = 0;
	};
}



