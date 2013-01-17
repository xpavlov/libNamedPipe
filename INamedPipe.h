#pragma once
#include <stdexcept>
#include <string>

	/*!
	* \brief
	* An interface for using namedpipes in a crossplatform way
	*/
	class INamedPipe
	{
	protected:
		///the pipe name
                std::string _name;
		///is this a server pipe?
		bool _server;
		/*!
		* \brief
		* A virtual function that connects to an existing pipe. client only
		* 
		* \throws std::exception
		*/
		virtual void connect()=0;
		/*!
		* \brief
		* A virtual function that creates a new pipe. server only
		* 
		* \throws std::exception
		*/
		virtual void open()=0;
		/*!
		* \brief
		* A default constructor for internal purposes;
		*/
		INamedPipe(){};
		virtual void  internalReadBytes(void* buf,size_t size)=0;
		virtual void internalWriteBytes(const void* buf,size_t size)=0;
		virtual void internalFlush()=0;
	public:

		INamedPipe(const std::string prefix, const std::string& name, bool server):_name(prefix),_server(server)
		{
			_name.append(name);
		}

		/*!
		* \brief
		* Creates server pipe or connects to an existing one
		* 
		* \throws std::exception
		*/
		void ConnectOrOpen()
		{
				if(_server)
					open();
				else
					connect();
		}
		/*!
		* \brief
		* Performs a read operation from pipe. Client only
		* 
		* \param buf
		* A pointer to store read data
		* 
		* \param size
		* The number of bytes to be read
		* 
		* \param has_extra_bytes
		* a pointer to bool value. It will be set into true if pipe has extra unread bytes after performing read operation
		* 
		* \returns
		* The number of read bytes
		* 
		* \throws std::exception
		*/
		virtual void ReadBytes(void* buf,size_t size)
		{
			if(!_server)
			{
				if(size<1)
					throw std::out_of_range("Size is 0 or less");
				internalReadBytes(buf,size);
			}
			else
				throw std::runtime_error("This operation is not supported on server pipe");
		}
		/*!
		* \brief
		* Performs a write operation to pipe. Client only
		* 
		* \param buf
		* A pointer to buffer with data to be written
		* 
		* \param size
		* The size of buffer

		* \returns
		* The number of written bytes
		* 
		* \throws std::exception
		*/
		virtual void WriteBytes(const void* buf,size_t size)
		{
			if(!_server)
			{
				if(size<1)
					throw std::out_of_range("Size is 0 or less");
				internalWriteBytes(buf,size);
			}
			else
				throw std::runtime_error("This operation is not supported on server pipe");
		}
		/*!
		* \brief
		* Flushes the pipe
		*/
		virtual void Flush()
		{
				internalFlush();
		}
		/*!
		* \brief
		* Closes the pipe
		*/
		virtual void Close()=0;
		/*!
		* \brief
		* Blocks current thread until new connection and returns new INamedPipe associatend with connected client
		*/
		virtual INamedPipe* WaitForConnection()=0;
		virtual INamedPipe* WaitForConnection(unsigned int timeout)=0;
		~INamedPipe(){};
	};

