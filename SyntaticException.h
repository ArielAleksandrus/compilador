/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SyntaticException.h
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 8:54 PM
 */

#ifndef SYNTATICEXCEPTION_H
#define SYNTATICEXCEPTION_H

#include <iostream>
#include <string>

#include "Token.h"
#include "objects/Expression.h"

class Expression;
class Token;

class SyntaticException : public std::exception{
public:
	SyntaticException(Expression* expr, std::string msg);
	SyntaticException(Token* token, std::string msg);
	SyntaticException(int line_number, int col_number, std::string msg,
					bool terminate = true);
	SyntaticException(int line_number, std::string msg, bool terminate = true);
	virtual ~SyntaticException();
private:

};

#endif /* SYNTATICEXCEPTION_H */

