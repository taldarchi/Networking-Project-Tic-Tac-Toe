

#ifndef SERVER_H_
#define SERVER_H_
#include "ConnectionManger.h"
#include "MThread.h"
#include "MessengerProtocol.h"
#include "TCPSocket.h"
#include "MTCPListener.h"


using namespace std;

class Server : public MThread{
	TCPSocket * socket;
	ConnectionManger * manager;
	bool running;
	temp data;
	void printList(temp & data);

public:
	Server();
	void run();
	void allUsers();
	void allP2PSessions();


	void connectedUsers();
	void close();
	virtual ~Server();
};

#endif /* SERVER_H_ */
