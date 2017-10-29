/*
 * TCPSocket.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */


#include "TCPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
using namespace std;


TCPSocket::TCPSocket(int connected_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr){
	this->serverAddr =  serverAddr;
	this->peerAddr =  peerAddr;
	socket_fd = connected_sock;
}


TCPSocket::TCPSocket(int port){
	socket_fd = socket (AF_INET, SOCK_STREAM, 0);

	if (bind(port)<0)
	{
		cout<<"Error naming channel"<<endl;
	}
}


TCPSocket::TCPSocket(const string& peerIp, int port){
	socket_fd = socket (AF_INET, SOCK_STREAM, 0);

	if (connect(peerIp,port) < 0)
	{
		cout<<"Error establishing communications"<<endl<<flush;
		close();
	}
}


TCPSocket* TCPSocket::listenAndAccept(){
	int rc = listen(socket_fd, 1);

	if (rc<0){
		return NULL;
	}
	socklen_t len = sizeof(peerAddr);
	bzero((char *) &peerAddr, sizeof(peerAddr));

	int connect_sock = accept(socket_fd, (struct sockaddr *)&peerAddr, &len);
	return new TCPSocket(connect_sock,serverAddr,peerAddr);
}

string TCPSocket::destIpAndPort(){
	string str = inet_ntoa(peerAddr.sin_addr);
	str.append(":");
	char buff[10];
	sprintf(buff,"%d",ntohs(peerAddr.sin_port));
	str.append(buff);
	return str;
}



