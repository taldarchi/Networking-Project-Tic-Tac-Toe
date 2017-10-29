
#include "Server.h"

Server::Server() {
	manager=new ConnectionManger();
	socket=NULL;
	running=false;
	start();

}


void Server::run(){
	running=true;
	socket=new TCPSocket(MSNGR_PORT);
	while(running)
	{
		TCPSocket * peer=socket->listenAndAccept();
		if(!peer)
			continue;
		else{
			cout<<"new peer connected: "<<peer->destIpAndPort()<<endl;
			manager->addPeer(peer);
		}
	}
}

void Server::printList(temp & data){
	temp::iterator iter=data.begin();
	temp::iterator eiter=data.end();
	for(;iter!=eiter;iter++){
		cout<<iter->first<<endl;
	}

}


void Server::allUsers(){
	data.clear();
	data=manager->allUsersServer();
	if(data.size()==0){
		cout<<"No users"<<endl;
		return;
	}
	printList(data);
}


void Server::connectedUsers(){
	data.clear();
	manager->allConnectedUsers(data);
	if(data.size()==0){
		cout<<"No users"<<endl;
		return;
	}
	printList(data);
	cout<<endl;
}

void Server::allP2PSessions(){
	data.clear();
	data=manager->p2pSessions();
	if(data.size()==0){
		cout<<"No p2p sessions"<<endl;
		return;
	}
	temp::iterator iter=data.begin();
	temp::iterator eiter=data.end();
	for(;iter!=eiter;iter++){
		cout<<"<"<<iter->first<<","<<iter->second<<">"<<endl<<endl;
	}
}

void Server::close(){
	if(running)
	{
		running=false;
		if(manager)
			manager->close();
		socket->close();
		waitForThread();
		delete socket;
		socket=NULL;
	}

	cout<<"Server closed"<<endl;
}


Server::~Server() {
	// TODO Auto-generated destructor stub
}

