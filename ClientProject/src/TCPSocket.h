/*

 * TCPSocket.h
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include "Socket.h"

using namespace std;



class TCPSocket:public Socket{
	struct sockaddr_in serverAddr;

private:
	/**
	 * private constructor to create a secondary server socket to communicate with a remote peer
	 */
	TCPSocket(int connected_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr);

public:
	/**
	 * Constructor create a TCP server socket
	 */
	TCPSocket(int port);

	/**
	 * Constructor creates TCP client socket
	 */
	TCPSocket(const string& peerIp, int port);
	/**
	 * Perform listen and accept on server socket
	 */
	TCPSocket* listenAndAccept();

	string destIpAndPort();
};


#endif
