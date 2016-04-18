/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SymbolTable.h
 * Author: aleksandrus
 *
 * Created on April 6, 2016, 1:56 AM
 */

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>

#include "circular_dep.h"

#include "objects/Function.h"
#include "objects/Parameter.h"
#include "objects/Variable.h"

class SymbolTable {
public:
	SymbolTable();
	SymbolTable(SymbolTable* parent);
	
	void printSymbolTable();
	
	virtual ~SymbolTable();
	
	std::vector<Function*> funcs;
	std::vector<Parameter*> params;
	std::vector<Variable*> vars;
	SymbolTable* parent;
private:
};

#endif /* SYMBOLTABLE_H */

