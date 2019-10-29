#pragma once
//File Name: arrayinices.h
//Author: Reese Wills, reesewills@wustl.edu, 448128
//(Lab 4) defining functions and structures for the game board
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "game_pieces.h"
using namespace std;

enum programArgs { programName = 0, fileName = 1};

enum exitCodes {
	success = 1, expectedNumberOfArguments = 2, invalidNumberCommandLine = 3, canNotGetLine = 4, canNotExtract = 5, notWellFormed = 6, unableToOpenFile = 7, wrongDxDy = 8, quit = 9, wrongFormat = 10, invalidMove = 11, stalematee = 12, nullptrError = 13, unhandledException = 14
};

exitCodes usageMessage(char* programName);

string allLower(string s);
