
#include <iostream>
using namespace std;

#include "Server.h"

void printInstructions(){


	cout<<"To see list of all users type: lu                                  "<<endl;
	cout<<"To see list of online users type: lcu                              "<<endl;
    cout<<"To see all the P2P sessions type: ls                               "<<endl;
    cout<<"To see the scoreboard type: sb                                     "<<endl;
	cout<<"To exit type: x                                                    "<<endl;
}


int main() {
	printInstructions();
	Server * server= new Server();
	while(true)
	{
		string command;
		cin>>command;
		if(command=="lu"){
			server->allUsers();

		}
		else if(command=="lcu")
		{
			server->connectedUsers();
		}
		else if(command=="ls")
		{
			server->allP2PSessions();
		}
		else if(command == "sb")
		{
			std::ifstream f("scoreboard.txt");
			if(f.is_open())
			{
				cout << f.rdbuf() << endl;
			}
		}
		else if (command=="x")
		{
			break;
		}else
				cout<<"Wrong Input... Try again"<<endl;

	}
	server->close();
	delete server;

	cout<<"Server closed  *"<<endl;

	return 0;
}
