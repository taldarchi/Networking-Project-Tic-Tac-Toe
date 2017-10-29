

#include "User.h"

User::User(TCPSocket * peer,const string & userName) {
	this->socket=peer;
	this->userName=userName;
	isconnectedToRoom=false;
	isconnectedToSession=false;
	roomName.clear();
	peerSession=NULL;


}

string User::getuserName(){
	return userName;
}

TCPSocket * User::getSocket(){
	return socket;
}

bool User::isConnectedToRoom(){
	return isconnectedToRoom;
}


bool User::isConnectedToSession(){
	return isconnectedToSession;
}


void User::setUserName(string userName){
	this->userName=userName;
}

void User::setSocket(TCPSocket * socket){
	this->socket=socket;
}

void User::setIsConnectedToRoom(bool flag){
	isconnectedToRoom=flag;
}




void User::setIsConnectedToPeer(bool flag){
	isconnectedToSession=flag;
}

void User::removePeerSession(){
	peerSession=NULL;
}




User * User::getPeerSession(){
	return peerSession;
}




void User::setPeerSession(User * user){
	peerSession=user;
}

void User::setRoomName(const string & roomName)
{
	string a=roomName;
	this->roomName=a;
}

string User::getRoomName()
{
	return roomName;
}



void User::removeRoomName()
{
	roomName.clear();
}

User::~User() {
	if(socket){
		socket->close();
	delete socket;
	socket=NULL;
	}
	//string peerName;
	userName.clear();
	isconnectedToRoom=false;
	isconnectedToSession=false;
	peerSession=NULL;
	roomName.clear();

}

