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
#include <cstdlib>

#include "circular_dep.h"

#include "SymbolTable.h"

#include "objects/Function.h"
#include "objects/Parameter.h"
#include "objects/Block.h"
#include "objects/Variable.h"

class Tree {
public:
	Tree(Utils* u);
	virtual ~Tree();
	
	void semanticAnalysis();
	
	void printTree();
	void printFunctions();
	void printGlobalVariables();
	void printPrograma();
	
	std::vector<Function*> functions;
	std::vector<Variable*> globals;
	Block* programa;
private:
    Utils* utils;
};

#endif /* TREE_H */

