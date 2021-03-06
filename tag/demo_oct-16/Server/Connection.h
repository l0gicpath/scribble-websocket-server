#include <iostream>
#include <string>
#include "./common/SocketClass.h"
#include "./protocols/WSProtocol.h"

#ifndef CONNECTION_HEADER
#define CONNECTION_HEADER

class Connection {
	public:
		Connection(int, WSProtocol*);
		~Connection();
		
		int getSocket();
		int recv ( void * buffer, size_t len );
		int send ( const void * buffer, size_t len );
		int handshake ( std::string, WSAttributes * );
		std::string decode ( std::string );
		std::string encode ( std::string );
	private:
		SocketClass socket;
		WSProtocol * protocol;
};

#endif
