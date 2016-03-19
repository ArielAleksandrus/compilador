/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Variable.cpp
 * Author: aleksandrus
 * 
 * Created on March 19, 2016, 12:21 AM
 */

#include "Variable.h"

using namespace std;

Variable::Variable(Token* type, Token* name, int arr_size /* = 0 */,
				Token* value /* = NULL */) {
	this->type = type;
	this->name = name;
	this->arr_size = arr_size;
	this->value = value;
	
	if(type->lexem == "int")
		this->var_size = 4;
	else if(type->lexem == "car")
		this->var_size = 1;
	else
		throw new Unimplemented("Should implement var size for: " + type->lexem);
	
	this->var_size = arr_size == 0 ? this->var_size : this->var_size * arr_size;
}

Variable::~Variable() {
}

