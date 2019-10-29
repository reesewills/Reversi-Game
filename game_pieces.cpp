//File Name: game_pieces.cpp
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining functions and structures for game pieces!

#include "stdafx.h"
#include "game_pieces.h"
#include "arrayindicies.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

//get the color as a string based on the pieceColor
string getColorString(pieceColor p) {
	switch (p) {
	case black: return "black";
		break;
	case white: return "white";
		break;
	case invalidColor: return "invalid color";
		break;
	case noColor: return "noColor";
		break;
	case red: return "red";
		break;
	case brown: return "brown";
		break;
	default: return "not an option";
	}
}

//get the pieceColor based on the string
pieceColor enumColor(string s) {
	if(s == "red") return red;
	else if (s == "black") {
		return black;
	}
	else if (s == "white") {
		return white;
	}
	else if (s == "invalidColor") {
		return invalidColor;
	}
	else if (s == "noColor") {
		return noColor;
	}
	else if (s == "brown") {
		return brown;
	}
	else {
		return invalidColor;
	}
}


game_piece::game_piece(pieceColor c, string n, string d):color(c), name(n), display(d) {};



