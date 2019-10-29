#include "stdafx.h"
#include "game_board.h"
#include "arrayindicies.h"
#include "game_pieces.h"
#include "reversi_game.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

reversi::reversi(string a, string b) {
	player1 = a;
	player2 = b;
	playerTurn = 0;
}

void reversi::setup() {
	ifstream fileStream_("Reversi_Game.txt");
	if (!(fileStream_.is_open())) {
		cout << "Can't open the file! IS IT OPEN";
	}
	int a;
	fileStream_ >> a;
	if (a == -1) {
		fileStream_.close();
		ifstream fileStream("Reversi.txt");
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
		cout << "Loading saved game..." << endl;
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

ostream & operator<< (ostream &o, const reversi rv) {
	vector<game_piece> v_ = rv.v;
	unsigned int dy = rv.dy;
	unsigned int dx = rv.dx;
	for (int i = dy - 1; i >= 0; --i) {
		cout << i << " ";
		for (unsigned int j = 0; j < dx; ++j) {
			cout << v_[(dy * i) + j].display;
			for (unsigned int c = 0; c < rv.ls.length() + 2 - v_[(dy*i) + j].display.length(); ++c) {
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

bool checkValid(unsigned int attemptedPlayY, unsigned int attemptedPlayX, vector<game_piece> gameGrid, unsigned int sizeX, unsigned int sizeY, pieceColor currentColor, pieceColor otherPlayerColor)
{
	int attemptedPlayLocation = sizeY*attemptedPlayY + attemptedPlayX;
	game_piece pieceAtPlayLocation = gameGrid[attemptedPlayLocation];
	if (pieceAtPlayLocation.color != noColor)
	{
		return false;
	}
	//surrounding pieces
	bool isOnLeftEdge = (attemptedPlayX - 1) < 0;
	bool isOnRightEdge = (attemptedPlayX + 1) >= sizeX;
	bool isOnTopEdge = (attemptedPlayY - 1) < 0;
	bool isOnBottomEdge = (attemptedPlayY + 1) >= sizeY;
	int pieceToLeftLocation = (sizeY*attemptedPlayY) + (attemptedPlayX - 1);
	int pieceToRightLocation = (sizeY*attemptedPlayY) + (attemptedPlayX + 1);
	int pieceAboveLocation = (sizeY*(attemptedPlayY - 1)) + attemptedPlayX;
	int pieceBelowLocation = (sizeY*(attemptedPlayY + 1)) + attemptedPlayX;

	// horizontal left
	int leftCheckX = attemptedPlayX - 1;
	bool leftCheckIsOnLeftEdge = (leftCheckX) < 0;
	int leftCheckPieceLocation = (sizeY*attemptedPlayY) + (leftCheckX);
	while (!leftCheckIsOnLeftEdge && (gameGrid[leftCheckPieceLocation].color != noColor))
	{
		if (gameGrid[leftCheckPieceLocation].color == otherPlayerColor)
		{
			while (!leftCheckIsOnLeftEdge && gameGrid[leftCheckPieceLocation].color == otherPlayerColor)
			{
				leftCheckX = leftCheckX - 1;
				leftCheckIsOnLeftEdge = (leftCheckX) < 0;
				leftCheckPieceLocation = (sizeY*attemptedPlayY) + (leftCheckX);
			}

			if (!leftCheckIsOnLeftEdge && gameGrid[leftCheckPieceLocation].color == currentColor)
			{
				return true;
			}
			break;
		}

		leftCheckX = leftCheckX - 1;
		leftCheckIsOnLeftEdge = (leftCheckX) < 0;
		leftCheckPieceLocation = (sizeY*attemptedPlayY) + (leftCheckX);
	}

	// horizontal right
	unsigned int rightCheckX = attemptedPlayX + 1;
	bool rightCheckIsOnRightEdge = (rightCheckX) >= sizeX;
	int rightCheckPieceLocation = (sizeY*attemptedPlayY) + (rightCheckX);
	while (!rightCheckIsOnRightEdge && (gameGrid[rightCheckPieceLocation].color != noColor))
	{
		if (gameGrid[rightCheckPieceLocation].color == otherPlayerColor)
		{
			while (!rightCheckIsOnRightEdge && gameGrid[rightCheckPieceLocation].color == otherPlayerColor)
			{
				rightCheckX = rightCheckX + 1;
				rightCheckIsOnRightEdge = (rightCheckX) >= sizeX;
				rightCheckPieceLocation = (sizeY*attemptedPlayY) + (rightCheckX);
			}

			if (!rightCheckIsOnRightEdge && gameGrid[rightCheckPieceLocation].color == currentColor)
			{
				return true;
			}
			break;
		}

		rightCheckX = rightCheckX + 1;
		rightCheckIsOnRightEdge = (rightCheckX) >= sizeX;
		rightCheckPieceLocation = (sizeY*attemptedPlayY) + (rightCheckX);
	}
	// vertical above
	int aboveCheckY = attemptedPlayY - 1;
	bool aboveCheckIsOnTopEdge = (aboveCheckY) < 0;
	int aboveCheckPieceLocation = (sizeY*aboveCheckY) + (attemptedPlayX);
	while (!aboveCheckIsOnTopEdge && (gameGrid[aboveCheckPieceLocation].color != noColor))
	{
		if (gameGrid[aboveCheckPieceLocation].color == otherPlayerColor)
		{
			while (!aboveCheckIsOnTopEdge && gameGrid[aboveCheckPieceLocation].color == otherPlayerColor)
			{
				aboveCheckY = aboveCheckY - 1;
				aboveCheckIsOnTopEdge = (aboveCheckY) < 0;
				aboveCheckPieceLocation = (sizeY*aboveCheckY) + (attemptedPlayX);
			}

			if (!aboveCheckIsOnTopEdge && gameGrid[aboveCheckPieceLocation].color == currentColor)
			{
				return true;
			}
			break;
		}

		aboveCheckY = aboveCheckY - 1;
		aboveCheckIsOnTopEdge = (aboveCheckY) < 0;
		aboveCheckPieceLocation = (sizeY*aboveCheckY) + (attemptedPlayX);
	}

	// vertical below
	unsigned int belowCheckY = attemptedPlayY + 1;
	bool belowCheckIsOnBottomEdge = (belowCheckY) >= sizeY;
	int belowCheckPieceLocation = (sizeY*belowCheckY) + (attemptedPlayX);
	while (!belowCheckIsOnBottomEdge && (gameGrid[belowCheckPieceLocation].color != noColor))
	{
		if (gameGrid[belowCheckPieceLocation].color == otherPlayerColor)
		{
			while (!belowCheckIsOnBottomEdge && gameGrid[belowCheckPieceLocation].color == otherPlayerColor)
			{
				belowCheckY = belowCheckY + 1;
				belowCheckIsOnBottomEdge = (belowCheckY) >= sizeY;
				belowCheckPieceLocation = (sizeY*belowCheckY) + (attemptedPlayX);
			}

			if (!belowCheckIsOnBottomEdge && gameGrid[belowCheckPieceLocation].color == currentColor)
			{
				return true;
			}
			break;
		}

		belowCheckY = belowCheckY + 1;
		belowCheckIsOnBottomEdge = (belowCheckY) >= sizeY;
		belowCheckPieceLocation = (sizeY*belowCheckY) + (attemptedPlayX);
	}
	//diag above left
	int leftCheckX_dal = attemptedPlayX - 1;
	int aboveCheckY_dal = attemptedPlayY - 1;
	bool leftCheckIsOnLeftEdge_dal = (leftCheckX_dal) < 0;
	bool aboveCheckIsOnTopEdge_dal = (aboveCheckY_dal) < 0;
	int aboveLeftCheckPieceLocation = (sizeY*aboveCheckY_dal) + (leftCheckX_dal);
	while (!leftCheckIsOnLeftEdge_dal && !aboveCheckIsOnTopEdge_dal && (gameGrid[aboveLeftCheckPieceLocation].color != noColor))
	{
		if (gameGrid[aboveLeftCheckPieceLocation].color == otherPlayerColor)
		{
			while (!leftCheckIsOnLeftEdge_dal && !aboveCheckIsOnTopEdge_dal && gameGrid[aboveLeftCheckPieceLocation].color == otherPlayerColor)
			{
				leftCheckX_dal = leftCheckX_dal - 1;
				aboveCheckY_dal = aboveCheckY_dal - 1;
				leftCheckIsOnLeftEdge_dal = (leftCheckX_dal) < 0;
				aboveCheckIsOnTopEdge_dal = (aboveCheckY_dal) < 0;
				aboveLeftCheckPieceLocation = (sizeY*aboveCheckY_dal) + (leftCheckX_dal);
			}

			if (!leftCheckIsOnLeftEdge_dal && !aboveCheckIsOnTopEdge_dal && gameGrid[aboveLeftCheckPieceLocation].color == currentColor)
			{
				return true;
			}
			break;
		}

		leftCheckX_dal = leftCheckX_dal - 1;
		aboveCheckY_dal = aboveCheckY_dal - 1;
		leftCheckIsOnLeftEdge_dal = (leftCheckX_dal) < 0;
		aboveCheckIsOnTopEdge_dal = (aboveCheckY_dal) < 0;
		aboveLeftCheckPieceLocation = (sizeY*aboveCheckY_dal) + (leftCheckX_dal);
	}
	
	//diag above right
	unsigned int rightCheckX_dar = attemptedPlayX + 1;
	int aboveCheckY_dar = attemptedPlayY - 1;
	bool rightCheckIsOnRightEdge_dar = (rightCheckX_dar) >= sizeX;
	bool aboveCheckIsOnTopEdge_dar = (aboveCheckY_dar) < 0;
	int aboveRightCheckPieceLocation = (sizeY*aboveCheckY_dar) + (rightCheckX_dar);
	while (!rightCheckIsOnRightEdge_dar && !aboveCheckIsOnTopEdge_dar && (gameGrid[aboveRightCheckPieceLocation].color != noColor))
	{
		if (gameGrid[aboveRightCheckPieceLocation].color == otherPlayerColor)
		{
			while (!rightCheckIsOnRightEdge_dar && !aboveCheckIsOnTopEdge_dar && gameGrid[aboveRightCheckPieceLocation].color == otherPlayerColor)
			{
				rightCheckX_dar = rightCheckX_dar + 1;
				aboveCheckY_dar = aboveCheckY_dar - 1;
				rightCheckIsOnRightEdge_dar = (rightCheckX_dar) >= sizeX;
				aboveCheckIsOnTopEdge_dar = (aboveCheckY_dar) < 0;
				aboveRightCheckPieceLocation = (sizeY*aboveCheckY_dar) + (rightCheckX_dar);
			}
			if (!rightCheckIsOnRightEdge_dar && !aboveCheckIsOnTopEdge_dar && gameGrid[aboveRightCheckPieceLocation].color == currentColor)
			{
				return true;
			}
			break;
		}
		rightCheckX_dar = rightCheckX_dar + 1;
		aboveCheckY_dar = aboveCheckY_dar - 1;
		rightCheckIsOnRightEdge_dar = (rightCheckX_dar) >= sizeX;
		aboveCheckIsOnTopEdge_dar = (aboveCheckY_dar) < 0;
		aboveRightCheckPieceLocation = (sizeY*aboveCheckY_dar) + (rightCheckX_dar);	
	}

	//diag below left
	int leftCheckX_dbl = attemptedPlayX - 1;
	unsigned int belowCheckY_dbl = attemptedPlayY + 1;
	bool leftCheckIsOnLeftEdge_dbl = (leftCheckX_dbl) < 0;
	bool belowCheckIsOnBottomEdge_dbl = (belowCheckY_dbl) >= sizeY;
	int belowLeftCheckPieceLocation = (sizeY*belowCheckY_dbl) + (leftCheckX_dbl);
	while (!leftCheckIsOnLeftEdge_dbl && !belowCheckIsOnBottomEdge_dbl && (gameGrid[belowLeftCheckPieceLocation].color != noColor))
	{
		if (gameGrid[belowLeftCheckPieceLocation].color == otherPlayerColor)
		{
			while (!leftCheckIsOnLeftEdge_dbl && !belowCheckIsOnBottomEdge_dbl && gameGrid[belowLeftCheckPieceLocation].color == otherPlayerColor)
			{
				leftCheckX_dbl = leftCheckX_dbl - 1;
				belowCheckY_dbl = belowCheckY_dbl + 1;
				leftCheckIsOnLeftEdge_dbl = (leftCheckX_dbl) < 0;
				belowCheckIsOnBottomEdge_dbl = (belowCheckY_dbl) >= sizeY;
				belowLeftCheckPieceLocation = (sizeY*belowCheckY_dbl) + (leftCheckX_dbl);
			}

			if (!leftCheckIsOnLeftEdge_dbl && !belowCheckIsOnBottomEdge_dbl && gameGrid[belowLeftCheckPieceLocation].color == currentColor)
			{
				return true;
			}
			break;
		}

		leftCheckX_dbl = leftCheckX_dbl - 1;
		belowCheckY_dbl = belowCheckY_dbl + 1;
		leftCheckIsOnLeftEdge_dbl = (leftCheckX_dbl) < 0;
		belowCheckIsOnBottomEdge_dbl = (belowCheckY_dbl) >= sizeY;
		belowLeftCheckPieceLocation = (sizeY*belowCheckY_dbl) + (leftCheckX_dbl);
	}

	//diag below right
	unsigned int rightCheckX_dbr = attemptedPlayX + 1;
	unsigned int belowCheckY_dbr = attemptedPlayY + 1;
	bool rightCheckIsOnRightEdge_dbr = (rightCheckX_dbr) >= sizeX;
	bool belowCheckIsOnBottomEdge_dbr = (belowCheckY_dbr) >= sizeY;
	int belowRightCheckPieceLocation = (sizeY*belowCheckY_dbr) + (rightCheckX_dbr);
	while (!rightCheckIsOnRightEdge_dbr && !belowCheckIsOnBottomEdge_dbr && (gameGrid[belowRightCheckPieceLocation].color != noColor))
	{
		if (gameGrid[belowRightCheckPieceLocation].color == otherPlayerColor)
		{
			while (!rightCheckIsOnRightEdge_dbr && !belowCheckIsOnBottomEdge_dbr && gameGrid[belowRightCheckPieceLocation].color == otherPlayerColor)
			{
				rightCheckX_dbr = rightCheckX_dbr + 1;
				belowCheckY_dbr = belowCheckY_dbr + 1;
				rightCheckIsOnRightEdge_dbr = (rightCheckX_dbr) >= sizeX;
				belowCheckIsOnBottomEdge_dbr = (belowCheckY_dbr) >= sizeY;
				belowRightCheckPieceLocation = (sizeY*belowCheckY_dbr) + (rightCheckX_dbr);
			}

			if (!rightCheckIsOnRightEdge_dbr && !belowCheckIsOnBottomEdge_dbr && gameGrid[belowRightCheckPieceLocation].color == currentColor)
			{
				return true;
			}
			break;
		}

		rightCheckX_dbr = rightCheckX_dbr + 1;
		belowCheckY_dbr = belowCheckY_dbr + 1;
		rightCheckIsOnRightEdge_dbr = (rightCheckX_dbr) >= sizeX;
		belowCheckIsOnBottomEdge_dbr = (belowCheckY_dbr) >= sizeY;
		belowRightCheckPieceLocation = (sizeY*belowCheckY_dbr) + (rightCheckX_dbr);
	}
	return false;
}

bool reversi::done() {
	int blackCount = 0;
	int whiteCount = 0;
	int noColorCount = 0;
	string currPlayer;
	string otherPlayer;
	pieceColor currColor;
	pieceColor otherColor;
	string currDisplay;
	if (playerTurn == 0) {
		currPlayer = player1;
		otherPlayer = player2;
		currColor = black;
		otherColor = white;
		currDisplay = "X";
	}
	else {
		currPlayer = player2;
		otherPlayer = player1;
		currColor = white;
		otherColor = black;
		currDisplay = "O";
	}
	bool allFilled = true;
	for (unsigned int i = 0; i < this->v.size(); ++i) {
		if (getColorString(v[i].color) == "black") {
			++blackCount;
		}
		else if (getColorString(v[i].color) == "white") {
			++whiteCount;
		}
		if (v[i].color == noColor) {
			allFilled = false;
		}
	}
	if (whiteCount == 0 || blackCount == 0) {
		return true;
	}
	else if (whiteCount < blackCount || whiteCount > blackCount) {
		if (allFilled) {
			return true;
		}
		bool validMove = false;
		for (unsigned int i = 0; i < v.size(); ++i) {
			if (checkValid((i / dy), (i % dy), v, dx, dy, currColor, otherColor)) {
				validMove = true;
			}
			if (checkValid((i / dy), (i % dy), v, dx, dy, otherColor, currColor)) {
				validMove = true;
			}
		}
		return !validMove;
	}
	return false;
}

int reversi::turn() {
	print();
	string currPlayer;
	string otherPlayer;
	pieceColor currColor;
	pieceColor otherColor;
	string currDisplay;
	if (playerTurn == 0) {
		currPlayer = player1;
		otherPlayer = player2;
		currColor = black;
		otherColor = white;
		currDisplay = "X";
	}
	else {
		currPlayer = player2;
		otherPlayer = player1;
		currColor = white;
		otherColor = black;
		currDisplay = "O";
	}
	cout << currPlayer << " please take your turn before " << otherPlayer << " falls asleep.." << endl;
	bool validMove = false;
	for (unsigned int i = 0; i < v.size(); ++i) {
		if (checkValid((i / dy), (i % dy), v, dx, dy, currColor, otherColor)) {
			validMove = true;
		}
	}
	if (!validMove) {
		cout << "There is no valid move for this player" << endl;
		playerTurn = !playerTurn;
		return success;
	}
	else {
		unsigned int a;
		unsigned int b;
		if (prompt(a, b) == quit) {
			return quit;
		}
		while (a >= this->dx || b >= this->dy || this->v[this->dy*b + a].color != noColor || !checkValid(b, a, v, dx, dy, currColor, otherColor)) {
			cout << "INVALID MOVE!" << endl;
			if (prompt(a, b) == quit) {
				return quit;
			}
		}
		game_piece gp(currColor, "coin", currDisplay);
		v[dy*b + a] = gp;
		int counter = 1;
		int temp = a - counter;
		//left element
		if (((int)(dy*b) +temp) > 0) {
			while ((a - counter) > 0 && v[((dy*b) + a - counter)].color == otherColor) {
				++counter;
			};
			if (v[((dy*b) + a - counter)].color == currColor) {
				counter = 1;
				while (((dy*b) + a - counter) > 0 && v[((dy*b) + a - counter)].color == otherColor) {
					v[dy*b + a - counter] = gp;
					++counter;
				}
			}
		}
		unsigned int temp_ = a + counter;
		//right element
		counter = 1;
		if (((unsigned int)(dy*b) + temp_) < v.size()) {
			while (((dy*b) + a + counter) < dy*(b + 1) && v[((dy*b) + a + counter)].color == otherColor) {
				++counter;
			}
			if (v[((dy*b) + a + counter)].color == currColor) {
				counter = 1;
				while (((dy*b) + a + counter) > 0 && v[((dy*b) + a + counter)].color == otherColor) {
					v[dy*b + a + counter] = gp;
					++counter;
				}
			}
		}
		counter = 1;
		//up and to the right
		while (((dy*(b + counter)) + a + counter) < v.size())
		{
			int tmp = ((dy*(b + counter)) + a + counter);
			if (v[tmp].color == otherColor)
			{
				++counter;
				continue;
			}
			break;
		}
		unsigned int testt = ((dy*(b + counter)) + a + counter);
		if (testt < v.size()) {
			if (v[((dy*(b + counter)) + a + counter)].color == currColor)
			{
				counter = 1;
				while (((dy*(b + counter)) + a + counter) < v.size() && v[((dy*(b + counter)) + a + counter)].color == otherColor) {
					v[dy*(b + counter) + a + counter] = gp;
					++counter;
				}
			}
		}
		counter = 1;
		//element above
		while (((dy*(b + counter)) + a) < v.size())
		{
			int tmp = ((dy*(b + counter)) + a);
			if (v[tmp].color == otherColor)
			{
				++counter;
				continue;
			}
			break;
		}
		unsigned int testtt = ((dy*(b + counter)) + a);
		if (testtt < v.size()) {
			if (v[((dy*(b + counter)) + a)].color == currColor)
			{
				counter = 1;
				while (((dy*(b + counter)) + a) < v.size() && v[((dy*(b + counter)) + a)].color == otherColor) {
					v[dy*(b + counter) + a] = gp;
					++counter;
				}
			}
		}
		counter = 1;
		// up and left
		while (((dy*(b + counter)) + a - counter) < v.size())
		{
			int tmp = ((dy*(b + counter)) + a - counter);
			if (v[tmp].color == otherColor)
			{
				++counter;
				continue;
			}
			break;
		}
		testt = ((dy*(b + counter)) + a - counter);
		if (testt < v.size()) {
			if (v[((dy*(b + counter)) + a - counter)].color == currColor)
			{
				counter = 1;
				while (((dy*(b + counter)) + a - counter) < v.size() && v[((dy*(b + counter)) + a - counter)].color == otherColor) {
					v[dy*(b + counter) + a - counter] = gp;
					++counter;
				}
			}
		}
		counter = 1;
		//below and right
		while (((dy*(b - counter)) + a + counter) > 0)
		{
			int tmp = ((dy*(b - counter)) + a + counter);
			cout << tmp << endl;
			if (tmp < 0) {
				break;
			}
			if (v[tmp].color == otherColor)
			{
				++counter;
				continue;
			}
			break;
		}
		
		int test = ((dy*(b - counter)) + a + counter);
		if (test > 0) {
			if (v[((dy*(b - counter)) + a + counter)].color == currColor)
			{
				counter = 1;
				while (((dy*(b - counter)) + a + counter) > 0 && v[((dy*(b - counter)) + a + counter)].color == otherColor) {
					v[dy*(b - counter) + a + counter] = gp;
					++counter;
				}
			}
		}
		
		counter = 1;
		// below
		while (((dy*(b - counter)) + a) > 0) {
			int tmp = ((dy*(b - counter)) + a);
			if (tmp < 0) {
				break;
			}
			if (v[tmp].color == otherColor) {
				++counter;
				continue;
			}
			break;
		}
		int test2 = ((dy*(b - counter)) + a);
		if (test2 > 0)
		{
			if (v[test2].color == currColor) {
				counter = 1;
				while (((dy*(b - counter)) + a) > 0 && v[((dy*(b - counter)) + a)].color == otherColor) {
					v[dy*(b - counter) + a] = gp;
					++counter;
				}
			}
		}
		counter = 1;
		//below and left
		while (((dy*(b - counter)) + a - counter) > 0) {
			int tmp = ((dy*(b - counter)) + a - counter);
			if (tmp < 0) {
				break;
			}
			if (v[tmp].color == otherColor)
			{
				++counter;
				continue;
			}
			break;
		}
		int test3 = dy*(b - counter) + a - counter;
		if (test3 > 0)
		{
			if (v[test3].color == currColor) {
				counter = 1;
				while (((dy*(b - counter)) + a - counter) > 0 && v[((dy*(b - counter)) + a - counter)].color == otherColor) {
					v[dy*(b-counter) + a - counter] = gp;
					++counter;
				}
			}
		}
		playerTurn = !playerTurn;
		return success;
	}
}

int reversi::prompt(unsigned int &a, unsigned int &b) {
	bool returnValue = false;
	cout << "Please type 'quit' to end the game or a coordinate separated by a comma such as '0,0'" << endl;
	string s = "";
	cin >> s;
	if (s == "quit") {
		return quit;
	}
	while (s.length() != 3 || (s.substr(1, 1) != ",")) {
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

bool reversi::stalemate() {
	int blackCount = 0;
	int whiteCount = 0;
	string currPlayer;
	string otherPlayer;
	pieceColor currColor;
	pieceColor otherColor;
	string currDisplay;
	if (playerTurn == 0) {
		currPlayer = player1;
		otherPlayer = player2;
		currColor = black;
		otherColor = white;
		currDisplay = "X";
	}
	else {
		currPlayer = player2;
		otherPlayer = player1;
		currColor = white;
		otherColor = black;
		currDisplay = "O";
	}
	bool allFilled = true;
	for (unsigned int i = 0; i < this->v.size(); ++i) {
		if (getColorString(v[i].color) == "black") {
			++blackCount;
		}
		else if (getColorString(v[i].color) == "white") {
			++whiteCount;
		}
		if (v[i].color == noColor) {
			allFilled = false;
		}
	}
	if (blackCount != whiteCount) {
		return false;
	}
	else {
		if (allFilled) {
			return true;
		}
		bool validMove = false;
		for (unsigned int i = 0; i < v.size(); ++i) {
			if (checkValid((i / dy), (i % dy), v, dx, dy, currColor, otherColor)) {
				validMove = true;
			}
			if (checkValid((i / dy), (i % dy), v, dx, dy, otherColor, currColor)) {
				validMove = true;
			}
		}
		return !validMove;
	}
}

void reversi::print() {
	cout << endl << *this << endl << endl;
}