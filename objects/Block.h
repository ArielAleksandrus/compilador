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

#include <iostream>
#include <vector>
#include <string>

#include "Variable.h"
#include "Command.h"


class Command;
class Block {
public:
	Block(std::vector<Variable*> variables, std::vector<Command*> commands);
	virtual ~Block();
	
	void printBlock();
private:
	std::vector<Variable*> variables;
	std::vector<Command*> commands;
};

#endif /* BLOCK_H */

