
#ifndef UDPMESSENGER_H_
#define UDPMESSENGER_H_
#include "MThread.h"
#include <MessengerProtocol.h>
#include "UDPSocket.h"
#include <string>
using namespace std;


class UDPMessenger: public MThread {
public:
	UDPSocket* socket;
	int port;
	bool running;
	Game* game;
	char mySymbol;
	UDPMessenger(int port);
	void initGame(Game* g,char symb);
	void run();
	void sendTo(string message,string ip,int port);
	void reply(string message);
	void close();
	virtual ~UDPMessenger();
};

#endif /* UDPMESSENGER_H_ */
