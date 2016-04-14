/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SemanticException.h
 * Author: aleksandrus
 *
 * Created on April 6, 2016, 10:50 AM
 */

#ifndef SEMANTICEXCEPTION_H
#define SEMANTICEXCEPTION_H

#include <string>
#include <iostream>

#include "circular_dep.h"

#include "Token.h"
#include "Unimplemented.h"

class SemanticException : public std::exception{
public:
	static const int PREVIOUSLY_FOUND = 0;
	static const int NOT_FOUND = 1;
	static const int CONFLICTING_TYPES = 2;
	static const int GENERIC = 3;
	
	SemanticException(int type, Token* token, Token* aux_token, std::string msg,
		bool terminate = true);
	SemanticException(Token* token, std::string msg, bool terminate = true);
	virtual ~SemanticException();
private:

};

#endif /* SEMANTICEXCEPTION_H */

