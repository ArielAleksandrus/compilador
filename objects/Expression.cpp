/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Expression.cpp
 * Author: aleksandrus
 * 
 * Created on March 19, 2016, 11:14 AM
 */

#include "Expression.h"

using namespace std;

Expression::Expression(Token* literal) {
	this->literal = literal;
}

Expression::Expression(Variable* variable){
	this->variable = variable;
}

Expression::Expression(FuncCall* call){
	this->call = call;
}

Expression::Expression(Token* unary_op, Expression* lval) {
	this->unary_op = unary_op;
	this->lval = lval;
}

Expression::Expression(Expression* lval, Token* dual_op, Expression* rval) {
	this->lval = lval;
	this->dual_op = dual_op;
	this->rval = rval;
}

Expression::Expression(Expression* lval, Token* ter_op1, Expression* rval1,
				Token* ter_op2, Expression* rval2) {
	this->lval = lval;
	this->ter_op1 = ter_op1;
	this->rval1 = rval1;
	this->ter_op2 = ter_op2;
	this->rval2 = rval2;
}

void Expression::printExpression(){
	cout << "\n+++++ EXPRESSION ++++++" << endl << endl;
	if(literal != NULL){
		cout << "+++++ LITERAL ++++++" << endl;
		literal->printToken();
		cout << "----- LITERAL ------" << endl;
	}
	if(variable != NULL){
		variable->printVariable();
	}
	if(call != NULL){
		cout << "+++++ FUNC CALL ++++++" << endl;
		call->printFuncCall();
		cout << "----- FUNC CALL ------" << endl;
	}
	if(unary_op != NULL){
		cout << "+++++ UNARY OPERATOR ++++++" << endl;
		unary_op->printToken();
		cout << "----- UNARY OPERATOR ------" << endl;
	}
	if(lval != NULL){
		cout << "+++++ LVAL ++++++" << endl;
		lval->printExpression();
		cout << "----- LVAL ------" << endl;
	}
	if(dual_op != NULL){
		cout << "+++++ DUAL OPERATOR ++++++" << endl;
		dual_op->printToken();
		cout << "----- DUAL OPERATOR ------" << endl;
	}
	if(rval != NULL){
		cout << "+++++ RVAL ++++++" << endl;
		rval->printExpression();
		cout << "----- RVAL ------" << endl;
	}
	if(ter_op1 != NULL){
		cout << "+++++ TERNARY OPERATOR 1 ++++++" << endl;
		ter_op1->printToken();
		cout << "----- TERNARY OPERATOR 1 ------" << endl;
	}
	if(rval1 != NULL){
		cout << "+++++ RVAL 1 ++++++" << endl;
		rval1->printExpression();
		cout << "----- RVAL 1 ------" << endl;
	}
	if(ter_op2 != NULL){
		cout << "+++++ TERNARY OPERATOR 2 ++++++" << endl;
		ter_op2->printToken();
		cout << "----- TERNARY OPERATOR 2 ------" << endl;
	}
	if(rval2 != NULL){
		cout << "+++++ RVAL 2 ++++++" << endl;
		rval2->printExpression();
		cout << "----- RVAL 2 ------" << endl;
	}
	
	cout << "----- EXPRESSION ------" << endl << endl;
}
Expression::~Expression(){
	
}