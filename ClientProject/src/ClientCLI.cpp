
#include "MThread.h"
#include "Client.h"
#include <iostream>

using namespace std;


void printInstructions(){
	cout<<"To connect to server type: c <server ip>                         "<<endl;
	cout<<"To play a game with peer type: play <user name>                  "<<endl;
	cout<<"To play a game against a a random opponent type: random         "<<endl;
	cout<<"Opening session will close any previously active sessions        "<<endl;
	cout<<"To see your status type: l                                       "<<endl;
	cout<<"To register type: register <username password>                  "<<endl;
	cout<<"To login type: login <username password>                         "<<endl;
	cout<<"To see list of all users type: lu                                "<<endl;
	cout<<"To see list of online users type: lcu                            "<<endl;
	cout<<"To send message type: s <message>                                "<<endl;
	cout<<"To close opened session cs              						    "<<endl;
	cout<<"To disconnect from server type: d                                "<<endl;
	cout<<"To exit type: x                                                  "<<endl;
}

int main() {

	printInstructions();
	string command,userName,password,peerName,message;
	Client * messenger=new Client();
	while(true){
		command.clear();
		cin>>command;
		if(command=="c")
		{
			if(messenger->isConnected()==true){
				cout<<"You are already connected to server "<<endl;
				continue;
			}
			else{
			string serverIp;
			cin>>serverIp;
			if(messenger->connect(serverIp)==false)
				cout<<"Error while trying to connect to connect to "<<serverIp<<endl;
			}
		}
		else if(command=="play")
		{
			if(messenger->isConnected()==false){
				messenger->print();

				continue;
			}else{
				cin>>userName;
				messenger->openSessionWithPeer(userName);
			}
		}
		else if(command=="l")
		{
			if(messenger->isConnected()==false){
				messenger->print();
				continue;
			}
			else{
				messenger->status();
			}
		}
		else if(command=="random")
		{
			if(messenger->isConnected()==false){
						messenger->print();
						continue;
			}
			else
			{
				messenger->getRandomOpponent();
			}
		}
		else if(command=="register")
		{
			if(messenger->isConnected()==false){
				messenger->print();
				continue;
			}
			else{
			cin>>userName;
			cin>>password;
			messenger->registerr(userName,password);
			}
		}
		else if(command=="login")
		{
			if(messenger->isConnected()==false){
				messenger->print();
				continue;
			}
			if(messenger->isLoggedIn()==true){
				cout<<"You are already logged in"<<endl;
				continue;
			}
			cin>>userName;
			cin>>password;
			messenger->login(userName,password);
		}
		else if(command=="lu")
		{
			if(messenger->isConnected()==false){
				messenger->print();
				continue;
			}
			messenger->askForRegistredUsers();
		}
		else if(command=="lcu")
		{
			if(messenger->isConnected()==false){
				messenger->print();
				continue;
			}
			messenger->askForConnectedUsers();
		}

		else if(command=="s")
		{
			if(messenger->isConnected()==false){
				messenger->print();
				continue;
			}
			message.clear();
			getline(std::cin,message);
			if(message.size()>0 && message[0]==' ')
				message.erase(0,1);
			messenger->sendMesseage(message);
		}
		else if(command=="cs")
		{
			if(messenger->isConnected()==false){
				messenger->print();
				continue;
			}
			if(messenger->isSessionActivePeer()==false)
				cout<<"You don't have session active, use instruction to connect to session"<<endl;
			messenger->closeSession();
		}
		else if(command=="d"){
			if(messenger->isConnected()==false){
				cout<<"Please connect to server, use Instructions(p) "<<endl;
				continue;
			}
			else
				messenger->disconnect();
		}
		else if(command=="x")
		{
			messenger->close();
			break;
		}
		else if(command=="p")
		{
			printInstructions();
		}
		else if(command=="y")
		{
			messenger->clientAccepted();
		}
		else if(command=="n")
		{
			messenger->clientDeclined();
		}
		else
			cout <<"wrong input"<< endl;
	}
	delete messenger;
	cout<<"Messenger Client App closed  "<<endl;

	return 0;
}
