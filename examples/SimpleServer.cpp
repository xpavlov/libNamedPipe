#include "SimpleServer.h"
#include <iostream>

void SimpleServer::handleClient(NamedPipe* client)
{
	for(int i=0;i<10;++i)
	{
		try
		{
			size_t size;
			client->ReadBytes(&size,sizeof(size));
			if(size>0)
			{
				char* message=new char[size];
				client->ReadBytes(message,size);
				//Using std::cout is bad in multi-threading apps
				//std::string msg(message,size);
				//std::cout<<"Message from pipe: "<<msg<<"\n";
				delete[] message;
			}
		}
		catch(const std::exception& e)
		{
			std::cout<<"Exception!:"<<e.what()<<"\n";
		}
	}
	client->Close();
}
