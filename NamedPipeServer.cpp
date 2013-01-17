#include "NamedPipeServer.h"

void NamedPipeServer::run()
{
	NamedPipe* client;
	_pipe=new NamedPipe(_name, 1);
	try
	{
		_pipe->ConnectOrOpen();
	}
	catch(const std::exception& e)
	{
	}
	while(_active)
	{
		try
		{
			client=_pipe->WaitForConnection(1000);
			if(client!=NULL)
				_clients.push(client);
		}
		catch(const std::exception& e)
		{
		}
		catch(...)
		{
		}
	}
	delete _pipe;
}

void NamedPipeServer::Start()
{
	_active=true;
	startWorkers();
}

void NamedPipeServer::workerProc()
{
	NamedPipe* client;
	while(_active)
	{
		if(_clients.try_pop(client))
		{
			handleClient(client);
		}
		else
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	}
}

void NamedPipeServer::startWorkers()
{
	for (size_t i = 0; i < _thread_pool_size; ++i)
	{
		boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&NamedPipeServer::workerProc, this)));
		_threads.push_back(thread);
	}
	boost::shared_ptr<boost::thread> dispatcher(new boost::thread(boost::bind(&NamedPipeServer::run,this)));
	_threads.push_back(dispatcher);
}

void NamedPipeServer::JoinWorkers()
{
	size_t size=_threads.size();
	for (std::size_t i = 0; i < size; ++i)
		_threads[i]->join();
	for (std::size_t i = 0; i < size; ++i)
		_threads[i].reset();
	_threads.clear();
}

void NamedPipeServer::Stop()
{
	_active=false;
	size_t size;
	if((size=_threads.size())>0)
	{
	this->JoinWorkers();
	}
}

NamedPipeServer::~NamedPipeServer(void)
{
	this->Stop();
	while(!_clients.empty())
	{
		if(_clients.try_pop(_pipe))
			delete _pipe;
	}
}
