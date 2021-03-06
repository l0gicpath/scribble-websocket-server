#include "Channel.h"

Channel::Channel ( std::string name, int maxConnections ) {
	this->name = name;	
	this->maxConnections = maxConnections;
	this->eventsList = new epoll_event[this->maxConnections];
	ThreadClass::Start(this);
}

Channel::~Channel ( ) {
	
}

void Channel::Setup() {
	try{		
		//Log ( "Channel " + this->name + " is being setup" );
		eventFD = epoll_create(10);//10 Doesn't mean anything after Linux Kern 2.6.8
		if ( eventFD<0 ) {
			  throw LogString("Unable to create an epoll Pipe");
		}
	}catch ( LogString e ) {
		Log ( "Listener: " + e );
		std::exit ( 0 );
	}
}

void Channel::Execute (void * arg) {
	try {
		WSProtocol * protocol = new RFC_6455();
		Log ( "Channel " + this->name + " ready..." );
		for ( ;; ) {
			eventsOccuring = epoll_wait(eventFD, eventsList, 10, -1);
	
			if(eventsOccuring < 0){
				  throw LogString("Unable to wait error occured");
			}
	
			for(int ce = 0; ce < eventsOccuring; ce++){
				int buffer_len = 0;
				char buffer[1024];
				if((buffer_len=recv(eventsList[ce].data.fd,buffer,sizeof(buffer)-1,0))==0){
					Log ( "User disconnected" ); 
					//removeConnection(eventsList[ce].data.fd);
					continue;
				}else{
					
					std::string decodedMsg = protocol->decode(buffer);
					std::string encodedMsg = protocol->encode(decodedMsg);
					broadcast ( encodedMsg.c_str() , encodedMsg.length(), eventsList[ce].data.fd );
				}
			}
		}
	}catch(LogString e) {
		Log ( "Listener: " + e );		
		std::exit(0);
	}
}

void Channel::broadcast ( const void * buffer, size_t len, int exception) {
	std::list<SocketClass*>::iterator it;
	for ( it = connections.begin(); it != connections.end(); it ++ ) {
		if ( (*it)->getSocket() != exception ){
			(*it)->send ( buffer, len );
		}
	}
}

void Channel::addConnection ( SocketClass * desc ) {
	try{
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = desc->getSocket();
		if(epoll_ctl(eventFD, EPOLL_CTL_ADD, desc->getSocket(), &ev)<0) {
			throw LogString("Unable to add new connection");
		}
		connections.push_back ( desc );
	}catch(LogString e){
		Log("Listener: " + e);
	}

}
