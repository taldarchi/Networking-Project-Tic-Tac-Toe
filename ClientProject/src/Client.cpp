

#include "Client.h"


Client::Client() {
	tcpSocket=NULL;
	udpSocket=NULL;
	SessionActivePeer=false;

	loggedIn=false;
	isGameRunning = false;
	connected=false;
	running=false;
	isMyTurn = false;
	command=0;
}


void Client::startUDP(temp & data){
	iter=data.begin();
	int i=0;
	string ip;
	string port;
	i = iter->second.find(':');
	ip = iter->second.substr(0, i);
	port=iter->second.substr(i+1,iter->second.size());
	int _port=atoi(port.c_str());
	udpSocket=new UDPMessenger(_port);
}


bool Client::connect(const string & ip){
	cout<<"Trying to connect to server"<<endl;
	tcpSocket = new TCPSocket(ip, MSNGR_PORT);
	if (tcpSocket->getSocketFd()<0){
		connected=false;
		return false;
	}
	connected = true;
	running=true;
	start();
	return true;
}


void Client::run()
{
	while(running)
	{
		data.clear();
		if(!MessengerProtocol::readFromSocket(command,data,tcpSocket)){
			running = false;
			break;
		}
		iter=data.begin();

		switch(command)
		{
			case SESSION_ESTABLISHED_SERVER:
				sessionEstablished();
				break;
			case REGISTER__FAILED:
				registrationError(iter->first);
				break;
			case USER_VERIFIED:
				userVerified(data);
				break;
			case LOGIN_DENIED:
				loginDenied();
				break;

			case PEER_NOT_FOUND:
				peerNotFound(iter->second);
				break;
			case SESSION_REFUSED:
				cout<<"Connection to peer refused, peer might be busy "<<endl;
				cout<<"or disconnected, try again later"<<endl;
				break;
			case OPEN_SESSION_WITH_PEER:
				newPeerSessionRequest(iter->first,iter->second);
				break;
			case ERROR_WHILE_CLOSING_SESSION:
				cout<<"Error while closing open session, all opened session closed"<<endl;
				break;
			case CLOSE_SESSION_WITH_PEER:
				sessionWithPeerEnded();
				break;
			case ALL_USERS://
				getAllUsers(data);
				break;
			case CONNECTED_USERS://
				getAllConnctedUsers(data);
				break;
			case PEER_ACCEPTED_GAME:
				newPeerSessionRequest(iter->first,iter->second);
				initGame(1);
				break;
			case RANDOM_USER_FOR_GAME:
					openSessionWithPeer(iter->first);

				break;
			case PEER_DECLINED_GAME:
				sessionWithPeerEnded();
				game = NULL;

				break;
			case GAME_ENDED:
				gameEnded();
				break;

			case EXIT:
			case MESSENGER_CLOSED:
				serverClosed();
				break;
			default:
				disconnect();
				break;
		}

	}
}

void Client::getAllUsers(temp & data)
{
	if(data.size()==0){
		cout<<"There are no user registered"<<endl;
		return;
	}
	cout<<"List of all registered users"<<endl;
	printList(data);
}


void Client::getAllConnctedUsers(temp & data)
{
	if(data.size()==0){
		cout<<"There are no user connected"<<endl;
		return;
	}
	cout<<"List of all connected users"<<endl;
	printList(data);
}



void Client::getRandomOpponent()
{
	MessengerProtocol::sendToSocket(RANDOM_USER_FOR_GAME,data,tcpSocket);
}
string Client::getSessionName()
{
	if(SessionActivePeer)
		return sessionWithPeer.name;


	return "";
}

void Client::clientAccepted()
{

	if(askingForAccept)
	{

		MessengerProtocol::insertDataToSend(data,this->userName,nameOfOpponent);
		MessengerProtocol::sendToSocket(PEER_ACCEPTED_GAME,data,tcpSocket);
		initGame(2);
		cout << "Game against " << nameOfOpponent << " has started!" << endl;
		cout << "Your symbol is " << mySymbol << endl;
		askingForAccept = false;
	}
	else if(askingForRematch)
	{
		openSessionWithPeer(nameOfOpponent);
		askingForRematch=false;
	}
}
void Client::clientDeclined()
{
	if(askingForAccept)
		{
			MessengerProtocol::insertDataToSend(data,this->userName,nameOfOpponent);
			MessengerProtocol::sendToSocket(PEER_DECLINED_GAME,data,tcpSocket);
			closeSession();
			askingForAccept = false;
		}
	if(askingForRematch)
	{
		closeSession();
		askingForRematch=false;
	}
}

