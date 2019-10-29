//File Name: magic_square.cpp
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining functions and structures for the magic square

#include "stdafx.h"
#include "game_board.h"
#include "arrayindicies.h"
#include "game_pieces.h"
#include "magic_square.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

magic_square::magic_square() {

}

int readAvailablePieces(ifstream & stream, vector<int> & v) {
	string s;
	//cant get the line
	if (!getline(stream, s)) {
		cout << "Can not get line";
		return canNotGetLine;
	}
	stringstream sstr = stringstream(s);
	unsigned int a;
	while (sstr >> a) {
		v.push_back(a);
	}
	return success;
}

void magic_square::setup() {
	ifstream fileStream_("Magic_Square_Game.txt");
	if (!(fileStream_.is_open())) {
		cout << "Can't open the file! IS IT OPEN";
		throw(quit);
	}
	int a;
	fileStream_ >> a;
	if (a == -1) {
		cout << "a is -1, using regular" << endl;
		fileStream_.close();
		ifstream fileStream("Magic_Square.txt");
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
		for (unsigned int i = 0; i < 10; ++i) {
			this->v_pieces.push_back(i);
		}
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
		readAvailablePieces(fileStream_, v_pieces);
		readPieces(fileStream_, dx, dy, this->v, this->ls);
		fileStream_.close();
	}
	else {
		cout << "a is not 1 or -1, help" << endl;
	}
}

ostream & operator<< (ostream &o, const magic_square ms) {
	vector<game_piece> v_ = ms.v;
	unsigned int dy = ms.dy;
	unsigned int dx = ms.dx;
	for (int i = dy - 1; i >= 0; --i) {
		cout << i << " ";
		for (unsigned int j = 0; j < dx; ++j) {
			//this formula works because the element is at the index height of board * height we are at + x value
			cout << v_[(dy * i) + j].display;
			for (unsigned int c = 0; c < ms.ls.length() + 2 - v_[(dy*i) + j].display.length(); ++c) {
				cout << " ";
			}
		}
		cout << endl << endl;
	}
	cout << "X ";
	for (unsigned int i = 0; i < dy; ++i) {
		cout << i << " ";
	}

	cout << endl << "Available pieces: ";
	for (unsigned int d = 0; d < ms.v_pieces.size(); ++d) {
		if (ms.v_pieces[d] != 0) {
			cout << ms.v_pieces[d] << " ";
		}
	}
	cout << endl << endl;
	return o;
}

void magic_square::print() {
	cout << endl << *this << endl << endl;
}

bool magic_square::done() {
	unsigned int sum = 0;
	vector<game_piece> v_ = this->v;
	for (int i = dy - 1; i >= 0; --i) {
		for (unsigned int j = 0; j < dx; ++j) {
			if (v_[(dy * i) + j].display == " ") {
				return false;
			}
		}
	}
	for (int i = dy - 1; i >= 0; --i) {
		for (unsigned int j = 0; j < dx; ++j) {
			//this formula works because the element is at the index height of board * height we are at + x value
			stringstream ss = stringstream(v_[(dy * i) + j].display);
			unsigned int s_ = 0;
			ss >> s_;
			if (s_ > 0 && s_ < 10) {
				sum += s_;
			}
		}
		if (sum != 15) {
			return false;
		}
		else {
			sum = 0;
		}
	}
	for (unsigned int j = 0; j < dx; ++j) {
		for (int i = dy - 1; i >= 0; --i) {
			stringstream ss = stringstream(v_[(dy * i) + j].display);
			unsigned int s_ = 0;
			ss >> s_;
			if (s_ > 0 && s_ < 10) {
				sum += s_;
			}
		}
		if (sum != 15) {
			return false;
		}
		else {
			sum = 0;
		}
	}
	unsigned int counter_ = 0;
	for (int i = dy - 1; i >= 0; --i) {
		stringstream ss = stringstream(v_[(dy * i) + counter_].display);
		unsigned int s_ = 0;
		ss >> s_;
		if (s_ > 0 && s_ < 10) {
			sum += s_;
		}
		++counter_;
	}
	if (sum != 15) {
		return false;
	}
	sum = 0;
	counter_ = 2;
	for (int i = dy - 1; i >= 0; --i) {
		stringstream ss = stringstream(v_[(dy * i) + counter_].display);
		unsigned int s_ = 0;
		ss >> s_;
		if (s_ > 0 && s_ < 10) {
			sum += s_;
		}
		--counter_;
	}
	if (sum != 15) {
		return false;
	}
	return true;
}

