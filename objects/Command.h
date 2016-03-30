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

#include <vector>
#include <string>
#include "../Token.h"
#include "Expression.h"
#include "Block.h"

class Command {
public:
	Command(Token* name);
	Command(Expression* val);
	Command(Block* body);
	Command(Token* name, Expression* val);
	Command(Token* name, Variable* lval);
	Command(Token* name, Expression* val, Token* aux_name1, Command* aux1);
	Command(Token* name, Expression* val, Token* aux_name1, Command* aux1,
	Token* aux_name2, Command* aux2);
	
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

