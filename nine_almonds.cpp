//File Name: nine_almonds.cpp
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) declaring functions for the nine almonds game

#include "stdafx.h"
#include "game_board.h"
#include "arrayindicies.h"
#include "game_pieces.h"
#include "nine_almonds.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

nine_almonds::nine_almonds() {

}

void nine_almonds::setup() {
	ifstream fileStream_("Nine_Almonds_Game.txt");
	if (!(fileStream_.is_open())) {
		cout << "Can't open the file! IS IT OPEN";
		throw(quit);
	}
	int a;
	fileStream_ >> a;
	if (a == -1) {
		cout << "a is -1, using regular" << endl;
		fileStream_.close();
		ifstream fileStream("Nine_Almonds.txt");
		if (!(fileStream.is_open())) {
			cout << "Can't open the file! IS IT OPEN";
		}
		unsigned int dx;
		unsigned int dy;
		int dimensionResults = dimensions(fileStream, dx, dy);
		while (dimensionResults == canNotExtract) {
			dimensionResults = dimensions(fileStream, dx, dy);
		}
		if (dimensionResults == canNotGetLine) {
			cout << "Can not get the line from dimension results" << endl;
		}
		this->dx = dx;
		this->dy = dy;
		fillEmptyArray(this->v, this->dx, this->dy);
		readPieces(fileStream, dx, dy, this->v, this->ls);
		fileStream.close();
	}
	else if (a == 1) {
		cout << "a is 1, using saved" << endl;
		unsigned int dy;
		unsigned int dx;
		int dimensionResults = dimensions(fileStream_, dx, dy);
		while (dimensionResults == canNotExtract) {
			dimensionResults = dimensions(fileStream_, dx, dy);
		}
		if (dimensionResults == canNotGetLine) {
			cout << "Can not get the line from dimension results" << endl;
		}
		this->dx = dx;
		this->dy = dy;
		fillEmptyArray(this->v, this->dx, this->dy);
		readPieces(fileStream_, dx, dy, this->v, this->ls);
		fileStream_.close();
	}
	else {
		cout << "a is not 1 or -1, help" << endl;
	}
}

bool nine_almonds::done() {
	bool returnValue = true;
	for (unsigned int i = 0; i < this->v.size(); ++i) {
		if (i == (this->v.size() / 2)) {
			if (this->v[i].color == noColor) {
				returnValue = false;
			}
		}
		else {
			if (this->v[i].color != noColor) {
				returnValue = false;
			}
		}
	}
	return returnValue;
}

bool nine_almonds::stalemate() {
	if (this->done() == true) {
		return false;
	}
	for (unsigned int i = 0; i < dy; ++i) {
		for (unsigned int j = 0; j < dx; ++j) {
			//if the element has a color, then show it
			if (v[(dy * i) + j].color != noColor) {
				//check if it has something to the left
				if (j > 0 && ((dy*i) + j - 1) < v.size() && v[((dy*i) + j - 1)].color != noColor) {
					return false;
				}
				//check if it has something to the right
				if (((dy*i) + j + 1) > 0 && ((dy*i) + j + 1) < v.size() && v[((dy*i) + j + 1)].color != noColor) {
					return false;
				}
				//element up and to the right
				if ((dy*(i + 1) + (j + 1)) > 0 && (dy*(i + 1) + (j + 1)) < v.size() && v[(dy*(i + 1) + (j + 1))].color != noColor) {
					//element below and to the left
					if (i > 0 && j > 0 && (dy*(i - 1) + (j - 1)) < v.size() && v[(dy*(i - 1) + (j - 1))].color == noColor) {
						return false;
					}
				}
				//element above
				if ((dy*(i + 1) + (j)) > 0 && (dy*(i + 1) + (j)) < v.size() && v[(dy*(i + 1) + (j))].color != noColor) {
					return false;
				}
				//element up and to the left
				if (j > 0 && (dy*(i + 1) + (j - 1)) < v.size() && v[(dy*(i + 1) + (j - 1))].color != noColor) {
					//element below to the right
					if (i > 0 && (dy*(i - 1) + (j + 1)) < v.size() && v[(dy*(i - 1) + (j + 1))].color == noColor) {
						return false;
					}
				}
				//element below
				if (i > 0 && (dy*(i - 1) + (j)) < v.size() && v[(dy*(i - 1) + (j))].color != noColor) {
					return false;
				}

			}
		}
	}
	return true;
}

