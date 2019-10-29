#pragma once

//File Name: magic_square.h
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining functions and structures for the magic square game

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include "game_board.h"
#include "arrayindicies.h"
using namespace std;

class magic_square : public game_base {
	friend ostream & operator<< (ostream &o, const magic_square ms);
public:
	magic_square();
	void print() override;
	void virtual setup() override;
	bool virtual done() override;
	bool virtual stalemate() override;
	int virtual prompt(unsigned int & a, unsigned int & b);
	void virtual prompt(unsigned int & a);
	int virtual turn() override;
};

ostream & operator<< (ostream &o, const magic_square ms);