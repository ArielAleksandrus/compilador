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

Command::Command(){
	this->type = EMPTY;
}

Command::Command(Token* name){
	this->type = NAME_ONLY;
	this->name = name;
}
Command::Command(Expression* val){
	this->type = EXPR_ONLY;
	this->val = val;
}
Command::Command(Block* body){
	this->type = BLOCK_ONLY;
	this->body = body;
}
Command::Command(Token* name, Expression* val){
	this->type = NAME_EXPR;
	this->name = name;
	this->val = val;
}
Command::Command(Token* name, Variable* lval){
	this->type = NAME_VARIABLE;
	this->name = name;
	this->lval = lval;
}
Command::Command(Token* name, Expression* val, Command* aux1){
	this->type = NAME_EXPR_COMMAND;
	this->name = name;
	this->val = val;
	this->aux1 = aux1;
}
Command::Command(Token* name, Expression* val, Token* aux_name1, Command* aux1){
	this->type = NAME_EXPR_TOKEN_COMMAND;
	this->name = name;
	this->val = val;
	this->aux_name1 = aux_name1;
	this->aux1 = aux1;
}
Command::Command(Token* name, Expression* val, Token* aux_name1, Command* aux1,
	Token* aux_name2, Command* aux2){
	this->type = NAME_EXPR_TOKEN_COMMAND_TOKEN_COMMAND;
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

void Command::semanticAnalysis(SymbolTable* st, Function* parent){
	switch(this->type){
		case EXPR_ONLY: {
			this->val->semanticAnalysis(st);
			break;
		}
		case BLOCK_ONLY: {
			this->body->semanticAnalysis(st, parent);
			break;
		}
		case NAME_ONLY:{
			
			break;
		}
		case NAME_VARIABLE: {
			Variable* v = this->lval->getDeclaration(st);
			if(v->is_array && this->lval->arr_pos == NULL)
				throw new SemanticException(SemanticException::GENERIC, this->lval->name,
								NULL, string("Variable is declared as array,")
								+ string(" but it's use does not inform array position"));
			break;
		}
		case NAME_EXPR: {
			this->val->semanticAnalysis(st);
			string expr_type = this->val->getExpressionTypeString(st);
			if(this->name->lexem == "retorne"){
				transform(expr_type.begin(), expr_type.end(), expr_type.begin(), ::tolower);
				if(expr_type != parent->type->lexem)
					throw new SemanticException(this->val->getAToken(),
									"Function type is '" + parent->type->lexem
									+ "' but told to return '" + expr_type + "'");
			}
			
			break;
		}
		case NAME_EXPR_COMMAND:
		case NAME_EXPR_TOKEN_COMMAND:{
			SymbolTable* local = new SymbolTable(st);
			this->val->semanticAnalysis(local);
			this->aux1->semanticAnalysis(local, parent);
			break;
		}
		case NAME_EXPR_TOKEN_COMMAND_TOKEN_COMMAND:{
			SymbolTable* local = new SymbolTable(st);
			this->val->semanticAnalysis(local);
			this->aux1->semanticAnalysis(local, parent);
			this->aux2->semanticAnalysis(local, parent);
			break;
		}
		case EMPTY: {
			// do nothing
			break;
		}
		default:
			throw new Unimplemented("Invalid command type '" + to_string(this->type)
							+ "' in Command::semanticAnalysis");
	}
}

Command::~Command() {
}

