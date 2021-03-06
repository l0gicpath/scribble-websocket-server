#include "ListenerClass.h"
#include "./common/Logger.h"
#include <cstdio>


ListenerClass::ListenerClass ( int port, int queue, int selectors ) {
	int i;	
	maxSelectors = selectors;
	channels.insert ( channels.begin(), std::pair <std::string, Channel* >("example", new Channel ( "Example","logictests/example.lua", 10 ) ) ); //Example channel
	channels.insert ( channels.begin(), std::pair <std::string, Channel* >("default", new Channel ( "Default","logictests/default.lua", 10 ) ) ); //Default channel
	channels.insert ( channels.begin(), std::pair <std::string, Channel* >("spritemoving", new Channel ( "Sprite Moving","logictests/spritemoving.lua", 10 ) ) ); //Sprite Moving channel
	

	chselect = (ChannelSelector**) malloc ( sizeof(ChannelSelector*) * maxSelectors );
	for ( i = 0; i < maxSelectors; i ++ ) {
		chselect[i] = new ChannelSelector ( &connectionsWaiting, channels );
	}
	
	listenerSocket = new SocketClass ( );
	listenerSocket->bindTo(port);
	listenerSocket->listenTo(queue);
	
	ThreadClass::Start(this);
	Log("Listener: Starting");
}

ListenerClass::~ListenerClass ( ) {
	int i;
	for ( i = 0; i < maxSelectors; i ++ ) {
		chselect[i]->Wait();
		delete chselect[i];
	}		
	delete listenerSocket;
}

void ListenerClass::handleConnection(int desc){
	try{
		//Create special socket for protocol version
		//Send it to the waiting line so it can be sent to the correct channel.
		connectionsWaiting.lock ();
		connectionsWaiting.insert ( desc );
		connectionsWaiting.unlock ();		
		connectionsWaiting.signal ();
	}catch(LogString e){
		Log("Listener: "+ e);
	}
}

void ListenerClass::Setup(){
	try{
	}catch(LogString e){
		Log("Listener: " + e);
		std::exit(0);
	}
}

void ListenerClass::Execute(void * arg){
	try{
		Log("Listener: Started");
		for(;;){
			Log("Listener: New Connection");
			incomingFD = accept( listenerSocket->getSocket(), 
					     (sockaddr *)&listenerSocket->tempInfo, 
					     &listenerSocket->tempLen );
			handleConnection(incomingFD);
		}
	}catch(LogString e){
		Log("Listener: " + e);
		std::exit(0);
	}
}


