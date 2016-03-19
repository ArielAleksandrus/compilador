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

#include "Argument.h"

using namespace std;

Argument::Argument(Token* tipo, Token* nome) {
	this->tipo = tipo;
	this->nome = nome;
}

Argument::~Argument() {
}

