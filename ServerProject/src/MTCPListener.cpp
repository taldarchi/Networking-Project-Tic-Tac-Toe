#include "MTCPListener.h"

using namespace std;



void MTCPListener::addSocket(TCPSocket* socket){
	sockets.push_back(socket);
}

void MTCPListener::addSockets(vector<TCPSocket*> socketVec){
	sockets = socketVec;
}

TCPSocket* MTCPListener::listenToSocket(int timeout){
	struct timeval tv = {timeout, 0};
	tSocketsContainer::iterator iter = sockets.begin();
	tSocketsContainer::iterator endIter = sockets.end();
	fd_set fdset;
	FD_ZERO(&fdset);
	int highfd = 0;
	//fill the set with file descriptors
	for (;iter != endIter;iter++) {
		highfd++;
		FD_SET((*iter)->getSocketFd(), &fdset);
		//if ((*iter)->getSocketFid()>highfd) highfd = (*iter)->getSocketFid();
	}

	//perform the select
	int returned;
	if (timeout>0){
		returned = select(sizeof(fdset)*8, &fdset, NULL, NULL, &tv);
	}else{
		returned = select(sizeof(fdset)*8, &fdset, NULL, NULL, NULL);
	}
	if (returned) {
		for (int i = 0; i < highfd; i++) {
			TCPSocket* tmpSocket = sockets[i];
			if (FD_ISSET(tmpSocket->getSocketFd(), &fdset)) {
				return tmpSocket;
			}
		}
	}
	return NULL;
}
