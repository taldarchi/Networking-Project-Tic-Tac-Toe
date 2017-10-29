

#include "Dispatcher.h"

#include <pthread.h>
#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
Dispatcher::Dispatcher() {
	running=false;
	pthread_mutex_init(&mutex,NULL);//initalazation for mutex

}

void Dispatcher::run(){

	int command;
	while(running){
		MTCPListener listener;
		listener.addSockets(getConnectedPeersSockets());
		TCPSocket * readyPeer=listener.listenToSocket(3);
		if(readyPeer!=NULL){
			if(!MessengerProtocol::readFromSocket(command,data,readyPeer)){
				running = false;
				break;
			}
			User * user=findPeerBySocket(readyPeer);
			temp::iterator iter=data.begin();
			if(command==0)
				continue;
			cout << command << endl;
			switch (command){
			case OPEN_SESSION_WITH_PEER:
				openSessionWithPeer(user,iter->second);
				break;
			case PEER_ACCEPTED_GAME:
				confirmGameWithPeer(user,iter->second);
				break;
			case CLOSE_SESSION_WITH_PEER:
				closeSessionWithPeer(user);
				break;
			case RANDOM_USER_FOR_GAME:
				getRandomUser(user);
				break;
			case CONNECTED_USERS:
				ConnectedUsers(user);
				break;
			case ALL_USERS:
				listAllUsers(user);
				break;
			case GAME_ENDED:
				gameEnded(user,iter->second);
				break;
			case PLAYER_WIN:
				playerWin(user,iter->first);
			case EXIT:
			default:
				disconnectPeer(user);
				break;
			}
		}
	}
}

void Dispatcher::playerWin(User* user,const string & username)
{
	    cout << "HALLO" << username << endl;
		string line;
		ifstream myFile("scoreboard.txt");
		ofstream tempFile("temp.txt");
		string strTemp;
		while(myFile >> strTemp)
		{
			if(strTemp.substr(0,username.length()).compare(username) == 0)
			{
				int n = atoi(strTemp.substr(username.length()+2,(strTemp.length()-username.length())-1).c_str());
				n = n+1;
				stringstream stream;
				stream << n;
				strTemp = username+":"+stream.str();
			}
			strTemp +="\n";
			cout << strTemp << endl;
			tempFile << strTemp;
		}
		remove("scoreboard.txt");
		rename("temp.txt","scoreboard.txt");
}
void Dispatcher::gameEnded(User* user, const string & username)
{
	map<string,User*>::iterator iter=connectedUsers.find(username);
	if(iter ==connectedUsers.end())
	{
		cout << "HERE" << endl;
	}
	else{
	MessengerProtocol::insertDataToSend(data,user->getuserName(),user->getSocket()->destIpAndPort());
	MessengerProtocol::sendToSocket(GAME_ENDED,data,iter->second->getSocket());
	}
}
void Dispatcher::getRandomUser(User * user)
{
	srand(time(NULL));
	int length = connectedUsers.size();
	if(length == 0)
	{
		MessengerProtocol::sendToSocket(PEER_NOT_FOUND,data,user->getSocket());
		return;
	}
	int randomNum = rand() % length;
	map<string,User*>::iterator iter = connectedUsers.begin();
	for(int i = 0; i < randomNum; i++ )
	{
		iter++;
	}

	if(iter != connectedUsers.end())
	{
		if(iter->second->getuserName() == user->getuserName())
			{
				getRandomUser(user);
				return;
			}
		MessengerProtocol::insertDataToSend(data,iter->second->getuserName(),"");
		MessengerProtocol::sendToSocket(RANDOM_USER_FOR_GAME,data,user->getSocket());
	}
	else
	{
		MessengerProtocol::sendToSocket(PEER_NOT_FOUND,data,user->getSocket());
	}
}
void Dispatcher::confirmGameWithPeer(User * user,const string & userName)
{
	cout <<  user -> getuserName() << endl;
	cout << userName << endl;
	map<string,User*>::iterator iter=connectedUsers.find(userName);
	if (iter == connectedUsers.end()){

			MessengerProtocol::sendToSocket(PEER_NOT_FOUND,data,user->getSocket());
			return;
		}
		if(iter->second->isConnectedToSession()==true )
		{
			MessengerProtocol::sendToSocket(SESSION_REFUSED,data,user->getSocket());
			return;
		}
	MessengerProtocol::insertDataToSend(data,user->getuserName(),user->getSocket()->destIpAndPort());
	MessengerProtocol::sendToSocket(PEER_ACCEPTED_GAME,data,iter->second->getSocket());
	cout<<"Session between "<<user->getuserName()<<" and "<<iter->second->getuserName()<<" established"<<endl;
}

void Dispatcher::listAllUsers(User * user){
	MessengerProtocol::sendToSocket(ALL_USERS,registredUsers,user->getSocket());
}

