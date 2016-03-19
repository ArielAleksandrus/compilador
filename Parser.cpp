/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Parser.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 8:14 PM
 */

#include <vector>

#include "Parser.h"
#include "SyntaticException.h"
#include "Tree.h"

using namespace std;

Parser::Parser(vector<Token*> tokens, Tree* t) {
	this->tokens = tokens;
	int pos = 0;
	while(tokens[pos]->type == Token::TYPE)
		t->functions.push_back(checkFunction(&pos));
	
	if(tokens[pos]->lexem == "programa"){
		// get block programa
	} else {
		throw new SyntaticException(tokens[pos],
						"Expected function or 'programa' block, got: '" + tokens[pos]->lexem + "'");
	}
	
	cout << "Position: " << pos << endl;
	
}

Function* Parser::checkFunction(int* position){
	Token *func_type, *func_name;
	vector<Argument*> args;
	int pos = *position;
	// should be a function
	if(this->tokens[pos]->type == Token::TYPE){
		func_type = this->tokens[pos];
		if(func_name = this->tokens[++pos], this->tokens[pos]->type == Token::NAME && this->tokens[++pos]->lexem == "("){
			// Now we will check for the argument's list.
			Token *arg_type, *arg_name;
			
			// Get function's arguments
			while(true){
				if(this->tokens[++pos]->lexem == ")" )
					break;
				if(this->tokens[pos]->lexem == ",")
					++pos;
				
				arg_type = this->tokens[pos];
				if(this->tokens[pos]->type != Token::TYPE)
					throw new SyntaticException(this->tokens[pos],
									"Should be Type. Found: " + this->tokens[pos]->type_string()
									+ " '" + this->tokens[pos]->lexem + "'");
				
				arg_name = this->tokens[++pos];
				if(this->tokens[pos]->type != Token::NAME){
					throw new SyntaticException(this->tokens[pos],
									"Should be Name. Found: " + this->tokens[pos]->type_string()
									+ " '" + this->tokens[pos]->lexem + "'");
				}
				args.push_back(new Argument(arg_type, arg_name));
				
			}
		}
	}
	++pos;
	*position = pos;
	return new Function(func_type, func_name, getBlock(position), args);
}

// INCOMPLETE!!!
Block* Parser::getBlock(int* position){
	int pos = *position;
	if(this->tokens[pos]->lexem != "{"){
		throw new SyntaticException(this->tokens[pos],
						"Should be '{'. Found: '" + this->tokens[pos]->lexem + "'");
	}
	
	while(this->tokens[++pos]->lexem != "}"){
		
	}
	
	++pos;
	*position = pos;
	
	return NULL;
}

Parser::~Parser() {
}

