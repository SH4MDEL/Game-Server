#include "pch.h"
#include "IOContextPool.h"

namespace GameServer::Core
{
	IoContextPool::IoContextPool(uint32 poolSize)
	{
		if (poolSize == 0)
		{
			// 하드웨어 동시성 개수만큼 생성
			poolSize = std::thread::hardware_concurrency();
		}

		m_ioContexts.reserve(poolSize);
		for (uint32 i = 0; i < poolSize; ++i)
		{
			auto context = std::make_shared<boost::asio::io_context>();
			m_ioContexts.push_back(context);
			// IO Context가 작업이 없어도 종료되지 않도록 Guard 생성
			m_workGuards.push_back(std::make_shared<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(context->get_executor()));
		}
	}

	IoContextPool::~IoContextPool()
	{
		Stop();
	}

	void IoContextPool::Start()
	{
		for (auto& context : m_ioContexts)
		{
			m_threads.emplace_back([context]() {
				context->run();
				});
		}
	}

	void IoContextPool::Stop()
	{
		for (auto& context : m_ioContexts)
		{
			context->stop();
		}

		for (auto& thread : m_threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}

		m_ioContexts.clear();
		m_workGuards.clear();
	}

	boost::asio::io_context& IoContextPool::GetIoContext()
	{
		// Round-Robin 방식으로 부하 분산
		boost::asio::io_context& context = *m_ioContexts[m_nextIoContextIndex];
		m_nextIoContextIndex = (m_nextIoContextIndex + 1) % static_cast<uint32>(m_ioContexts.size());
		return context;
	}
}