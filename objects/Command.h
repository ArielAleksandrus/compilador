/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Command.h
 * Author: aleksandrus
 *
 * Created on March 19, 2016, 2:32 AM
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <vector>
#include <string>

#include "../circular_dep.h"

#include "../Token.h"
#include "../Unimplemented.h"

#include "Expression.h"
#include "Block.h"

class Command {
public:
	/**
	 * Useful for emtpy command, like ';' in se(expr) then ;
	 */
	Command();
	/**
	 * Useful for the simplest of commands.\n
	 * Example:\n
	 * 1) novalinha\n
	 * 
	 * @param name Name of the command.
	 */
	Command(Token* name);
	/**
	 * This constructor is useful for using the instance returned as aux1 or aux2 
	 * of the next constructors and to use for assignments/function calls.\n
	 * Example:\n
	 * 1) a = 3 + 5;\n
	 * 2) init(1);\n
	 * 
	 * @param val Expression to be executed.
	 */
	Command(Expression* val);
	/**
	 * This constructor is useful for using the instance returned as aux1 or aux2 
	 * of the next constructors.\n
	 * 
	 * @param body Block to be executed.
	 */
	Command(Block* body);
	/**
	 * Useful for commands that can be accompanied by an expression.\n
	 * Example:\n
	 * 1) escreva "ariel";\n
	 * 2) retorne 4 * a;\n
	 * 
	 * @param name Name of the command.
	 * @param val Expression that is assigned to the command.
	 */
	Command(Token* name, Expression* val);
	/**
	 * Useful for commands that can be accompanied by a variable.\n
	 * Example:\n
	 * 1) leia x;\n
	 * 2) leia y[3];\n
	 * 
	 * @param name Name of the command.
	 * @param lval Variable that is assigned to the command.
	 */
	Command(Token* name, Variable* lval);
	/**
	 * Useful for commands that can be accompanied by a boolean expression to be
	 * evaluated and then by a block or exception or variable or another command,
	 * and so on so forth.\n
	 * Example:\n
	 * 1) enquanto (a > 3) a = a - 1;\n
	 * 2) enquanto (a > 3) { int b; a = a - 1; escreva a; leia b; a = a - b; }\n
	 * 
	 * @param name Name of the command.
	 * @param val Expression that is assigned to the command. In some cases, may
	 * be the boolean expression to be evaluated.
	 * @param aux1 Command to be run depending on the result of the expression.
	 */
	Command(Token* name, Expression* val, Command* aux1);
	/**
	 * Useful for commands that can be accompanied by another command.\n
	 * Example:\n
	 * 1) se (x > 0) entao retorne x;\n
	 * 
	 * @param name Name of the command.
	 * @param val Expression that is assigned to the command. In some cases, may
	 * be the boolean expression to be evaluated.
	 * @param aux_name1 Name of a supported extension to the original command,
	 * like the command "entao" that is followed by "se".
	 */
	Command(Token* name, Expression* val, Token* aux_name1, Command* aux1);
	/**
	 * Useful for commands that can be accompanied by two other commands.\n
	 * Example:\n
	 * 1) se (x > 0) entao retorne x; senao retorne -x;\n
	 * 
	 * @param name Name of the command.
	 * @param val Expression that is assigned to the command. In some cases, may
	 * be the boolean expression to be evaluated.
	 * @param aux_name1 Name of a supported extension to the original command,
	 * like the command "entao" that is followed by "se".
	 */
	Command(Token* name, Expression* val, Token* aux_name1, Command* aux1,
		Token* aux_name2, Command* aux2);
	
	void semanticAnalysis(SymbolTable* st);
	
	void printCommand();
	
	virtual ~Command();
	
	// aux_name is used for "se (Expression) entao Command senao Command" or
	// similar cases. aux_name1 would be entao, aux_name2 would be senao.
	Token *name = NULL, *aux_name1 = NULL, *aux_name2 = NULL;
	Expression* val = NULL;
	Variable* lval = NULL;
	Command *aux1 = NULL, *aux2 = NULL;
	Block* body = NULL;
private:
	
};

#endif /* COMMAND_H */

