/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Block.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 10:31 PM
 */

#include "Block.h"
#include "Command.h"

using namespace std;

Block::Block(vector<Variable*> variables, vector<Command*> commands) {
	this->variables = variables;
	this->commands = commands;
}

void Block::printBlock(){
	cout << "+++++++++ BLOCK +++++++++\n";
	cout << "VARIABLES DETECTED: " << variables.size() << endl << endl;
	for(int i = 0; i < variables.size(); i++){
		variables[i]->printVariable();
	}
	cout << "\nEND OF VARIABLES DETECTION\n\n";
	cout << "COMMANDS DETECTED: " << commands.size() << endl << endl;
	for(int i = 0; i < commands.size(); i++){
		commands[i]->printCommand();
	}
	cout << "\nEND OF COMMANDS DETECTION\n";
	cout << "--------- BLOCK ---------\n";
}

Block::~Block() {
}

