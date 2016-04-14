/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FuncCall.h
 * Author: aleksandrus
 *
 * Created on March 22, 2016, 1:50 PM
 */

#ifndef FUNCCALL_H
#define FUNCCALL_H

#include "../circular_dep.h"

#include <string>
#include <vector>
#include <cstring>

#include "../Unimplemented.h"
#include "../SymbolTable.h"
#include "../Token.h"

#include "Expression.h"
#include "Function.h"

class FuncCall {
public:
	FuncCall(Token* func_name);
	FuncCall(Token* func_name, std::vector<Expression*> args);
	
	void printFuncCall();
	
	void semanticAnalysis(SymbolTable* st);
	
	Function* getFunctionDeclaration(SymbolTable* st);
	
	virtual ~FuncCall();
	
	Token* func_name;
	std::vector<Expression*> args;
private:

};

#endif /* FUNCCALL_H */

