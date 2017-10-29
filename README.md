# Networking-Project-Tic-Tac-Toe

The final project is a hybrid P2P and client-server gaming platform with a
game that uses it.

The developed project implements the following functionality:

* The project includes two apps: Messenger Server App (MSA) and
 Messenger Client App (MCA).
* The communication between the MCA and MSA based on TCP 
(control messages).
* The communication between two MCA’s based on UDP 
(interaction between two clients).
* Messenger Server App and Messenger Client App using a CLI
* Programming in C++ in eclipse (JavaEE) / Linux enviroment


### To connect to the server the client will sign in using a username and a password:
○ The client can create a new user with a new password (unless
this user is already created).</br>
○ To communicate with another client, the user will request to
open a session with the required user name.</br>
○ The list of usernames and passwords will be saved into a file in
the server side.</br>

### Client side:
○ A user can send a request to start a game to another user
○ The platform will enable a user to automatically start a game with a
random available user (the user does not need to select the opponent
user, the system will do it for him).
○ A user may accept or reject a game request if he was not the one who
sent the request or asked the platform to automatically do it for him.

### Game session:
○ The client is able to open a game room.</br>
○ After a matching of two players is done the system will enable the
game app to send messages from one user to the other.
○ The game app can end the game and declare its result: user 1 won,
user 2 won, draw, game cancelled. And the score for each user: for
example winner: user 1, score 1: 20, score 2: 10
○ In order not to overload the server, all client communication is
P2P using UDP and does not pass through the server.</br>
