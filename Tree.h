/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tree.h
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 10:55 PM
 */

#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include <string>

#include "objects/Function.h"
#include "objects/Argument.h"
#include "objects/Block.h"

class Tree {
public:
	Tree();
	virtual ~Tree();
	
	void printFunctions();
	
	std::vector<Function*> functions;
	Block* programa;
private:
};

#endif /* TREE_H */

