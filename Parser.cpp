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
#include "objects/Variable.h"

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
			bool is_array = false;
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
				if(this->tokens[++pos]->lexem == "["){
					if(this->tokens[++pos]->lexem == "]"){
						is_array = true;
					} else {
						throw new SyntaticException(this->tokens[pos],
										"Expected ']', got: '" + this->tokens[pos]->lexem + "'");
					}
				} else {
					pos--;
				}
				args.push_back(new Argument(arg_type, arg_name, is_array));
				
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
	
	pos++;
	vector<Variable*> variables = getVariableDeclarations(&pos);
	pos--;
	while(this->tokens[++pos]->lexem != "}"){
		cout << "I Have: " << this->tokens[pos]->lexem << endl;
	}
	
	++pos;
	*position = pos;
	
	return NULL;
}

vector<Variable*> Parser::getVariableDeclarations(int* position) {
	vector<Variable*> variables;
	int pos = *position;
	// full declaration: type name ...
	while(true){
		// partial declaration: name, name ...
		Token* type = this->tokens[pos];
		if(type->type != Token::TYPE){
			break;
		}
		while(true){
			Token* name;
			int array_size = 0;
			// variable has a name. OK for now...
			if(this->tokens[++pos]->type == Token::NAME){
				name = this->tokens[pos];
				// it is an array. Now we can get it's size.
				if(this->tokens[++pos]->lexem == "["){
					// array size is a literal.
					if(this->tokens[++pos]->type == Token::LITERAL){
						try{
							array_size = stoi(this->tokens[pos]->lexem);
						} catch(exception& e){
							throw new SyntaticException(this->tokens[pos],
											"Unable to convert '" + this->tokens[pos]->lexem
											+ "' to integer");
						}
						// this variable is ok.
						if(this->tokens[++pos]->lexem == "]"){
							variables.push_back(new Variable(type, name, array_size));
						} else {
							throw new SyntaticException(this->tokens[pos],
											"Expected ']', got: " + this->tokens[pos]->lexem);
						}
						// it's an array of unknown size (bound to a variable, not a literal).
					} else if(this->tokens[pos]->type == Token::NAME){
						// this variable is ok.
						if(this->tokens[++pos]->lexem == "]"){
							variables.push_back(new Variable(type, name, -1));
						} else {
							throw new SyntaticException(this->tokens[pos],
											"Expected ']', got: " + this->tokens[pos]->lexem);
						}
					} else {
						throw new SyntaticException(this->tokens[pos],
										"Expected Name or Literal, got: "
										+ this->tokens[pos]->type_string());
					}
					// it's not an array
				} else {
					// will be initialized
					if(this->tokens[pos]->lexem == "="){
						// INCOMPLETE!!!
					} else {
						pos--;
					}
					variables.push_back(new Variable(type, name));
				}
				if(this->tokens[++pos]->lexem != ","){
					pos--;
					break;
				}
			} else {
				throw new SyntaticException(this->tokens[pos],
								"Expected Name. Found: " + this->tokens[pos]->type_string()
								+ " '" + this->tokens[pos]->lexem + "'");
			}
		}
		if(this->tokens[++pos]->lexem != ";"){
			throw new SyntaticException(this->tokens[pos],
							"Expected ';', got: " + this->tokens[pos]->lexem);
		}
		pos++;
	}
	
	*position = pos;
	
	return variables;
}


Parser::~Parser() {
}

