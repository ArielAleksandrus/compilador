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

#include "circular_dep.h"

#include "FileReader.h"
#include "Parser.h"
#include "Tree.h"

using namespace std;

typedef struct Options{
	char fileName[256];
	bool printTokens;
	bool printTree;
	bool printGlobals;
	bool printFunctions;
	bool terminate;
}Options;

Options* handleArgs(int argc, char** argv){
	Options* o = (Options*) calloc(1, sizeof(Options));
	strncpy(o->fileName, argv[1], 256);
	for(int i = 2; i <= argc; i++){
		if(strcmp(argv[i - 1], "-h") == 0){
			cout << "Usage:\n";
			cout << "First argument should be source file's path or -h alone.\n";
			cout << "\t-h\t\tPrint available options.\n";
			cout << "\t-pto\t\tPrint recognized tokens.\n";
			cout << "\t-ptr\t\tPrint elements's tree.\n";
			cout << "\t-pg\t\tPrint global variables.\n";
			cout << "\t-pf\t\tPrint functions.\n";
			cout << "\t-pa\t\tPrint all.\n";
			o->terminate = true;
		}
		if(strcmp(argv[i - 1], "-pto") == 0){
			o->printTokens = true;
		}
		if(strcmp(argv[i - 1], "-ptr") == 0){
			o->printTree = true;
		}
		if(strcmp(argv[i - 1], "-pg") == 0){
			o->printGlobals = true;
		}
		if(strcmp(argv[i - 1], "-pf") == 0){
			o->printFunctions = true;
		}
		if(strcmp(argv[i - 1], "-pa") == 0){
			o->printTokens = true;
			o->printTree = true;
			o->printGlobals = true;
			o->printFunctions = true;
		}
	}
	
	return o;
}

int main(int argc, char** argv) {
	if(argc < 2){
		cout << "Missing file as argument" << endl;
		exit(1);
	}
	Options* o = handleArgs(argc, argv);
	if(o->terminate)
		return 0;
	
	FileReader* fr = new FileReader(argv[1]);
	
	if(o->printTokens)
		fr->printTokens();
	
	Utils u;
	u.out.open("a.out");
	
	Tree* t = new Tree(&u);
	Parser* p = new Parser(fr->getTokens(), t);
	
	if(o->printTree)
		t->printTree();
	else {
		if(o->printGlobals)
			t->printGlobalVariables();
		if(o->printFunctions)
			t->printFunctions();
	}
	
	t->semanticAnalysis();
	u.out.close();
	return 0;
}

