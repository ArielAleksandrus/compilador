/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Argument.h
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 8:36 PM
 */

#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>

#include "../Token.h"

class Argument {
public:
	Argument(Token* tipo, Token* nome);
	virtual ~Argument();
	
	Token* tipo;
	Token* nome;
private:
};

#endif /* ARGUMENT_H */

