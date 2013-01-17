#ifndef _WIN32
#include "PosixNamedPipe.h"
#include <errno.h>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>


#define THROW_ERROR(X) std::stringstream s;					\
	s<<X<<strerror(errno);									\
	throw std::runtime_error(s.str());



PosixNamedPipe::PosixNamedPipe(const std::string& name,bool server):INamedPipe("/tmp/",name,server)
{
	memset(&desc, 0, sizeof(struct sockaddr_un));
}

PosixNamedPipe::PosixNamedPipe(int pipe)
{
	sock=pipe;
	_server=false;
	memset(&desc, 0, sizeof(struct sockaddr_un));
}

void PosixNamedPipe::open()
{
	sock= socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock == -1) {
		THROW_ERROR("Create_socket failed: ");
	}
	unlink(_name.c_str());
	desc.sun_family = AF_UNIX;
	strcpy(desc.sun_path, _name.c_str());
	if (bind(sock, (sockaddr*)&desc, sizeof(struct sockaddr_un)) == -1) {
		THROW_ERROR("Connection failed(bind): ");
	}
	if (listen(sock,SOMAXCONN) == -1) {
		THROW_ERROR("Connection failed(listen): ");
	}
}

void PosixNamedPipe::connect()
{
	sock= socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock == -1)
	{
		THROW_ERROR("Create_socket failed: ");
	}
	desc.sun_family = AF_UNIX;
	strcpy(desc.sun_path, _name.c_str());
	if (::connect(sock, (sockaddr*)&desc, sizeof(struct sockaddr_un)) == -1)
	{
		THROW_ERROR("Connection failed(connect): ");
	}
}

void PosixNamedPipe::internalReadBytes(void* buf,size_t size)
{
	if ((recv(sock, buf, size, MSG_WAITALL)) == -1) {
		THROW_ERROR("Error while reading: ");
	}
}

void PosixNamedPipe::internalWriteBytes(const void* buf,size_t size)
{
	size_t ret=-1;
	if ((ret = send(sock, buf, size, 0)) == -1||ret!=size) {
		THROW_ERROR("Error while sending: ");
	}
}

void PosixNamedPipe::internalFlush()
{
}

void PosixNamedPipe::Close()
{
	if(_server)
		unlink(desc.sun_path);
	close(sock);
}

PosixNamedPipe* PosixNamedPipe::WaitForConnection()
{
	int client=accept(sock,NULL,NULL);
	if(client!=-1)
		return new PosixNamedPipe(client);
	else {
		THROW_ERROR("Accept error: ");
	}
}

PosixNamedPipe* PosixNamedPipe::WaitForConnection(unsigned int timeout)
{ 
	int nsock;              
	int retour;             
	fd_set readf;          
	fd_set writef;           
	struct timeval to;      

	FD_ZERO(&readf);
	FD_ZERO(&writef);
	FD_SET(sock, &readf);
	FD_SET(sock, &writef);
	to.tv_usec = timeout*1000;

	retour = select(sock+1, &readf, &writef, 0, &to);

	if (retour == 0)  
	{
		return NULL;
	}

	if ( (FD_ISSET(sock, &readf)) || (FD_ISSET(sock,&writef))) 
	{
			nsock = accept(sock, NULL, NULL);
			return new PosixNamedPipe(nsock);
	}
	else
	{
		throw std::runtime_error("invalid socket descriptor!\n");
	}
}

PosixNamedPipe::~PosixNamedPipe()
{
	Close();
}

#endif
