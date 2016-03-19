/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Function.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 8:33 PM
 */

#include "Function.h"

using namespace std;

Function::Function(Token* type, Token* name, Block* block, vector<Argument*> arguments) {
	this->type = type;
	this->name = name;
	this->arguments = arguments;
}

Function::~Function() {
}