void Client::login(string userName,string password){
	MessengerProtocol::insertDataToSend(data,userName,password);
	MessengerProtocol::sendToSocket(LOGIN,data,tcpSocket);
}


void Client::registerr(string userName,string  password)
{

	MessengerProtocol::insertDataToSend(data,userName,password);
	MessengerProtocol::sendToSocket(REGISTER,data,tcpSocket);
}


//Send command to close room. Done


//Send command to close session with peer/leave room. Done
void Client::closeSession()
{
	if(SessionActivePeer){
		//MessengerProtocol::insertDataToSend(data,clientName,sessionClinet.name);
		MessengerProtocol::sendToSocket(CLOSE_SESSION_WITH_PEER,data,tcpSocket);
		return;
	}

}

bool Client::isSessionActivePeer(){
	return SessionActivePeer;
}


bool Client::isConnected(){
	return connected;
}

bool Client::isLoggedIn(){
	return loggedIn;
}

void Client::userVerified(temp data ){
	cout<<"Login successful you are connected"<<endl;
	temp::iterator iter=data.begin();
	loggedIn=true;
	userName=iter->first;
	startUDP(data);

}

void Client::sessionWithPeerEnded(){
	cout<<"Your session with "<<sessionWithPeer.name<<" has ended "<<endl;
	sessionWithPeer.remove();
	SessionActivePeer=false;
	gameAccepted = false;
}

void Client::loginDenied(){
	cout<<"Login Denied  "<<endl;
	userName="";
	loggedIn=false;
}

void Client::registrationError(const string & userName){
	cout<<"Registration failed a user with name: "+userName+""<<endl;
	cout<<"already exits. try different user name"<<endl;
	loggedIn=false;
}

void Client::sessionEstablished(){
	cout<<"Connection established. Please login or register"<<endl;
	cout<<"for help use instructions(p) "<<endl;
	connected=true;

}

void Client::serverClosed()
{
	cout<<"The connection to the server interrupted "<<endl;
	cout<<"please try to connect again later "<<endl;
	connected=false;

	SessionActivePeer=false;
	loggedIn=false;
	userName.clear();

	sessionWithPeer.remove();
	sessions.clear();
	running=false;
}

//Get new peer session. Done
void Client::newPeerSessionRequest(const string & userName,const string & ipAndPort){


	nameOfOpponent = userName;
	temp::iterator iter=data.begin();
	int i=0;
	string ip;
	string port;
	i=ipAndPort.find(':');
	ip=ipAndPort.substr(0,i);
	port=ipAndPort.substr(i+1,iter->second.size());
	sessionWithPeer.ip=ip;
	sessionWithPeer.name=userName;
	sessionWithPeer.port=atoi(port.c_str());
	//cout<<"You are in session With ->" <<sessionWithPeer.name<< "(" +  ipAndPort + ")."<<endl;
	SessionActivePeer=true;
	if(!isGameRunning)
	{
	cout << userName << " has challenged you to a game! y-accept/n-decline" << endl;
	askingForAccept = true;
	}

}

void Client::initGame(int x)
{
	game = new Game();
	mySymbol = game->GetSymbol(x);
	udpSocket->initGame(game,mySymbol);
	game->PrintBoard();
}

void Client::peerNotFound(const string & peerName){
	cout<<"Open session with "<<peerName<<" failed user not found.  "<<endl;
	SessionActivePeer=false;
}

//*------------------------------------------------------------------------*//


//Disconnect user from server. Done
void Client::disconnect(){
	closeSession();
	MessengerProtocol::insertDataToSend(data,userName);
	MessengerProtocol::sendToSocket(EXIT,data,tcpSocket);
	sleep(1);
	running=false;
	connected = false;
	SessionActivePeer = false;

	loggedIn = false;
	sessions.empty();
	sessionWithPeer.remove();
	if (udpSocket) {
		udpSocket->close();
		delete udpSocket;
		udpSocket = NULL;
	}
	if (tcpSocket) {
		tcpSocket->close();
	}
	waitForThread();
	delete tcpSocket;
	tcpSocket = NULL;
	cout<<"You are disconnected from server"<<endl;
}

