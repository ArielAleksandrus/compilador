/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tree.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 10:55 PM
 */

#include "Tree.h"

using namespace std;

Tree::Tree() {
}

void Tree::printFunctions(){
	if(this->functions.size() == 0){
		cout << "No functions to show." << endl;
		return;
	}
	cout << "We have " << this->functions.size() << " functions" << endl;
	cout << "\tFunction <number>: <type> <name>(<args>)" << endl;
	for(int i = 0; i < this->functions.size(); i++){
		Function* f = this->functions[i];
		cout << "\tFunction " << i+1 << ": " << f->type->lexem << " "
						<< f->name->lexem << " (";
		for(int j = 0; j < f->arguments.size(); j++){
			Parameter* a = f->arguments[j];
			cout << a->type->lexem << " " << a->name->lexem;
			if(a->is_array)
				cout << "[]";
			if(j != f->arguments.size() - 1)
				cout << ", ";
		}
		cout << ");" << endl;
	}
}

Tree::~Tree() {
}

