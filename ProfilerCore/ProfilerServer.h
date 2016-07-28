#pragma once

#include "Concurrency.h"
#include "Message.h"
#include "Thread.h"
#include <thread>
#include <atomic>

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Socket;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Server
{
	InputDataStream networkStream;
	std::atomic<bool> running;

	static const int BIFFER_SIZE = 1024;
	char buffer[BIFFER_SIZE];

	std::unique_ptr<std::thread> acceptThread;
	std::unique_ptr<Socket> socket;

	CriticalSection lock;
	
	Server( short port );
	~Server();

	bool InitConnection();

	void AsyncAccept( );
	bool Accept();
public:
	void Send(DataResponse::Type type, OutputDataStream& stream = OutputDataStream::Empty);
	void Update();

	static Server &Get();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
