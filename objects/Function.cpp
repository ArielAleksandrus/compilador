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
	SymbolTable* local = new SymbolTable(st);
	
	for(int i = 0; i < arguments.size(); i++)
		arguments[i]->semanticAnalysis(local);
	
	for(int i = 0; i < st->funcs.size(); i++){
		if(st->funcs[i]->name->lexem == name->lexem){
			if(st->funcs[i]->arguments.size() == arguments.size()){
				bool match = true;
				for(int j = 0; j < arguments.size(); j++){
					if(st->funcs[i]->arguments[j]->type->lexem != arguments[j]->type->lexem
									|| st->funcs[i]->arguments[j]->is_array != arguments[j]->is_array)
						match = false;
				}
				if(match){
					string msg = "Redeclaration of function " + name->lexem + "(";
					for(int j = 0; j < arguments.size(); j++){
						msg += arguments[j]->type->lexem;
						msg += arguments[j]->is_array ? "[]" : "";
						if(j < arguments.size() - 1)
							msg += ", ";
					}
					msg += "). Previously found in line "
									+ to_string(st->funcs[i]->name->line_number) + " col "
									+ to_string(st->funcs[i]->name->col_number);
					throw new SemanticException(this->name, msg);
				}
			}
		}
	}
	
	st->funcs.push_back(this);
	block->semanticAnalysis(local);
}