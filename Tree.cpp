/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tree.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 10:55 PM
 */

#include "Tree.h"

using namespace std;

Tree::Tree(Utils* u) {
	this->utils = u;
}

void Tree::printTree(){
	printFunctions();
	printGlobalVariables();
	printPrograma();
}

void Tree::printPrograma(){
	cout << "\nBlock 'programa':" << endl;
	programa->printBlock();
	cout << "End of block 'programa'" << endl << endl;
}

void Tree::printGlobalVariables(){
	if(globals.size() == 0){
		cout << "No global variables declared" << endl;
	}
	cout << "Global variables: " << globals.size() << endl;
	for(int i = 0; i < globals.size(); i++){
		globals[i]->printVariable();
	}
}

void Tree::printFunctions(){
	if(this->functions.size() == 0){
		cout << "No functions to show." << endl;
		return;
	}
	cout << "We have " << this->functions.size() << " functions" << endl;
	cout << "\tFunction <number>: <type> <name>(<args>)" << endl;
	for(int i = 0; i < this->functions.size(); i++){
		Function* f = this->functions[i];
		cout << "\tFunction " << i+1 << ": " << f->type->lexem << " "
						<< f->name->lexem << " (";
		for(int j = 0; j < f->arguments.size(); j++){
			Parameter* a = f->arguments[j];
			cout << a->type->lexem << " " << a->name->lexem;
			if(a->is_array)
				cout << "[]";
			if(j != f->arguments.size() - 1)
				cout << ", ";
		}
		cout << ");" << endl;
	}
}

void Tree::semanticAnalysis() {
	SymbolTable* table = new SymbolTable();
	
	this->utils->out << ".text" << endl;
	this->utils->out << ".globl main" << endl;
	
	
	for(int i = 0; i < globals.size(); i++){
		globals[i]->semanticAnalysis(table);
	}

	for(int i = 0; i < functions.size(); i++)
		functions[i]->semanticAnalysis(table);

	// allocate memory for global variables.
	this->utils->out << "addi $sp, $sp, -" << table->varAlloc << endl;
	// jump to "programa" block.
	this->utils->out << "jal programa" << endl;
	
	// "programa" block:
	this->utils->out << "programa:" << endl;
	programa->semanticAnalysis(table);
	// required to exit program when using "spim".
	this->utils->out << "jr $ra" << endl;
}


Tree::~Tree() {
}

