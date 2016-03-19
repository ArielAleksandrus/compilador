/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Block.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 10:31 PM
 */

#include "Block.h"

using namespace std;

Block::Block(vector<Variable*> variables) {
	this->variables = variables;
}

Block::~Block() {
}

