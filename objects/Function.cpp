/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Function.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 8:33 PM
 */

#include "Function.h"

using namespace std;

Function::Function(Token* type, Token* name, Block* block, vector<Parameter*> arguments) {
	this->type = type;
	this->name = name;
	this->arguments = arguments;
	this->block = block;
}

Function::~Function() {
}

void Function::semanticAnalysis(SymbolTable* st){
	SymbolTable* local = new SymbolTable();
	local->parent = st;
	
	for(int i = 0; i < arguments.size(); i++)
		arguments[i]->semanticAnalysis(local);
	
	for(int i = 0; i < st->funcs.size(); i++){
		if(st->funcs[i]->name->lexem == name->lexem)
			throw new SemanticException(SemanticException::PREVIOUSLY_FOUND,
							name, st->funcs[i]->name, "Redeclaration of function");
	}
	
	st->funcs.push_back(this);
	block->semanticAnalysis(local);
}