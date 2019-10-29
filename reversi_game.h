#pragma once

//File Name: Reversi_Game.h
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining header functions for the reversi game

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include "game_board.h"
#include "arrayindicies.h"
using namespace std;

class reversi: public game_base {
public:
	void print() override;
	friend ostream & operator<< (ostream &o, const reversi rv);
	reversi(string a, string b);
	string player1;
	string player2;
	bool playerTurn;
protected:
	bool virtual stalemate() override;
	int virtual prompt(unsigned int & a, unsigned int & b);
	bool virtual done() override;
	int virtual turn() override;
	void virtual setup() override;
};

ostream & operator<< (ostream &o, const reversi rv);


bool checkValid(unsigned int i, unsigned int j, vector<game_piece> v, unsigned int dx, unsigned int dy, pieceColor c);