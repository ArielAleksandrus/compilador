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
        /**
         * Check if the tokens combination matches a function's declaration.
         * @param position Position in the token's array on which the analysis 
         * will start. If it is a function, 'position' gets updated.
         * @return Function object if it's a function. NULL if not a function
         */
	Function* checkFunction(int* position);
        /**
         * Check if the next tokens form a block. If so, returns a Block object.
         * Else, throws a Syntatic Exception
         * @param position Position in the token's array on which the analysis
         * will start. After the analysis, 'position' will be updated.
         * @param tokens array of tokens
         * @return Block object or throws syntactic exception
         */
	Block* getBlock(int* position, std::vector<Token*> tokens);
	
	Expression* resolve(std::vector<Token*> tokens, bool command_bool_expr = false);
	Expression* resolve(std::vector<ExprOrOpToken*> expr_tokens);
	
	std::vector<Variable*> getVariableDeclarations(int* position,
		std::vector<Token*> tokens, bool is_global = false);
	Variable* getVariable(std::vector<Token*> tokens, int* pos,
		bool is_global = false, bool from_command = false);
	FuncCall* getFuncCall(std::vector<Token*> tokens, int* pos);
	
	std::vector<Command*> getCommands(int* position, std::vector<Token*> tokens,
		int max = -1);
};

#endif /* PARSER_H */

