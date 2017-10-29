

#ifndef DISPATCHER_H_
#define DISPATCHER_H_
#include "MThread.h"
#include <vector>
#include <map>
#include "User.h"
#include "MessengerProtocol.h"
#include "MTCPListener.h"
#include "TCPSocket.h"
#include <time.h>
#include <stdlib.h>

using namespace std;
class Dispatcher : public MThread {

	//
	map<string,User*> connectedUsers;

	temp registredUsers;
	temp p2pSessions;
	bool running;
	temp data;
	pthread_mutex_t mutex;

	void disconnectPeer(User * user);
	User * findPeerBySocket(TCPSocket * peer);
	void openSessionWithPeer(User * user,const string & userName);
	void closeSessionWithPeer(User * user);
	vector<TCPSocket*> getConnectedPeersSockets();
	void playerWin(User* user,const string & username);
	void listAllUsers(User * user);
	void acceptGame(User * user);
	void confirmGameWithPeer(User * user, const string & userName);
	void getRandomUser(User * user);
	void gameEnded(User* user,const string & username);
	void removeAllUsers();

public:
	Dispatcher();
	void addUserToConnected(TCPSocket * peer,string userName);


	void run();
	void close();
	void lock();
	void unlock();




	void ConnectedUsers(User * user);
	void AllOnlineUsersServer(temp & data);
	void updateRegistredUsers(temp userList);
	temp getP2PSessions();


	virtual ~Dispatcher();
};

#endif /* DISPATCHER_H_ */
