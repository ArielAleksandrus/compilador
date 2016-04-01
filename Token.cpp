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

#include <map>

#include "Token.h"

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
		case Token::OPERATOR2:
			return "Operator";
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

bool Token::is_op_type(int type){
	if(this->type != Token::OPERATOR && this->type != Token::OPERATOR2)
		return 0;
	
	FileReader* fr = new FileReader();
	if(type == Token::UNARY_OPERATOR){
		for(int i = 0; i < fr->unary_operators.size(); i++){
			if(fr->unary_operators[i] == this->lexem){
				delete fr;
				return true;
			}
		}
	} else if(type == Token::DUAL_OPERATOR){
		for(int i = 0; i < fr->dual_operators.size(); i++){
			if(fr->dual_operators[i] == this->lexem){
				delete fr;
				return true;
			}
		}
	} else if(type == Token::TERN_OPERATOR){
		for(int i = 0; i < fr->ternary_operators.size(); i++){
			if(fr->ternary_operators[i] == this->lexem){
				delete fr;
				return true;
			}
		}
	} else {
		delete fr;
		return true;
	}
	
	delete fr;
	return false;
}

void Token::printToken(){
	cout << "Token:"
					<< "\n\tType: " << this->type_string()
					<< "\n\tLine/Col: " << this->line_number << ", " << this->col_number
					<< "\n\tLexem: " << this->lexem
					<< endl;
}

Token::~Token() {
}

