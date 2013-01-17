#include "SimpleServer.h"

int main(int argc,char* argv[])
{
		SimpleServer* s=new SimpleServer("NamedPipeTester");
		s->Start();
		boost::this_thread::sleep(boost::posix_time::milliseconds(10000));
		delete s;
		system("pause");
	return 0;
}