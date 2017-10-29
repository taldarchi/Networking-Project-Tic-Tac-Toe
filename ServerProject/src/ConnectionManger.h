

#ifndef LOGINMANGER_H_
#define LOGINMANGER_H_
#include "Dispatcher.h"
#include "MThread.h"
#include "TCPSocket.h"
#include <vector>
#include "MTCPListener.h"
#include "MessengerProtocol.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <fstream>
#include <tr1/functional>
#include <string>
#include <sstream>

#define DATABASE "database.txt"
#define SCOREBOARD "scoreboard.txt"

using namespace std;


class ConnectionManger : public MThread {
	Dispatcher * dispatcher;
	vector <TCPSocket *> peers;
	MTCPListener * listener;
	temp allUserList;
	temp data;
	bool running;
	pthread_mutex_t mutex;

	void removePeer(TCPSocket * peer);


public:
	ConnectionManger();
	temp listUsersInRoom(const string & roomName);
	void run();
	void addPeer(TCPSocket * socket);
	void close();
	void playerWin(TCPSocket * readypeer, string username);

	void addUserToDatabase(string userName,string password);
	void registerr(TCPSocket * readyPeer,string userName,string password);
	bool isNameExists(const string & userName);
	void login(TCPSocket * readyPeer,const string  userName,const string  password);
	bool verfiyUserAndPassword(const string & userName,const string & password);
	void userList();
	temp allUsersServer();
	void allRooms(temp & data);
	void allConnectedUsers(temp & data);
	temp p2pSessions();






	virtual ~ConnectionManger();
};

#endif /* CONNECTIONMANGER_H_ */
