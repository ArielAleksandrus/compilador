/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Command.cpp
 * Author: aleksandrus
 * 
 * Created on March 19, 2016, 2:32 AM
 */

#include "Command.h"

using namespace std;

Command::Command(Token* name){
	this->name = name;
}
Command::Command(Expression* val){
	this->val = val;
}
Command::Command(Block* body){
	this->body = body;
}
Command::Command(Token* name, Expression* val){
	this->name = name;
	this->val = val;
}
Command::Command(Token* name, Variable* lval){
	this->name = name;
	this->lval = lval;
}
Command::Command(Token* name, Expression* val, Command* aux1){
	this->name = name;
	this->val = val;
	this->aux1 = aux1;
}
Command::Command(Token* name, Expression* val, Token* aux_name1, Command* aux1){
	this->name = name;
	this->val = val;
	this->aux_name1 = aux_name1;
	this->aux1 = aux1;
}
Command::Command(Token* name, Expression* val, Token* aux_name1, Command* aux1,
	Token* aux_name2, Command* aux2){
	this->name = name;
	this->val = val;
	this->aux_name1 = aux_name1;
	this->aux1 = aux1;
	this->aux_name2 = aux_name2;
	this->aux2 = aux2;
}

void Command::printCommand(){
	cout << "++++++ COMMAND ++++++\n\n";
	if(name != NULL){
		cout << "++++++ COMMAND NAME ++++++\n";
		name->printToken();
		cout << "------ COMMAND NAME ------\n";
	}
	if(val != NULL){
		cout << "++++++ COMMAND EXPR ++++++\n";
		val->printExpression();
		cout << "------ COMMAND EXPR ------\n";
	}
	if(lval != NULL){
		cout << "++++++ COMMAND VARIABLE ++++++\n";
		lval->printVariable();
		cout << "------ COMMAND VARIABLE ------\n";
	}
	if(body != NULL){
		cout << "++++++ COMMAND BLOCK ++++++\n";
		body->printBlock();
		cout << "------ COMMAND BLOCK ------\n";
	}
	if(aux_name1 != NULL){
		cout << "++++++ COMMAND AUX_NAME1 ++++++\n";
		aux_name1->printToken();
		cout << "------ COMMAND AUX_NAME1 ------\n";
	}
	if(aux1 != NULL){
		cout << "++++++ COMMAND AUX1 ++++++\n";
		aux1->printCommand();
		cout << "------ COMMAND AUX1 ------\n";
	}
	if(aux_name2 != NULL){
		cout << "++++++ COMMAND AUX_NAME2 ++++++\n";
		aux_name2->printToken();
		cout << "------ COMMAND AUX_NAME2 ------\n";
	}
	if(aux2 != NULL){
		cout << "++++++ COMMAND AUX2 ++++++\n";
		aux2->printCommand();
		cout << "------ COMMAND AUX2 ------\n";
	}
	cout << "------ COMMAND ------\n\n";
}

Command::~Command() {
}

