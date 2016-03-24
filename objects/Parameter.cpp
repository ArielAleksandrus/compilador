/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Argument.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 8:36 PM
 */

#include "Parameter.h"

using namespace std;

Parameter::Parameter(Token* type, Token* name, bool is_array /* = false */) {
	this->type = type;
	this->name = name;
	this->is_array = is_array;
}

Parameter::~Parameter() {
}

