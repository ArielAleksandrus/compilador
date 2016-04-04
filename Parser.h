/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Parser.h
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 8:14 PM
 */

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>

#include "objects/Function.h"
#include "objects/Parameter.h"
#include "objects/Block.h"
#include "objects/Variable.h"
#include "objects/Command.h"
#include "objects/Expression.h"
#include "objects/FuncCall.h"
#include "SyntaticException.h"

#include "Tree.h"
#include "Token.h"


typedef struct ExprOrOpToken{
	Expression* expr;
	Token* op;
}ExprOrOpToken;

class Parser {
public:
	Parser(std::vector<Token*> tokens, Tree* t);
	virtual ~Parser();
private:
	std::vector<Token*> tokens;
	Function* checkFunction(int* position);
	Block* getBlock(int* position, std::vector<Token*> tokens);
	
	Expression* resolve(std::vector<Token*> tokens);
	Expression* resolve(std::vector<ExprOrOpToken*> expr_tokens);
	
	std::vector<Variable*> getVariableDeclarations(int* position,
		std::vector<Token*> tokens);
	Variable* getVariable(std::vector<Token*> tokens, int* pos);
	FuncCall* getFuncCall(std::vector<Token*> tokens, int* pos);
	
	std::vector<Command*> getCommands(int* position, std::vector<Token*> tokens,
		int max = -1);
};

#endif /* PARSER_H */

