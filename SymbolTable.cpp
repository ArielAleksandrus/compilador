/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SymbolTable.cpp
 * Author: aleksandrus
 * 
 * Created on April 6, 2016, 1:56 AM
 */

#include "SymbolTable.h"

using namespace std;

SymbolTable::SymbolTable(){
	this->parent = NULL;
}
SymbolTable::SymbolTable(SymbolTable* parent) {
	this->parent = parent;
}

SymbolTable::~SymbolTable() {
}

