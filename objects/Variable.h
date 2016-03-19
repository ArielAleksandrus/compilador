/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Variable.h
 * Author: aleksandrus
 *
 * Created on March 19, 2016, 12:21 AM
 */

#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

#include "../Token.h"
#include "../Unimplemented.h"

class Variable {
public:
	static const int INT_SIZE = 4;
	static const int CAR_SIZE = 1;
	
	/**
	 * 
	 *  @param type
	 * @param name
	 * @param arr_size 0 if not an array. -1 for indefinite size.
	 */
	Variable(Token* type, Token* name, int arr_size = 0, Token* value = NULL);
	virtual ~Variable();
	
	Token *type, *name, *value;
	int arr_size, var_size;
private:

};

#endif /* VARIABLE_H */

