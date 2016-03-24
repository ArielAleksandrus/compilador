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

Variable::Variable(Token* name, bool is_array /* = false */,
				Expression* expr /* = NULL */, Expression* expr2 /* = NULL */){
	this->name = name;
	if(is_array)
		this->arr_size = expr;
	else
		this->value = expr;
}

Variable::Variable(Token* type, Token* name, bool is_array /* = false */,
				Expression* expr /* = NULL */){
	this->type = type;
	this->name = name;
	
	if(is_array){
		if(expr != NULL)
			this->arr_size = expr;
		else 
			throw new SyntaticException(type, "Invalid use of Variable constructor");
	} else {
		this->value = expr;
	}
}

Variable::~Variable() {
	
}

void Variable::printVariable(){
	cout << "+++++++ VARIABLE +++++++" << endl;
	if(type != NULL)
		cout << "Type: " + type->lexem << endl;
	cout << "Name: " + name->lexem << endl << endl;
	if(arr_size != NULL){
		cout << "+++++ ARR SIZE +++++" << endl;
		arr_size->printExpression();
		cout << "----- ARR SIZE -----" << endl;
	}
	if(arr_pos != NULL){
		cout << "+++++ ARR POS +++++" << endl;
		arr_pos->printExpression();
		cout << "----- ARR POS -----" << endl;
	}
	if(value != NULL){
		cout << "+++++ VALUE +++++" << endl;
		value->printExpression();
		cout << "----- VALUE -----" << endl;
	}
	cout << "------- VARIABLE -------" << endl;
}
