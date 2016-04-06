/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Argument.h
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 8:36 PM
 */

#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>

#include "../circular_dep.h"

#include "../SemanticException.h"
#include "../SymbolTable.h"
#include "../Token.h"

class Parameter {
public:
	Parameter(Token* type, Token* name, bool is_array = false);
	virtual ~Parameter();
	
	void printParameter();
	
	void semanticAnalysis(SymbolTable* st);
	
	Token *type, *name;
	bool is_array;
private:
};

#endif /* ARGUMENT_H */

