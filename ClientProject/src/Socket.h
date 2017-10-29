#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include "FileInter.h"

using namespace std;


class Socket: public FileInter{
protected:
	struct sockaddr_in peerAddr;

public:
	int bind(int port);
	/**
	 * return the address of the connected peer
	 */
	virtual string fromAddr();
	virtual int connect(const string& ip, int port);
};


#endif
