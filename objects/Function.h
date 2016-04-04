/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Function.h
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 8:33 PM
 */

#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <string>

#include "../Token.h"
#include "Parameter.h"
#include "Block.h"

class Function {
public:
	Function(Token* type, Token* name, Block* block, std::vector<Parameter*> arguments);
	virtual ~Function();
	
	Token *type, *name;
	Block *block;
	std::vector<Parameter*> arguments;
private:

};

#endif /* FUNCTION_H */

