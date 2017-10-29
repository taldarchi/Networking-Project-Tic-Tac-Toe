
#include "ConnectionManger.h"
#include <functional>


ConnectionManger::ConnectionManger() {

	running=false;
	dispatcher=new Dispatcher();
	listener=NULL;
	userList();
}

void ConnectionManger::run(){

	while (running){
		listener=new MTCPListener();
		listener->addSockets(peers);
		TCPSocket * readyPeer = listener->listenToSocket(3);
		delete listener ;
		if (readyPeer != NULL)
		{
			data.clear();
			int command;
			if(!MessengerProtocol::readFromSocket(command,data,readyPeer)){
				running = false;
				break;
			}
			temp::iterator iter = data.begin();
			cout << command << endl;
			switch (command){
				case REGISTER:
					registerr(readyPeer,iter->first,iter->second);
					break;
				case LOGIN:
					login(readyPeer,iter->first,iter->second);
					break;
				case PLAYER_WIN:
					playerWin(readyPeer, iter->first);
				break;
				case EXIT:
					break;
				default:
					removePeer(readyPeer);
					break;

			}
		}
	}
}

void ConnectionManger::playerWin(TCPSocket* readpeer,string username)
{
	cout << "HALLO" << username << endl;
	string line;
	ifstream myFile(SCOREBOARD);
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
	remove(SCOREBOARD);
	rename("temp.txt",SCOREBOARD);

}
void ConnectionManger::removePeer(TCPSocket * peer){

	vector<TCPSocket*>:: iterator it=find(peers.begin(),peers.end(),peer);
	if((*it)==peer){
		pthread_mutex_lock(&mutex);
		peers.erase(it);
		pthread_mutex_unlock(&mutex);
	}
}

void ConnectionManger::addPeer(TCPSocket * socket){
	peers.push_back(socket);

	if(running==false){
		running=true;
		start();
	}
	MessengerProtocol::insertDataToSend(data);
	MessengerProtocol::sendToSocket(SESSION_ESTABLISHED_SERVER,data,socket);

}

void ConnectionManger::close(){
	if(running){
		running=false;
		if(dispatcher)
			dispatcher->close();
		vector<TCPSocket *>::iterator iter=peers.begin();
		vector<TCPSocket *>::iterator eiter=peers.end();
		for(;iter!=eiter;iter++)
			MessengerProtocol::sendToSocket(MESSENGER_CLOSED,data,(*iter));
		peers.clear();
		//delete listener;
		listener=NULL;
		waitForThread();
		peers.clear();
	}


}



void ConnectionManger::allConnectedUsers(temp & data){
	dispatcher->AllOnlineUsersServer(data);

}


bool ConnectionManger::isNameExists(const string & userName) {

	string line;
	ifstream myFile(DATABASE);
	if(myFile.is_open())
	{
		while(myFile.good())
		{
			getline(myFile,line);

			if(line.find("User:"+userName)==0)
				return true;
			else
				continue;
		}
		myFile.close();
	}

	//If the user not found
	return false;

}


void ConnectionManger::registerr(TCPSocket * readyPeer,string userName,string password){

	if(isNameExists(userName)==true){
		string name=userName;
		MessengerProtocol::insertDataToSend(data,name);
		MessengerProtocol::sendToSocket(REGISTER__FAILED,data,readyPeer);
		cout<<"User: "<<userName<<" not registered"<<endl;
	}
	else{
		MessengerProtocol::insertDataToSend(data,userName,readyPeer->destIpAndPort());
		MessengerProtocol::sendToSocket(USER_VERIFIED,data,readyPeer);
		dispatcher->addUserToConnected(readyPeer,userName);
		vector <TCPSocket*>::iterator iter;
		iter = find(peers.begin(), peers.end(),readyPeer);
		if (iter != peers.end())
			peers.erase(iter);
		addUserToDatabase(userName,password);
		cout<<"User: "<<userName<<" registered and logged in"<<endl;
		userList();
		dispatcher->updateRegistredUsers(allUserList);
	}
}


void ConnectionManger::addUserToDatabase(string userName,string password){
	ofstream writeFile;
	writeFile.open(DATABASE,ios::app);
	if(writeFile.is_open())
	{
		writeFile<<"User:"+userName<<endl;
		std::tr1::hash<std::string> hash;

		writeFile<<"Pass:" << hash(password)<<endl;
		writeFile.close();

	}
	ofstream writeFile1;
		writeFile1.open(SCOREBOARD,ios::app);
		if(writeFile1.is_open())
		{
			writeFile1<<userName<<":"<< 0<<endl;
			writeFile1.close();

		}
}


void ConnectionManger::login(TCPSocket * readyPeer,const string  userName,const string  password){
	/*if(isUserAlready(userName)==true)
		return false;*/
	if(verfiyUserAndPassword(userName,password)==false){
		MessengerProtocol::sendToSocket(LOGIN_DENIED,data,readyPeer);
		cout<<"User: "<<userName<<" not logged in"<<endl;
		return;
	}
	MessengerProtocol::insertDataToSend(data,userName,readyPeer->destIpAndPort());
	MessengerProtocol::sendToSocket(USER_VERIFIED,data,readyPeer);
	dispatcher->addUserToConnected(readyPeer,userName);
	userList();
	dispatcher->updateRegistredUsers(allUserList);
	vector <TCPSocket*>::iterator iter;
	iter = find(peers.begin(), peers.end(),readyPeer);
	if (iter != peers.end())
		peers.erase(iter);
	cout<<"User: "<<userName<<" logged in"<<endl;
}



bool ConnectionManger::verfiyUserAndPassword(const string & userName,const string & password){
	string line;
	ifstream myFile(DATABASE);
	if(myFile.is_open())
	{
		while(myFile.good())
		{
			getline(myFile,line);

			if(line.compare("User:"+userName)==0)
			{
				//getting password
				getline(myFile,line);
				//checking if equals
				std::tr1::hash<std::string> hash;
				std::ostringstream passStr;
				passStr << "Pass:" << hash(password);
				cout << passStr.str();
				if(line.compare(passStr.str()) == 0)
					return true;
			}
			else
				continue;
		}
		myFile.close();
	}
	//if after reading the whole file we didnt found match, return false
	return false;

}



void ConnectionManger::userList(){
	string line;
	string name;
	int i=1;
	ifstream readFile(DATABASE);
	if(readFile.is_open())
	{
		while(readFile.good())
		{
			getline(readFile,line);
			if(line.find("User:")==0)
			{
				allUserList.insert(pair<string,string>(line.substr(5,line.length()),""));
				i++;
			}
			else
				continue;
		}
		readFile.close();
	}
}

temp ConnectionManger::allUsersServer(){
	return allUserList;
}


temp ConnectionManger::p2pSessions(){
	return dispatcher->getP2PSessions();
}




ConnectionManger::~ConnectionManger() {
}

