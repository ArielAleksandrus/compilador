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
	
	
	/* Operator precedence.
	 * integer value = operator type (unary, dual, tern)
	 * string value = lexem
	 *  */
	/* parenthesis are evaluated on sight */
	/*std::map<int, std::string> levelMax = {
		{-1, "("},
		{-1, ")"},
	};*/
	/* Unary operators should be evaluated first, after parenthesis are processed */
	/*std::map<int, std::string> level5 = {
		{1, "-"},
		{1, "!"},
		{1, "nao"}
	};*/
	/* Ternary operators should be evaluated secondly */
	/*std::map<int, std::string> level4 = {
		{3, "?"}
	};
	std::map<int, std::string> level3 = {
		{2, ">="},
		{2, "<="},
		{2, ">"},
		{2, "<"},
		{2, "=="},
		{2, "!="},
		{2, "e"},
		{2, "ou"},
		{2, "&&"},
		{2, "||"},
	};
	std::map<int, std::string> level2 = {
		{2, "*"},
		{2, "/"},
		{2, "%"},
	};
	std::map<int, std::string> level1 = {
		{2, "+"},
		{2, "-"},
	};*/
	
	/**
	 * 
	 * @param line_number
	 * @param col_number
	 * @param lexem
	 * @param type One of the constants declared in this class.
	 * @param operator_type The type of operator. 0 if not an operator or
	 * operator does not have a particular type assigned.
	 */
	Token(int line_number, int col_number, std::string lexem, int type,
	int operator_type = 0);
	
	std::string type_string();
	std::string operator_type_string();
	
	void printToken();
	virtual ~Token();
	
																			/* consider removing operator_type */
	int line_number, col_number, type, operator_type;
	std::string lexem;
	
	bool is_op_type(int type);
private:
};

#endif /* TOKEN_H */

