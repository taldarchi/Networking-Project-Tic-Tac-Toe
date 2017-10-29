

#ifndef USER_H_
#define USER_H_

#include "TCPSocket.h"



using namespace std;

class User {
	TCPSocket* socket;
	//string peerName;
	string userName;
	bool isconnectedToRoom;
	bool isconnectedToSession;
	User * peerSession;
	string roomName;

public:
	User(TCPSocket * peer,const string & userName);
	string getuserName();
	User * getPeerSession();
	TCPSocket * getSocket();
	//string getPeerName();
	bool isConnectedToRoom();
	bool isConnectedToSession();
	void setUserName(string userName);
	void setSocket(TCPSocket * socket);
	void setIsConnectedToRoom(bool flag);
	void setIsConnectedToPeer(bool flag);
	void setPeerSession(User * user);
	void setRoomName(const string & roomName);
	string getRoomName();
	void removeRoomName();
	void removePeerSession();

	//void setPeerName(string peerName);

	virtual ~User();

};

#endif /* USER_H_ */
