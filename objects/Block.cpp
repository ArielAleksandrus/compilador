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

void Block::semanticAnalysis(SymbolTable* st, Function* parent /* = NULL */ ){
	SymbolTable* local = new SymbolTable(st);
	
	for(int i = 0; i < variables.size(); i++)
		variables[i]->semanticAnalysis(local);
	
	for(int i = 0; i < commands.size(); i++){
		commands[i]->semanticAnalysis(local);

	}
	if(parent != NULL){
		int retorne_count = 0;
		for(int i = 0; i < commands.size(); i++){
			if(commands[i]->name->lexem == "retorne"){
				retorne_count++;
				string expr_type = commands[i]->val->getExpressionTypeString(local);
				transform(expr_type.begin(), expr_type.end(), expr_type.begin(), ::tolower);
				if(expr_type != parent->type->lexem)
					throw new SemanticException(commands[i]->val->getAToken(),
									"Function type is '" + parent->type->lexem
									+ "' but told to return '" + expr_type + "'");
			}
		}
		if(retorne_count == 0)
			throw new SemanticException(parent->name,
							"No command 'retorne' found for this function");
	}
		
}

Block::~Block() {
}

