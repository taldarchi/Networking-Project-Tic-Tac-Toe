/*
 * TCPMessengerProtocol.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef MESSENGERPROTOCOL_H_
#define MESSENGERPROTOCOL_H_

using namespace std;

#include "TCPSocket.h"
#include "FileInter.h"
#include <map>
#include <vector>
typedef map<string,string> temp;
typedef vector<TCPSocket *> tPeers;

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */

#define MSNGR_PORT 3346
#define TEST_PORT  3347




#define OPEN_SESSION_WITH_PEER        1//
#define CLOSE_SESSION_WITH_PEER       2//
#define LOGIN                         3//
#define REGISTER                      4//
#define ALL_USERS                     9//
#define CONNECTED_USERS               10//
#define USERS_IN_SESSION              13//
//Reports
#define SESSION_ESTABLISHED_SERVER    50
#define SESSION_REFUSED               17//
#define PEER_NOT_FOUND                18//
#define USER_VERIFIED                 19//
#define LOGIN_DENIED                  20//
#define REGISTER__FAILED              21//
#define PEER_ACCEPTED_GAME			  22
#define PEER_DECLINED_GAME            23
#define RANDOM_USER_FOR_GAME          24
#define GAME_ENDED                    25
#define PLAYER_WIN                    26
#define ERROR_WHILE_CLOSING_SESSION   34//
#define MESSENGER_CLOSED              40
#define EXIT                          41

#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"
#define SERVER_SHUTDOWN "The connection to the server interrupted please try to connect again later"




class MessengerProtocol{
private:
	void static sendInt(int val,TCPSocket* sock){
		val = htonl(val);
		sock->write((char*)&val,4);
	}
	void static sendString(const string& val,TCPSocket* sock){
		sendInt(val.length(),sock);
		sock->write(val.c_str(),val.length());
	}

	bool static readInt(int& val,TCPSocket* sock){
		int rc = sock->read((char*)&val,4);
		val = ntohl(val);
		if (rc < 4) return false;
		return true;
	}
	bool static readString(string& val,TCPSocket* sock){
		int len;
		bool rc = readInt(len,sock);
		if(!rc) return false;
		char buff[512];
		sock->read(buff,len);
		buff[len] = '\0';
		val = buff;
		return true;
	}
public:
	void static sendToSocket(int command,const map<string,string>& data, TCPSocket* sock){
		//first send the command
		sendInt(command,sock);

		//send the size of the map
		sendInt(data.size(),sock);

		//send the map
		if(data.size()>0){
			map<string,string>::const_iterator iter = data.begin();
			map<string,string>::const_iterator eiter = data.end();
			for(;iter != eiter ; ++iter){
				string key = iter->first;
				string val = iter->second;
				sendString(key,sock);
				sendString(val,sock);
			}
		}
	}


	bool static readFromSocket(int& command, map<string,string>& data,TCPSocket* sock ){
		bool rc = readInt(command,sock);
		if (!rc) return false;

		data.clear();
		int mapSize = 0;
		rc = readInt(mapSize,sock);
		if (!rc) return false;

		if (mapSize > 0){
			for(int i=0;i<mapSize;++i){
				string key;
				string val;
				if(!readString(key,sock)) return false;
				if(!readString(val,sock)) return false;
				data.insert(pair<string,string>(key,val));
			}
		}
		return true;
	}

	void static insertDataToSend(temp & data, const string & data1="",const string & data2="")
	{
		data.clear();
		data.insert(pair<string,string>(data1,data2));

	}

};


#endif /* MESSENGERPROTOCOL_H_ */
