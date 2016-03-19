/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 11:06 AM
 */

#include <iostream>
#include <cstdlib>

#include "FileReader.h"
#include "Parser.h"
#include "Tree.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	if(argc < 2){
		cout << "Missing file as argument" << endl;
		exit(1);
	}
	FileReader* fr = new FileReader(argv[1]);
	fr->printTokens();
	
	Tree* t = new Tree();
	Parser* p = new Parser(fr->getTokens(), t);
	t->printFunctions();
	return 0;
}

