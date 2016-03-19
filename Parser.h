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
#include "Token.h"
#include "Tree.h"

class Parser {
public:
	Parser(std::vector<Token*> tokens, Tree* t);
	virtual ~Parser();
private:
	std::vector<Token*> tokens;
	Function* checkFunction(int* position);
	Block* getBlock(int* position);
	std::vector<Variable*> getVariableDeclarations(int* position);
};

#endif /* PARSER_H */

