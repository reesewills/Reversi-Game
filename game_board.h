#pragma once

//File Name: game_board.h
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining functions and structures for the game board

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include "arrayindicies.h"
using namespace std;

class game_base {
public:
	game_base() {

	}
	static void create_game(int a, char * arr[]);
	int play();
	static shared_ptr<game_base> instance();
protected:
	static shared_ptr<game_base> shared_p;
	void virtual setup() = 0;
	void virtual print() = 0;
	bool virtual done() = 0;
	bool virtual stalemate() = 0;
	int virtual prompt(unsigned int & a, unsigned int &b) = 0;
	int virtual turn() = 0;
	unsigned int dx;
	unsigned int dy;
	string game_name;
	vector<int> v_pieces;
	vector<game_piece> v;
	string ls;
};

void fillEmptyArray(vector<game_piece> &v, unsigned int a, unsigned int b);

int readPieces(ifstream & stream, unsigned int dx, unsigned int dy, vector<game_piece>& v, string ls);

int dimensions(ifstream & stream, unsigned int & dx, unsigned int & dy);