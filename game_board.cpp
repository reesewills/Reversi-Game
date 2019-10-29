//File Name: game_board.cpp
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining functions and structures for the game board

#include "stdafx.h"
#include "game_board.h"
#include "arrayindicies.h"
#include "magic_square.h"
#include "game_pieces.h"
#include "reversi_game.h"
#include "nine_almonds.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Here are all the neutral methods not directly tied to the games

//adds the pieces to the vector based on the text file
int readPieces(ifstream & stream, unsigned int dx, unsigned int dy, vector<game_piece>& v, string ls) {
	string line;
	bool result = false;
	while (getline(stream, line)) {
		stringstream sstr = stringstream(line);
		string color;
		string name;
		string display;
		//start with it being invalid, we can change this later
		pieceColor pc = invalidColor;
		unsigned int x;
		unsigned int y;
		if (sstr >> color) {
			pc = enumColor(color);
			//if it is invalid, go on to next part in while loop
			if (pc == invalidColor) {
				continue;
			}
		}
		if (sstr >> name) {

		}
		if (sstr >> display) {

		}
		if (sstr >> x) {
			//continue to next part of while loop if it is out of the dimensions of board
			if (x > dx) {
				continue;
			}
		}
		if (sstr >> y) {
			if (y > dy) {
				continue;
			}
		}
		game_piece gp(pc, name, display);
		result = true;
		//add it to the vector using the formula defined aboves
		v[dy * y + x] = gp;

		if (display.length() > ls.length()) {
			ls = display;
		}
	}
	if (result) {
		return success;
	}
	else {
		//text file doesnt have the correct format to be read in
		return notWellFormed;
	}
}

//reads in the dimensions of the board
int dimensions(ifstream & stream, unsigned int & dx, unsigned int & dy) {
	string s;
	//cant get the line
	if (!getline(stream, s)) {
		cout << "Can not get line";
		return canNotGetLine;
	}
	stringstream sstr = stringstream(s);
	if (!(sstr >> dx)) {
		return canNotExtract;
	}
	if (!(sstr >> dy)) {
		return canNotExtract;
	}
	return success;
}

void fillEmptyArray(vector<game_piece>& v, unsigned int a, unsigned int b) {
	game_piece emptyPiece(noColor, "", " ");
	for (unsigned int i = 0; i < a * b; ++i) {
		v.push_back(emptyPiece);
	}
}

//Here are the Game Base methods

shared_ptr<game_base> game_base::shared_p = nullptr;

void game_base::create_game(int a, char * arr[]) {
	if (shared_p != nullptr) {
		string s = "shared_p is not equal to nullptr";
		throw s.c_str();
	}
	else {
		if (expectedNumberOfArguments == a && arr[1] == string("NineAlmonds")) {
			nine_almonds* na = new nine_almonds();
			(*na).game_name = "Nine Almonds";
			shared_p.reset(na);
		}
		else if (expectedNumberOfArguments == a && arr[1] == string("MagicSquare")) {
			game_base* ms = new magic_square();
			(*ms).game_name = "Magic Square";
			shared_p.reset(ms);
		}
		else if (4 == a && arr[1] == string("Reversi")) {
			game_base* rv = new reversi(arr[2], arr[3]);
			(*rv).game_name = "Reversi";
			shared_p.reset(rv);
		}
		else {
			throw arr[1];
		}
	}
}

int game_base::play() {
	int numTurns = 0;
	try {
		this->setup();
	}
	catch (exitCodes e) {
		return quit;
	}
	while (true) {
		if (turn() == quit) {
			cout << "This game took " << numTurns << " turn(s) and finally the user quit (n00b)." << endl;
			cout << "Would you like to save the game? (YyNn)" << endl;
			string s = "";
			cin >> s;
			while (s != "y" && s != "Y" && s != "n" && s != "N") {
				cin >> s;
			}
			if (s == "y" || s == "Y") {
				ofstream ofs;
				if (game_name == "Nine Almonds") {
					ofs.open("Nine_Almonds_Game.txt");
				}
				else if (game_name == "Magic Square"){
					ofs.open("Magic_Square_Game.txt");
				}
				else if (game_name == "Reversi") {
					ofs.open("Reversi_Game.txt");
				}
				ofs << 1 << endl;
				if (!(ofs << dx << " " << dy << endl)) {
					cout << "Can not put dx and dy in " << endl;
					return quit;
				}
				if (game_name == "Magic Square") {
					for (unsigned int i = 0; i < v_pieces.size(); ++i) {
						if (v_pieces[i] != 0) {
							ofs << v_pieces[i] << " ";
						}
					}
					ofs << endl;
				}
				for (unsigned int i = 0; i < v.size(); ++i) {
					if (v[i].display != " ") {
						if (!(ofs << getColorString(v[i].color) << " " << v[i].name << " " << v[i].display << " " << (i%dy) << " " << (i / dy) << endl)) {
							cout << "Can not put in color display or x and y values" << endl;
							return quit;
						}
					}
				}
				ofs.close();
				return quit;
			}
			else if (s == "n" || s == "N") {
				ofstream ofs;
				if (game_name == "Nine Almonds") {
					ofs.open("Nine_Almonds_Game.txt");
				}
				else if(game_name == "Magic Square"){
					ofs.open("Magic_Square_Game.txt");
				}
				else if (game_name == "Reversi") {
					ofs.open("Reversi_Game.txt");
				}
				ofs << -1 << endl;
				ofs.close();
				return quit;
			}
		}
		if (done()) {
			cout << "This game took " << numTurns << " turn(s) and finally ended in a victory (took you long enough lol)!" << endl;
			return success;
		}
		if (stalemate()) {
			cout << "This game took " << numTurns << " turn(s) and resulted in a stalemate." << endl;
			return stalematee;
		}
		++numTurns;
	}
}