int nine_almonds::prompt(unsigned int &a, unsigned int &b) {
	bool returnValue = false;
	cout << "Please type 'quit' to end the game or a coordinate separated by a comma such as '0,0'" << endl;
	string s = "";
	cin >> s;
	if (s == "quit") {
		return quit;
	}
	while (s.length() != 3 || (s.substr(1, 1) != ",")) {
		cout << s.substr(1, 1) << endl;
		cout << "Please type 'quit' to end the game or a coordinate separated by a comma such as '0,0'" << endl;
		cin >> s;
	}
	if (s == "quit") {
		return quit;
	}
	s.replace(1, 1, " ");
	stringstream sstr = stringstream(s);
	if (sstr >> a) {

	}
	if (sstr >> b) {

	}
	return returnValue;
}

int nine_almonds::turn() {
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
	print();
	cout << "Which almond are you moving?" << endl;
	if (prompt(a, b) == quit) {
		return quit;
	}
	while (a >= this->dx || b >= this->dy || this->v[this->dy*b + a].color != brown) {
		cout << "INVALID MOVE, choose a space with an almond!" << endl;
		if (prompt(a, b) == quit) {
			return quit;
		}
	}
	bool sameTurn = true;
	vector<int> vi;
	vi.push_back(a);
	vi.push_back(b);
	while (sameTurn) {
		cout << "Where would you like to move it to?" << endl;
		if (prompt(c, d) == quit) {
			return quit;
		}
		while ((a == c && b == d) || ((this->dy * d + c) >= this->v.size() || this->v[this->dy*d + c].color != noColor) || ((this->dy * b + a) >= this->v.size() || this->v[this->dy*b + a].color == noColor) || (abs(int(a) - int(c)) != 2 && abs(int(a) - int(c)) != 0) || (abs(int(b) - int(d)) != 2 && abs(int(b) - int(d)) != 0)) {
			cout << "INVALID MOVE" << endl;
			if (prompt(c, d) == quit) {
				return quit;
			}
		}
		unsigned int e = ((a + c) / 2);
		unsigned int f = ((b + d) / 2);
		if (this->v[this->dy*f + e].color != brown) {
			return invalidMove;
		}
		game_piece emptyPiece(noColor, "", " ");
		game_piece brownAlmond(brown, "almond", "A");
		this->v[this->dy*f + e] = emptyPiece;
		this->v[this->dy*d + c] = brownAlmond;
		this->v[this->dy*b + a] = emptyPiece;
		vi.push_back(c);
		vi.push_back(d);
		a = c;
		b = d;
		;

		for (unsigned int i = 0; i < vi.size(); i += 2) {
			if (i != 0) {
				cout << " to ";
			}
			cout << vi[i] << "," << vi[(i + 1)];
		}

		cout << endl << endl << "Continue this turn? (YyNn)" << endl;
		string s;
		while (s != string("y") && s != string("Y") && s != string("N") && s != string("n")) {
			s = "";
			cin >> s;
		}
		if (s == "N" || s == "n") {
			sameTurn = false;
		}
	}
	return success;
}

ostream & operator<< (ostream &o, const nine_almonds na) {
	vector<game_piece> v_ = na.v;
	unsigned int dy = na.dy;
	unsigned int dx = na.dx;
	for (int i = dy - 1; i >= 0; --i) {
		cout << i << " ";
		for (unsigned int j = 0; j < dx; ++j) {
			//this formula works because the element is at the index height of board * height we are at + x value
			cout << v_[(dy * i) + j].display;
			for (unsigned int c = 0; c < na.ls.length() + 2 - v_[(dy*i) + j].display.length(); ++c) {
				cout << " ";
			}
		}
		cout << endl << endl;
	}
	cout << "X ";
	for (unsigned int i = 0; i < dy; ++i) {
		cout << i << " ";
	}
	return o;
}

void nine_almonds::print() {
	cout << endl << *this << endl << endl;
}
