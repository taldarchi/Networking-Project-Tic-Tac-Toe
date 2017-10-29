

#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include "MessengerProtocol.h"
#include <string.h>
#include <vector>
#include "MThread.h"
#include "UDPMessenger.h"
#include "unistd.h"
#include  "TCPSocket.h"
#include "UDPSocket.h"
#include "Game.h"

using namespace std;


class UDPClient{
public:
	string name;
	string ip;
	int port;
	void remove()
	{
		name.clear();
		ip.clear();
		port = 0;
	}

};

typedef vector<UDPClient *> Sessions;

class Client:public MThread {
	Sessions sessions;
	string userName;

	TCPSocket* tcpSocket;
	UDPMessenger* udpSocket;
	UDPClient sessionWithPeer;
	Game* game;
	char mySymbol;
	bool isMyTurn;
	bool isGameRunning;
	temp data;
	bool running;
	bool SessionActivePeer;
	bool gameAccepted;
	string nameOfOpponent;
	bool askingForAccept;
	bool askingForRematch;
	bool loggedIn;
	bool connected;//is user connected to server
	int command;
	temp::iterator iter;

	bool askForGame(const string & username, const string & ipAndPort);
	void PrintGameResult(char res);
	void getAllUsers(temp & data);
	void getAllConnctedUsers(temp & data);
	void sessionEstablished();
	void registrationError(const string & userName);
	void userVerified(temp data);
	void loginDenied();
	void sessionWithPeerEnded();
	void newPeerSessionRequest(const string & userName,const string & ipAndPort);
	void peerNotFound(const string & peerName);
	void serverClosed();
	void initGame(int x);
	void gameEnded();
	//*-------------------------------------------------*//
	// TODO Auto-generated destructor stub
	void printList(temp & data);

public:
	Client();
	void status();
	TCPSocket* getSocket(){return tcpSocket;}
	int getCommand(){
		return command;
	}

	bool isConnected();




	void clientAccepted();
	void clientDeclined();
	bool isSessionActivePeer();
	bool isLoggedIn();
	bool connect(const string & ip);
	//*-------------------------------------------*//

	void print();
	string getSessionName();

	/*For thread */
	void run();
	void startUDP(temp & data);


	void getRandomOpponent();
	void askForRegistredUsers();
	void askForConnectedUsers();
	void registerr(string userName,string  password);
	void login(string userName,string password);
	void openSessionWithPeer(string userName);
	void sendMesseage(string message);
	void closeSession();
	void disconnect();
	void close();
	virtual ~Client();
};

#endif /* MESSENGERCLIENTAPP_H_ */
