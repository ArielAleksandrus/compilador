/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Token.h
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 11:41 AM
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "Unimplemented.h"
#include "FileReader.h"

class FileReader;
class SyntaticException;

class Token {
public:
	static const int OPERATOR = 1;
	static const int OPERATOR2 = 2;
	static const int NAME = 3;
	static const int LITERAL = 4;
	static const int TYPE = 5;
	static const int COMMAND = 6;
	
	// if you change these values, check for operator precedence levels below,
	// because it is hardcoded there! Couldn't use reference to these constants
	// for it gave me undefined reference error. Don't know why, though.
	static const int UNARY_OPERATOR = 1;
	static const int DUAL_OPERATOR = 2;
	static const int TERN_OPERATOR = 3;
	
	/**
	 * 
	 * @param line_number
	 * @param col_number
	 * @param lexem
	 * @param type One of the constants declared in this class.
	 */
	Token(int line_number, int col_number, std::string lexem, int type);
	
	std::string type_string();
	std::string operator_type_string();
	
	void printToken();
	virtual ~Token();
	
	int line_number, col_number, type;
	std::string lexem;
	
	bool is_op_type(int type);
private:
};

#endif /* TOKEN_H */

