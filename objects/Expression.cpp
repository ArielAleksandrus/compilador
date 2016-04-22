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
	this->type = LITERAL;
	this->literal = literal;
}

Expression::Expression(Variable* variable){
	this->type = VARIABLE;
	this->variable = variable;
}

Expression::Expression(FuncCall* call){
	this->type = FUNC_CALL;
	this->call = call;
}

Expression::Expression(Token* unary_op, Expression* lval) {
	this->type = UOP_LVAL;
	this->unary_op = unary_op;
	this->lval = lval;
}

Expression::Expression(Expression* lval, Token* dual_op, Expression* rval) {
	this->type = LVAL_DOP_RVAL;
	this->lval = lval;
	this->dual_op = dual_op;
	this->rval = rval;
}

Expression::Expression(Expression* lval, Token* ter_op1, Expression* rval1,
				Token* ter_op2, Expression* rval2) {
	this->type = LVAL_TOP1_RVAL1_TOP2_RVAL2;
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

void Expression::semanticAnalysis(SymbolTable* st){
	switch(this->type){
		case LITERAL:{
			// do nothing
			break;
		}
		case VARIABLE:{
			this->variable->semanticAnalysis(st);
			break;
		}
		case FUNC_CALL:{
			this->call->semanticAnalysis(st);
			break;
		}
		case UOP_LVAL:{
			this->lval->semanticAnalysis(st);
			break;
		}
		case LVAL_DOP_RVAL:{
			this->lval->semanticAnalysis(st);
			this->rval->semanticAnalysis(st);
			
			int lval_type = this->lval->getExpressionType(st);
			int rval_type = this->rval->getExpressionType(st);
			if((lval_type != INT && lval_type != CAR)
							|| (rval_type != INT && rval_type != CAR))
				throw new SemanticException(this->lval->getAToken(),
								"Both lval and rval should be of type int or car. Found: "
								+ this->lval->getExpressionTypeString(st) + " and "
								+ this->rval->getExpressionTypeString(st));
			break;
		}
		case LVAL_TOP1_RVAL1_TOP2_RVAL2:{
			this->lval->semanticAnalysis(st);
			this->rval1->semanticAnalysis(st);
			this->rval2->semanticAnalysis(st);
			
			int lval_type = this->lval->getExpressionType(st);
			if(lval_type != INT && lval_type != CAR)
				throw new SemanticException(this->lval->getAToken(),
								string("Ternary expression's lval should be composed of") +
								string(" an int or car, found: '")
								+ this->lval->getExpressionTypeString(st) + "'");
			int rval1_type = this->rval1->getExpressionType(st);
			int rval2_type = this->rval2->getExpressionType(st);
			if(rval1_type != rval2_type)
				throw new SemanticException(this->rval1->getAToken(),
								string("Ternary's expression rval1 and rval2 should be of the ")
								+ "same type, but found " + this->rval1->getExpressionTypeString(st)
								+ " and " + this->rval2->getExpressionTypeString(st));
			
			break;
		}
		default:
			throw new Unimplemented("Invalid Expression type '" + to_string(this->type)
							+ "' for Expression::semanticAnalysis");
	}
}

int Expression::getExpressionType(SymbolTable* st){
	switch(this->type){
		case LITERAL: {
			// is string literal
			if(this->literal->lexem.at(0) == '"')
				return CAR_ARR;
			// is char literal
			else if(this->literal->lexem.at(0) == '\'')
				return CAR;
			else
				return INT;
		}
		case VARIABLE: {
			Variable* v = this->variable->getDeclaration(st);

			if(v->type->lexem == "int"){
				if(this->variable->arr_size != NULL
								|| (v->is_array && this->variable->arr_pos == NULL))
					return INT_ARR;
				else 
					return INT;
			} else if(v->type->lexem == "car"){
				if(this->variable->arr_size != NULL
								|| (v->is_array && this->variable->arr_pos == NULL))
					return CAR_ARR;
				else 
					return CAR;
			}
		}
		case FUNC_CALL:{
			Function* f = this->call->getFunctionDeclaration(st);
			if(f->type->lexem == "int")
				return INT;
			else if(f->type->lexem == "car")
				return CAR;
			else
				throw new Unimplemented("Should implement new type '" + f->type->lexem
								+ "' for Expression::getExpressionType");
		}
		case UOP_LVAL:{
			return this->lval->getExpressionType(st);
			break;
		}
		case LVAL_DOP_RVAL:{
			return this->lval->getExpressionType(st);
			break;
		}
		case LVAL_TOP1_RVAL1_TOP2_RVAL2:{
			return this->rval1->getExpressionType(st);
			break;
		}
	}
}

string Expression::getExpressionTypeString(SymbolTable* st) {
	switch(getExpressionType(st)){
		case INT:
			return "INT";
		case INT_ARR:
			return "INT ARRAY";
		case CAR:
			return "CAR";
		case CAR_ARR:
			return "CAR ARRAY";
		default:
			throw new Unimplemented("Invalid expression type '"
							+ to_string(getExpressionType(st))
							+ "' for Expression::getExpressionTypeString");
	}
}

Token* Expression::getAToken(){
	if(literal != NULL)
		return literal;
	if(variable != NULL)
		return variable->name;
	if(call != NULL)
		return call->func_name;
	if(lval != NULL)
		return lval->getAToken();
	throw new Unimplemented("Should implement Expression::getAToken for something other than literal, variable, func call or lval");
}


Expression::~Expression(){
	
}