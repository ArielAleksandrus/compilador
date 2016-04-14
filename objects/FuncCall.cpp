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

void FuncCall::semanticAnalysis(SymbolTable* st){
	// checks for arguments and for an existance of a function declaration.
	Function* decl = getFunctionDeclaration(st);
}
/**
 * Gets the function declaration related to the function call.
 * 
 * @param st
 * @return Function declaration related to this function call.
 */
Function* FuncCall::getFunctionDeclaration(SymbolTable* st){
	vector<Function*> res;
	vector<Function*> candidates;
	SymbolTable* aux = st;
	while(aux != NULL){
		for(int i = 0; i < aux->funcs.size(); i++){
			if(this->func_name->lexem == aux->funcs[i]->name->lexem){
				candidates.push_back(aux->funcs[i]);
			}
		}
		aux = aux->parent;
	}
	if(candidates.size() == 0)
		throw new SemanticException(SemanticException::NOT_FOUND, this->func_name,
						NULL, "Function ");
	for(int i = 0; i < candidates.size(); i++){
		if(candidates[i]->arguments.size() != this->args.size()){
			continue;
		}
		bool match = true;
		for(int j = 0; j < candidates[i]->arguments.size(); j++){
			string type_str = candidates[i]->arguments[j]->type->lexem
							+ (candidates[i]->arguments[j]->is_array ? " ARRAY" : "");
			string arg_str = this->args[j]->getExpressionTypeString(st);
			// type lexem is lowercase. Expression type constant is UPPERCASE
			if(strcasecmp(type_str.c_str(), arg_str.c_str()) != 0){
				match = false;
				break;
			}
		}
		if(match)
			res.push_back(candidates[i]);
	}
	
	if(res.size() == 0){
		string msg = "Function declaration of " + this->func_name->lexem + "(";
		for(int i = 0; i < this->args.size(); i++){
			msg += this->args[i]->getExpressionTypeString(st);
			if(i < this->args.size() - 1)
				msg += ", ";
		}
		msg += ") not found";
		throw new SemanticException(this->func_name, msg);
	}
	if(res.size() > 1){
		string msg = "Ambiguous function call(";
		for(int i = 0; i < res.size(); i++){
			msg += res[i]->name->lexem;
			if(i < res.size() - 1)
				msg += ", ";
		}
		msg += ") with parameters (";
		for(int i = 0; i < this->args.size(); i++){
			msg += this->args[i]->getExpressionTypeString(st);
			if(i < this->args.size() - 1)
				msg += ", ";
		}
		msg += ")";
		throw new SemanticException(this->func_name, msg);
	}
	return res[0];
}

FuncCall::~FuncCall() {
}

