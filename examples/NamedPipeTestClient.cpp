#include "../NamedPipe.h"
#include <boost/thread.hpp>


void spamming_thread()
{
	std::vector<std::string> words;
	words.push_back(std::string("one "));
	words.push_back(std::string("two "));
	words.push_back(std::string("three "));
	words.push_back(std::string("four "));
	words.push_back(std::string("five "));
	words.push_back(std::string("six "));
	words.push_back(std::string("seven "));
	words.push_back(std::string("eight "));
	words.push_back(std::string("nine "));
	words.push_back(std::string("ten "));

	NamedPipe client("NamedPipeTester",0);
	try
	{
		client.ConnectOrOpen();
		for(int i=0;i<words.size();++i)
		{
			std::cout<<"sending "<<words[i]<<"\n";
			size_t size=words[i].size();
			client.WriteBytes(&size,sizeof(size));
			client.WriteBytes(words[i].data(),size);
		}
		client.Close();
	}
	catch(const std::runtime_error &e)
	{
		std::cout<<"Exception: "<<e.what()<<"\n";
	}
}

int main(int argc,char* argv[])
{
	try
		{
			for(;;)
			{
				boost::thread* t=new boost::thread(spamming_thread);
				boost::thread* q=new boost::thread(spamming_thread);
				t->join();
				q->join();
				delete t;
				delete q;
			}
		}
		catch(const std::exception &e)
		{
			std::cout<<"Exception!:"<<e.what()<<"\n";
		}
	return 0;
}