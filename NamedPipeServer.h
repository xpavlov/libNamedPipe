#pragma once

#include "NamedPipe.h"
#include <vector>
#include <boost/thread.hpp>
#include <tbb/concurrent_queue.h>


/*!
* \brief
* This class contains the main functions to make an multithreaded NamedPipe server
* 
* Inherit from it and override the handleClient(INamedPipe* client) function to make your own server
*/
class NamedPipeServer
{
protected:
	///the server pipe name 
	std::string _name;
	///the number of threads handling client connections 
	size_t _thread_pool_size;
	/// the pointer to server pipe
	NamedPipe* _pipe;
	///activity. change to false in order to make all threads stop
	bool volatile _active;
	///this queue stores the pointers to connected clients
	tbb::concurrent_queue<NamedPipe*> _clients;
	///this vector stores all threads serving clients
	std::vector<boost::shared_ptr<boost::thread> > _threads;
	/*!
	* \brief
	* Starts the main loop (waits for new clients and puts them into queue)
	*/
	void run();
	/*!
	* \brief
	* Starts all worker theads
	*/
	void startWorkers();
	/*!
	* \brief
	* Pops the client from queue and passes it to handleClient()
	*/
	void workerProc();
	/*!
	* \brief
	* This function must be overriden. It is intended for handling data from clients;
	*/
	virtual void handleClient(NamedPipe* client)=0; //Override this to make your own protocol
public:
	/*!
	* \brief
	* Starts the server and all worker theads. You must create a new thread for this func.
	*/
	void Start();
	/*!
	* \brief
	* Stops all worker threads and main thread - run()
	*/
	void Stop();
	/*!
	* \brief
	* Joins all worker theads
	*/
	void JoinWorkers();
	/*!
	* \brief
	* Write brief comment for NamedPipeServer here.
	* 
	* \param name
	* The name for server pipe
	* 
	* \param thread_pool_size
	* the number of threads for handling clients connection
	*/
	NamedPipeServer(const std::string& name, size_t thread_pool_size):_name(name),_thread_pool_size(thread_pool_size){}
	virtual ~NamedPipeServer();
};