//Close client
void Client::close(){
	if(!connected)
		return;
	disconnect();
}

//Print list of users. Done
void Client::printList(temp & data){

	temp::iterator iter=data.begin();
	temp::iterator eiter=data.end();
	for(;iter!=eiter;iter++){
		cout<<iter->first<<endl;
	}
}

void Client::print(){
	cout<<"Please connect to server, use Instructions(p) "<<endl;
}


void Client::status(){
	if(SessionActivePeer){
		cout<<"You are in session with "<<getSessionName()<<endl;
	}

	else{
		cout<<"You are connected to server with no open session"<<endl;
	}



}

void Client::openSessionWithPeer(string userName){
	closeSession();

    cout<<"Waiting for " << userName << " to accept or decline" << endl;
    isGameRunning = true;

	MessengerProtocol::insertDataToSend(data,this->userName,userName);
	MessengerProtocol::sendToSocket(OPEN_SESSION_WITH_PEER,data,tcpSocket);
}


void Client::PrintGameResult(char result)
{
	if(result == 'T')
	{
		cout << "The game ended in a tie!" << endl;
	}
	if(result == game->GetSymbol(1))
	{
		cout << "Player " << game->GetSymbol(1) << " won the game!" << endl;
	}
	if(result == game->GetSymbol(2))
	{
		cout << "Player " << game->GetSymbol(2) << " won the game!" << endl;
	}

	isGameRunning = false;
}

void Client::gameEnded()
{
	if(game->CheckForWin() == 'T')
			{
				PrintGameResult('T');


			}
			else if(game->CheckForWin() == game->GetSymbol(1)) // player 1 won!
			{
				PrintGameResult(game->GetSymbol(1));


			}
			else if(game->CheckForWin() == game->GetSymbol(2)) // player 2 won!;
			{
				PrintGameResult(game->GetSymbol(2));


			}
}
void Client::sendMesseage(string message){
	cout << game->getTurnSymbol() << endl;

	if (SessionActivePeer == true ){
		if(mySymbol == game->getTurnSymbol()){
		cout << "Is Input good? " << game->isGoodInput(message) << endl;
		if(game->isGoodInput(message)){

		game->MakeTurn(mySymbol,message[1]-'0',message[3]-'0');
		game->PrintBoard();
		udpSocket->sendTo("[" + userName + "] " + message+mySymbol,sessionWithPeer.ip,sessionWithPeer.port);
		if(game->CheckForWin() == 'T')
		{
			PrintGameResult('T');
			MessengerProtocol::insertDataToSend(data,this->userName,sessionWithPeer.name);
			MessengerProtocol::sendToSocket(GAME_ENDED,data,tcpSocket);

			closeSession();
		}
		else if(game->CheckForWin() == game->GetSymbol(1)) // player 1 won!
		{
			PrintGameResult(game->GetSymbol(1));
			MessengerProtocol::insertDataToSend(data,this->userName,sessionWithPeer.name);
			MessengerProtocol::sendToSocket(GAME_ENDED,data,tcpSocket);
			MessengerProtocol::sendToSocket(PLAYER_WIN,data,tcpSocket);

			closeSession();
		}
		else if(game->CheckForWin() == game->GetSymbol(2)) // player 2 won!;
		{
			PrintGameResult(game->GetSymbol(2));
			MessengerProtocol::insertDataToSend(data,this->userName,sessionWithPeer.name);
			MessengerProtocol::sendToSocket(GAME_ENDED,data,tcpSocket);
			MessengerProtocol::sendToSocket(PLAYER_WIN,data,tcpSocket);
	    	closeSession();
		}
		else //nothing happend, game is still on
		{

		}
		return;
		}
		}
		else
		{
			cout << "It's not my turn! wait for the other player to make a move." << endl;
		}
	//	}

		//else

	}
	else
	cout << "You don't have an open session, Please open session to send message" <<  endl;

}


void Client::askForRegistredUsers()
{
	MessengerProtocol::sendToSocket(ALL_USERS,data,tcpSocket);
}


void Client::askForConnectedUsers()
{
	MessengerProtocol::sendToSocket(CONNECTED_USERS,data,tcpSocket);
}


Client::~Client() {}
