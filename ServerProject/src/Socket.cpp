#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "Socket.h"

using namespace std;


int Socket::bind(int port){
	struct sockaddr_in  s_in;
	bzero((char *) &s_in, sizeof(s_in));
	s_in.sin_family = (short)AF_INET;
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD any system ip*/
	s_in.sin_port = htons(port);

	int rc = ::bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));
	if (rc<0){
		cerr<<"Error naming channel1"<<endl;
	}
	return rc;
}

string Socket::fromAddr(){
	return inet_ntoa(peerAddr.sin_addr);
}

int Socket::connect(const string& ip, int port){
	// clear the s_in struct
	bzero((char *) &peerAddr, sizeof(peerAddr));  /* They say you must do this    */

	//sets the sin address
	peerAddr.sin_family = (short)AF_INET;
	peerAddr.sin_addr.s_addr = inet_addr(ip.data());
	peerAddr.sin_port = htons((u_short)port);
	int rc = ::connect(socket_fd, (struct sockaddr *)&peerAddr, sizeof(peerAddr));
	if (rc < 0)
	{
		perror ("Error establishing communications");
		close();
	}
	return rc;
}
