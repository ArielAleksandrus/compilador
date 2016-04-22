/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Variable.cpp
 * Author: aleksandrus
 * 
 * Created on March 19, 2016, 12:21 AM
 */

#include "Variable.h"

using namespace std;

Variable::Variable(Token* name, bool is_array /* = false */,
				Expression* expr /* = NULL */, Expression* expr2 /* = NULL */){
	this->name = name;
	if(is_array){
		this->arr_pos = expr;
		if(expr2 != NULL)
			this->value = expr;
	} else
		this->value = expr;
	
	this->is_array = is_array;
}

Variable::Variable(Token* type, Token* name, bool is_array /* = false */,
				Expression* expr /* = NULL */){
	this->type = type;
	this->name = name;
	this->is_array = is_array;
	if(is_array){
		if(expr != NULL)
			this->arr_size = expr;
		
			// is a parameter.
		else
			is_parameter = true;
	} else {
		this->value = expr;
	}
}

Variable::~Variable() {
	
}

void Variable::printVariable(){
	cout << "+++++++ VARIABLE +++++++" << endl;
	if(type != NULL)
		cout << "Type: " + type->lexem << endl;
	cout << "Name: " + name->lexem << endl;
	string s = is_array ? "YES" : "NO";
	cout << "Is array? " << s << endl << endl;
	if(arr_size != NULL){
		cout << "+++++ ARR SIZE +++++" << endl;
		arr_size->printExpression();
		cout << "----- ARR SIZE -----" << endl;
	}
	if(arr_pos != NULL){
		cout << "+++++ ARR POS +++++" << endl;
		arr_pos->printExpression();
		cout << "----- ARR POS -----" << endl;
	}
	if(value != NULL){
		cout << "+++++ VALUE +++++" << endl;
		value->printExpression();
		cout << "----- VALUE -----" << endl;
	}
	cout << "------- VARIABLE -------" << endl;
}

void Variable::semanticAnalysis(SymbolTable* st){
	Variable* decl = NULL;
	// is a variable declaration. check for redeclarations.
	if(this->type != NULL){
		for(int i = 0; i < st->params.size(); i++){
			if(st->params[i]->name->lexem == name->lexem){
				throw new SemanticException(SemanticException::PREVIOUSLY_FOUND,
								name, st->params[i]->name, "Redeclaration of variable");
			}
		}
		for(int i = 0; i < st->vars.size(); i++){
			if(st->vars[i]->name->lexem == name->lexem){
				throw new SemanticException(SemanticException::PREVIOUSLY_FOUND,
								name, st->vars[i]->name, "Redeclaration of variable");
			}
		}
		st->vars.push_back(this);
		
		// is a variable use. check if is declared somewhere
	} else {
		decl = getDeclaration(st);
		
		// check if is an array. if so, check if declaration is also an array.
		if(this->arr_pos != NULL && !decl->is_array)
			throw new SemanticException(this->arr_pos->getAToken(),
							"Shouldn't be an array position, as the variable is not an array");
	}
	
	// check for array size or array position expressions:
	if(this->arr_size != NULL){
		this->arr_size->semanticAnalysis(st);
		int arr_size_expr_type = this->arr_size->getExpressionType(st);
		if(arr_size_expr_type != Expression::INT)
			throw new SemanticException(this->arr_size->getAToken(),
							"Expected an integer value for array size. Got '"
							+ this->arr_size->getExpressionTypeString(st) + "'");
	}
	if(this->arr_pos != NULL){
		this->arr_pos->semanticAnalysis(st);
		
		int arr_pos_expr_type = this->arr_pos->getExpressionType(st);
		if(arr_pos_expr_type != Expression::INT)
			throw new SemanticException(this->arr_pos->getAToken(),
							"Expected an integer value for array position. Got '"
							+ this->arr_pos->getExpressionTypeString(st) + "'");
	}
	
	// check for variable's value:
	if(this->value != NULL){
		this->value->semanticAnalysis(st);
		int value_type = this->value->getExpressionType(st);
		string variable_type;
		bool is_arr;
		if(this->type != NULL){
			variable_type = this->type->lexem;
			is_arr = this->is_array;
		} else {
			variable_type = decl->type->lexem;
			is_arr = decl->is_array;
		}
		
		if(variable_type == "int"){
			if(value_type != Expression::INT && value_type != Expression::CAR){
				throw new SemanticException(this->value->getAToken(),
								"Expected an int or car expression, got: "
								+ this->value->getExpressionTypeString(st));
			}
		}
		if(variable_type == "car"){
			if(is_arr){
				if(this->arr_pos != NULL && value_type != Expression::CAR
								&& value_type != Expression::INT)
					throw new SemanticException(this->value->getAToken(),
									"Expected a car or int expression, got: "
									+ this->value->getExpressionTypeString(st));
				if(this->arr_pos == NULL && value_type != Expression::CAR_ARR)
					throw new SemanticException(this->value->getAToken(),
									"Expected a car array expression, got: "
									+ this->value->getExpressionTypeString(st));
			} else {
				if(this->arr_pos != NULL)
					throw new SemanticException(this->arr_pos->getAToken(),
							"Shouldn't be an array position, as the variable is not an array");
				if(value_type != Expression::CAR && value_type != Expression::INT){
					throw new SemanticException(this->value->getAToken(),
									"Expected a car or int expression, got: "
									+ this->value->getExpressionTypeString(st));
				}
			}
		}
	}
}

Variable* Variable::getDeclaration(SymbolTable* st){
	if(this->type != NULL)
		return this;
	
	Variable* res = NULL;
	while(res == NULL && st != NULL){
		for(int i = 0; i < st->params.size(); i++){
			if(st->params[i]->name->lexem == name->lexem){
				res = new Variable(st->params[i]->type, st->params[i]->name,
								st->params[i]->is_array);
				break;
			}
		}
		if(res == NULL){
			for(int i = 0; i < st->vars.size(); i++){
				if(st->vars[i]->name->lexem == name->lexem
								&& st->vars[i]->type != NULL){
					res = st->vars[i];
					break;
				}
			}
		}
		st = st->parent;
	}
		
	if(res == NULL)
		throw new SemanticException(SemanticException::NOT_FOUND, this->name,
						NULL, "Declaration of variable ");
	
	return res;
}