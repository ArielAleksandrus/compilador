/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Expression.h
 * Author: aleksandrus
 *
 * Created on March 19, 2016, 11:14 AM
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

#include "../Token.h"
#include "Variable.h"
#include "FuncCall.h"

class Variable;
class FuncCall;

class Expression {
public:
	Expression(Token* literal);
	
	Expression(Variable* variable);
	
	Expression(FuncCall* call);
	
	Expression(Token* unary_op, Expression* lval);
	
	Expression(Expression* lval, Token* dual_op, Expression* rval);
	
	Expression(Expression* lval, Token* ter_op1, Expression* rval1, 
	Token* ter_op2, Expression * rval2);
	
	void printExpression();
	
	virtual ~Expression();
	
	Token *literal = NULL, *unary_op = NULL, *dual_op = NULL, *ter_op1 = NULL,
	*ter_op2 = NULL;
	Variable *variable = NULL;
	Expression *lval = NULL, *rval = NULL, *rval1 = NULL, *rval2 = NULL;
	FuncCall *call = NULL;
private:
};

#endif /* EXPRESSION_H */

