#pragma once
#ifndef _WIN32
#include "INamedPipe.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <exception>
#include <sys/un.h>

	class PosixNamedPipe :
		public INamedPipe
	{
	private:
                int sock;
		sockaddr_un desc;
	protected:
		void connect();
		void open();
		void internalReadBytes(void* buf,size_t size);
		void internalWriteBytes(const void* buf,size_t size);
		void internalFlush();
	public:
		void Close();
		PosixNamedPipe* WaitForConnection();
		PosixNamedPipe* WaitForConnection(unsigned int timeout);
		PosixNamedPipe(int pipe);
		PosixNamedPipe(const std::string& name,bool server);
		virtual ~PosixNamedPipe();
	};
#endif
