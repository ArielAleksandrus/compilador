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

class Token {
public:
	static const int OPERATOR = 1;
	static const int OPERATOR2 = 2;
	static const int NAME = 3;
	static const int LITERAL = 4;
	static const int TYPE = 5;
	static const int COMMAND = 6;
	/**
	 * 
	 * @param line_number
	 * @param col_number
	 * @param lexem
	 * @param type One of the constants declared in this class.
	 */
	Token(int line_number, int col_number, std::string lexem, int type);
	virtual ~Token();
	
	
	int line_number;
	int col_number;
	std::string lexem;
	int type;
private:
};

#endif /* TOKEN_H */

