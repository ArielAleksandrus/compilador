/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FuncCall.cpp
 * Author: aleksandrus
 * 
 * Created on March 22, 2016, 1:50 PM
 */

#include "FuncCall.h"

using namespace std;

FuncCall::FuncCall(Token* func_name){
	this->func_name = func_name;
}
FuncCall::FuncCall(Token* func_name, vector<Expression*> args) {
	this->func_name = func_name;
	this->args = args;
}

void FuncCall::printFuncCall(){
	cout << "Function Call:" << endl;
	cout << "\tFunction name: " << func_name->lexem << endl;
	cout << "+++++++ ARGS +++++++++" << endl << endl;
	for(int i = 0; i < args.size(); i++){
		cout << "+++++ ARG +++++" << endl;
		args[i]->printExpression();
		cout << "----- ARG -----" << endl;
	}
	cout << "------- ARGS ---------" << endl << endl;
}

FuncCall::~FuncCall() {
}

