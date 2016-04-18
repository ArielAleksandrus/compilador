/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SymbolTable.cpp
 * Author: aleksandrus
 * 
 * Created on April 6, 2016, 1:56 AM
 */

#include "SymbolTable.h"

using namespace std;

SymbolTable::SymbolTable(){
	this->parent = NULL;
}
SymbolTable::SymbolTable(SymbolTable* parent) {
	this->parent = parent;
}

SymbolTable::~SymbolTable() {
}

void SymbolTable::printSymbolTable(){
	cout << "\n++++++ SYMBOL TABLE ++++++\n";
	cout << "++++++ SYMBOL TABLE FUNCTIONS ++++++\n";
	for(int i = 0; i < this->funcs.size(); i++)
		this->funcs[i]->printFunction();
	cout << "------ SYMBOL TABLE FUNCTIONS ------\n\n";
	cout << "++++++ SYMBOL TABLE PARAMETERS ++++++\n";
	for(int i = 0; i < this->params.size(); i++)
		this->params[i]->printParameter();
	cout << "------ SYMBOL TABLE PARAMETERS ------\n\n";
	cout << "++++++ SYMBOL TABLE VARS ++++++\n";
	for(int i = 0; i < this->vars.size(); i++)
		this->vars[i]->printVariable();
	cout << "------ SYMBOL TABLE VARS ------\n\n";
	cout << "++++++ SYMBOL TABLE HAS PARENT ++++++\n";
	string hp = this->parent == NULL ? "NO" : "YES";
	cout << hp << endl;
	cout << "------ SYMBOL TABLE HAS PARENT ------\n";
	cout << "\n------ SYMBOL TABLE ------\n";
}
