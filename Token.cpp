/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Token.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 11:41 AM
 */

#include "Token.h"
#include "Unimplemented.h"

using namespace std;

Token::Token(int line_number, int col_number, string lexem, int type){
	this->line_number = line_number;
	this->col_number = col_number;
	this->lexem = lexem;
	this->type = type;
}

string Token::type_string(){
	switch(this->type){
		case Token::OPERATOR:
			return "Operator";
		case Token::OPERATOR2:
			return "Double operator";
		case Token::NAME:
			return "Name";
		case Token::LITERAL:
			return "Literal";
		case Token::TYPE:
			return "Type";
		case Token::COMMAND:
			return "Command";
		default:
			throw new Unimplemented("Should implement string value for Token type: "
							+ to_string(this->type));
	}
}

Token::~Token() {
}

