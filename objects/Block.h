/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Block.h
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 10:31 PM
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>

#include "Block.h"
#include "Variable.h"

class Block {
public:
	Block(std::vector<Variable*> variables);
	virtual ~Block();
private:
	std::vector<Variable*> variables;
};

#endif /* BLOCK_H */

