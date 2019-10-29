#pragma once
//File Name: game_pieces.h
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining functions for the game pieces
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//#include "arrayindicies.h"
using namespace std;

enum pieceColor { red = 0, black = 1, white = 2, invalidColor = 3, noColor = 4, brown = 5 };

string getColorString(pieceColor p);

pieceColor enumColor(string s);

//game piece structure
struct game_piece {
	pieceColor color;
	string name;
	string display;
	game_piece(pieceColor c, string n, string d);
};