void Dispatcher::AllOnlineUsersServer(temp & data){
	map<string,User*>::iterator a= connectedUsers.begin();
	map<string,User*>::iterator b= connectedUsers.end();
	data.clear();
	for(;a!=b;a++){
		data.insert(pair<string,string>(a->first,""));
	}

}

void Dispatcher::ConnectedUsers(User * user){

	map<string,User*>::iterator a= connectedUsers.begin();
	map<string,User*>::iterator b= connectedUsers.end();
	for(;a!=b;a++){
		data.insert(pair<string,string>(a->first,""));
	}
	MessengerProtocol::sendToSocket(CONNECTED_USERS,data,user->getSocket());
	cout<<"Send connected users"<<endl;

}


//Disconnect user from server.
void Dispatcher::disconnectPeer(User * user){

	//Remove the user from connected users map.
	lock();
	connectedUsers.erase(user->getuserName());
	unlock();
	cout<<"User :"<<user->getuserName()<<" disconnected"<<endl;


}

User * Dispatcher::findPeerBySocket(TCPSocket * peer){


	map<string,User * >::iterator iter=connectedUsers.begin();
	map<string,User * >::iterator eiter=connectedUsers.end();
	for(;iter!=eiter;iter++){
		if(iter->second->getSocket()==peer)
			return iter->second;

	}
	return NULL;
}


//Get the connected users sockets for listening.
vector<TCPSocket*> Dispatcher::getConnectedPeersSockets(){
	vector<TCPSocket*> peers;
	map<string,User*>::iterator iter=connectedUsers.begin();
	map<string,User*>::iterator eiter=connectedUsers.end();
	for (;iter!=eiter;iter++){
		peers.push_back(iter->second->getSocket());
	}
	return peers;
}

//Add user to connected map.
void Dispatcher::addUserToConnected(TCPSocket * peer,string userName){
	//Create new user
	User * newUser=new User(peer,userName);
	//Add the user to the connected map.
	lock();
	connectedUsers.insert(pair<string,User *>(userName,newUser));
	unlock();
	//Start the tread.
	if(running==false){
		running=true;
		start();
	}
}

//Close session with peer. Done
void Dispatcher::closeSessionWithPeer(User * user){
	User * user2=user->getPeerSession();
	MessengerProtocol::sendToSocket(CLOSE_SESSION_WITH_PEER,data,user2->getSocket());
	MessengerProtocol::sendToSocket(CLOSE_SESSION_WITH_PEER,data,user->getSocket());
	//Set the users status.
	user->setIsConnectedToPeer(false);
	user->removePeerSession();
	user2->setIsConnectedToPeer(false);
	user2->removePeerSession();
	//Remove the session from the session database

	temp::iterator iter=p2pSessions.begin();
	temp::iterator eiter=p2pSessions.end();
	for(;iter!=eiter;iter++){
		if(iter->first==user->getuserName() || iter->second==user->getuserName()){
			p2pSessions.erase(iter);
		}
	}


}


//Open session with peer.
void Dispatcher::openSessionWithPeer(User * user,const string & userName){
	map<string,User*>::iterator iter=connectedUsers.find(userName);
	if (iter == connectedUsers.end()){

		MessengerProtocol::sendToSocket(PEER_NOT_FOUND,data,user->getSocket());
		return;
	}
	if(iter->second->isConnectedToSession()==true )
	{
		MessengerProtocol::sendToSocket(SESSION_REFUSED,data,user->getSocket());
		return;
	}
	MessengerProtocol::insertDataToSend(data,user->getuserName(),user->getSocket()->destIpAndPort());
	MessengerProtocol::sendToSocket(OPEN_SESSION_WITH_PEER,data,iter->second->getSocket());
	p2pSessions.insert(pair<string,string>(user->getuserName(),iter->second->getuserName()));
}



void Dispatcher::close(){
	if(running){
		running=false;
		removeAllUsers();
		connectedUsers.clear();
		waitForThread();
	}


}

Dispatcher::~Dispatcher(){

}




void Dispatcher::removeAllUsers(){
	map<string,User *>::iterator iter=connectedUsers.begin();
	map<string,User *>::iterator eiter=connectedUsers.end();
	for (;iter!=eiter;iter++)
	{
		if(iter->second!=NULL){
			MessengerProtocol::sendToSocket(MESSENGER_CLOSED,data,iter->second->getSocket());
			delete iter->second;
		}
	}
	connectedUsers.clear();
}

void Dispatcher::lock(){
	pthread_mutex_lock(&mutex);
}
void Dispatcher::unlock(){
	pthread_mutex_unlock(&mutex);
}


void Dispatcher::updateRegistredUsers(temp userList){
	registredUsers=userList;
}


temp Dispatcher::getP2PSessions(){
	return p2pSessions;
}
