/*
 * Game.h
 *
 *  Created on: Aug 7, 2017
 *      Author: ubuntu
 */

#ifndef GAME_H_
#define GAME_H_

#define PLAYER1 'X'
#define PLAYER2 'O'
#define TIE 'T'
#include <string>
#include <iostream>
using namespace std;

class Game {
private:
	char board[3][3];
	char lineWin(int a, int b, int c);
	int getXcord(int n);
	int getYcord(int n);
	int count;
public:
	Game();
	void MakeTurn(char player,int x, int y);
	char GetSymbol(int pos);
	char getTurnSymbol();
	char CheckForWin();
	bool isGoodInput(string s);
	bool IsBoardFull();
	void PrintBoard();
	virtual ~Game();
};

#endif /* GAME_H_ */
