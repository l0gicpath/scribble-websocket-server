#include "ConnectionsWaiting.h"

ConnectionsWaiting::ConnectionsWaiting() {
	
}

ConnectionsWaiting::~ConnectionsWaiting() {

}

void ConnectionsWaiting::lock ( ) {
	pad.lock();
}

void ConnectionsWaiting::unlock ( ) {
	pad.unlock();
}

void ConnectionsWaiting::insert ( int conn ) {
	connectionsWaiting.push_back ( conn );
}

int ConnectionsWaiting::get ( ) {
	int desc = connectionsWaiting.front ( );
	connectionsWaiting.pop_front ( );
	return desc;
}	

void ConnectionsWaiting::signal ( ) {
	sem.post();
}

void ConnectionsWaiting::wait ( ) {
	sem.wait();
}
