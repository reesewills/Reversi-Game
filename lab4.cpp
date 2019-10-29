//File Name: lab4.cpp
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining the main function for the lab, the place that all things are run from

#include "stdafx.h"
#include "arrayindicies.h"
#include "game_pieces.h"
#include "game_board.h"
#include "reversi_game.h"
#include "nine_almonds.h"
//#include "magic_square.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
using namespace std;

exitCodes usageMessage(char* programName) {
	//prints out how to use the function
	cout << "usage: " << programName << " NineAlmonds or " << programName << " MagicSquare or " << programName << " Reversi" << endl;
	return invalidNumberCommandLine;
}

int main(int argc, char * argv[]) {
	// if we do not have only 1 paramater with file name, return an error
	//shared_ptr<game_base> na_pointer(game_base::create_game(argc, argv));
	try {
		game_base::create_game(argc, argv);
		game_base* gbp = game_base::instance().get();
		return (*gbp).play();
		return 0;
	}
	catch(char* c){
		cout << c << endl;
		string s = "shared_p is not equal to nullptr";
		if (c == s.c_str()) {
			return nullptrError;
		}
		else {
			return usageMessage(argv[programName]);
		}
	}
	catch (bad_alloc) {
		cout << "Bad memory allocation" << endl;
	}
	catch (...) {
		return unhandledException;
	}
}