bool magic_square::stalemate() {
	unsigned int savedSum = 0;
	unsigned int sum = 0;
	unsigned int firstTime = 0;
	vector<game_piece> v_ = this->v;
	for (int i = dy - 1; i >= 0; --i) {
		for (unsigned int j = 0; j < dx; ++j) {
			if (v_[(dy * i) + j].display == " ") {
				return false;
			}
		}
	}
	for (int i = dy - 1; i >= 0; --i) {
		for (unsigned int j = 0; j < dx; ++j) {
			//this formula works because the element is at the index height of board * height we are at + x value
			stringstream ss = stringstream(v_[(dy * i) + j].display);
			unsigned int s_ = 0;
			ss >> s_;
			if (s_ > 0 && s_ < 10) {
				sum += s_;
			}
		}
		if (firstTime == 0) {
			savedSum = sum;
			++firstTime;
		}
		if (sum != savedSum) {
			return true;
		}
		sum = 0;
	}
	for (unsigned int j = 0; j < dx; ++j) {
		for (int i = dy - 1; i >= 0; --i) {
			stringstream ss = stringstream(v_[(dy * i) + j].display);
			unsigned int s_ = 0;
			ss >> s_;
			if (s_ > 0 && s_ < 10) {
				sum += s_;
			}
		}
		if (sum != savedSum) {
			return true;
		}
		sum = 0;
	}
	unsigned int counter_ = 0;
	for (int i = dy - 1; i >= 0; --i) {
		stringstream ss = stringstream(v_[(dy * i) + counter_].display);
		unsigned int s_ = 0;
		ss >> s_;
		if (s_ > 0 && s_ < 10) {
			sum += s_;
		}
		++counter_;
	}
	if (sum != savedSum) {
		return true;
	}
	sum = 0;
	counter_ = 2;
	for (int i = dy - 1; i >= 0; --i) {
		stringstream ss = stringstream(v_[(dy * i) + counter_].display);
		unsigned int s_ = 0;
		ss >> s_;
		if (s_ > 0 && s_ < 10) {
			sum += s_;
		}
		--counter_;
	}
	if (sum != savedSum) {
		return true;
	}
	return false;
}

int magic_square::prompt(unsigned int & a, unsigned int & b) {
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
		if (a >= dx) {
			prompt(a, b);
			return true;
		}
	}
	if (sstr >> b) {
		if (b >= dy) {
			prompt(a, b);
			return true;
		}
		returnValue = true;
	}
	return returnValue;
}

void magic_square::prompt(unsigned int & a) {
	string s = "";
	bool continue_ = false;
	while (!continue_) {
		cout << "Please type quit to end the game or the value of an available piece you would like to use" << endl;
		cin >> s;
		if (s == string("quit")) {
			throw quit;
		}
		int i;
		try {
			i = stoi(s);
		}
		catch (exception e) {
			continue;
		}

		if (i < 10 && i > 0 && v_pieces[i] > 0) {
			continue_ = true;
		}

	}
	stringstream sstr = stringstream(s);
	if (sstr >> a) {
	}
}

int magic_square::turn() {
	unsigned int a;
	unsigned int b;
	unsigned int c;
	print();
	try {
		prompt(a);
	}
	catch (exitCodes e) {
		cout << e << endl;
		return quit;
	}
	cout << "Where would you like to put the available piece?" << endl;
	if (prompt(b, c) == quit) {
		return quit;
	}
	while (this->v[this->dy*c + b].display != " ") {
		cout << "INVALID MOVE, choose a space without a piece already present" << endl;
		if (prompt(b, c) == quit) {
			return quit;
		}
	}
	game_piece emptyPiece(noColor, "", " ");
	string ss = "";
	ss += to_string(a);
	game_piece thisPiece(noColor, ss, ss);
	this->v_pieces[a] = 0;
	this->v[this->dy*c + b] = thisPiece;
	return success;
}

/*game_base::game_base() {
shared_p = make_shared<game_base>(nullptr);
}

nine_almonds::nine_almonds() {

}

magic_square::magic_square() {

}*/

shared_ptr<game_base> game_base::instance() {
	if (shared_p != nullptr) {
		return shared_p;
	}
	else {
		throw "shared_p is a null ptr";
	}
}

