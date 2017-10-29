/*
 * Game.cpp
 *
 *  Created on: Aug 7, 2017
 *      Author: ubuntu
 */

#include "Game.h"



Game::Game() {
	for(int i =0; i <3 ; i++)
		for(int j = 0; j< 3; j++)
			board[i][j] = ' ';
	count = 0;
}

Game::~Game() {

}

void Game::MakeTurn(char player,int x, int y)
{
	cout << player << "x"<<x<<"y"<<y<<endl;
	if(board[x][y] == ' '){
		board[x][y] = player;
		count++;
	}
}
char Game::lineWin(int a, int b, int c)
{

	if (board[getXcord(a)][getYcord(a)] == ' ')  return 'n';
	if (board[getXcord(a)][getYcord(a)] != board[getXcord(b)][getYcord(b)]) return 'n';
	if (board[getXcord(b)][getYcord(b)] != board[getXcord(c)][getYcord(c)]) return 'n';
	return board[getXcord(a)][getYcord(a)];
}

int Game::getXcord(int n)
{
	return n % 3;
}
int Game::getYcord(int n)
{
	return n / 3;
}
bool Game::IsBoardFull()
{
	return count == 9;
}
char Game::getTurnSymbol()
{
	if(count % 2 == 0)
		return PLAYER1;
	return PLAYER2;
}
char Game::CheckForWin()
{

	char wnr;

	// Horizontal lines.

	if ((wnr = lineWin(0,1,2)) != 'n') return wnr;
	if ((wnr = lineWin(3,4,5)) != 'n') return wnr;
	if ((wnr = lineWin(6,7,8)) != 'n') return wnr;

	// Vertical lines.

	if ((wnr = lineWin(0,  3,   6  )) != 'n') return wnr;
	if ((wnr = lineWin(1,4,7)) != 'n') return wnr;
	if ((wnr = lineWin(2,  5,  8 )) != 'n') return wnr;

	// Diagonal lines.

	if ((wnr = lineWin(0,  4, 8  )) != 'n') return wnr;
	if ((wnr = lineWin(2, 4, 6  )) != 'n') return wnr;

	if(IsBoardFull())
		return TIE;
	return 'n';
}
char Game::GetSymbol(int pos)
{
	if(pos == 1)
		return PLAYER1;
	if(pos == 2)
		return PLAYER2;
	return ' ';
}

bool Game::isGoodInput(string s)
{
	if(s.length() !=5)	// <x,y>
	{
		return false;
	}
	if(s[1]>='0' && s[3]<='2')
	{
		if(s[0] =='<' && s[2] == ',' && s[4] == '>')
			return true;
	}
	if(board[s[1]-'0'][s[3]-'0'] != ' ')
		return false;
	return false;
}

void Game::PrintBoard()
{
	cout << "     |     |     " << endl;
	cout << "  " << board[0][0] << "  |  " << board[0][1] << "  |  " << board[0][2] << endl;

	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;

	cout << "  " << board[1][0] << "  |  " << board[1][1] << "  |  " << board[1][2] << endl;

	cout << "_____|_____|_____" << endl;
	cout << "     |     |     " << endl;

	cout << "  " << board[2][0] << "  |  " << board[2][1] << "  |  " << board[2][2] << endl;

	cout << "     |     |     " << endl << endl;
}
