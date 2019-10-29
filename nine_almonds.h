#pragma once

//File Name: nine_almonds.h
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining functions and structures for the the nine almonds game

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include "game_board.h"
#include "arrayindicies.h"
using namespace std;

class nine_almonds : public game_base {
	void print() override;
	friend ostream & operator<< (ostream &o, const nine_almonds na);
	friend int readPieces(ifstream & stream, unsigned int dx, unsigned int dy, vector<game_piece>& v, string ls);
	friend void fillEmptyArray(vector<game_piece>& v, unsigned int a, unsigned int b);
public:
	nine_almonds();
	void virtual setup() override;
	bool virtual done() override;
	bool virtual stalemate() override;
	int virtual prompt(unsigned int & a, unsigned int & b);
	int virtual turn() override;
};

ostream & operator<< (ostream &o, const nine_almonds na);
