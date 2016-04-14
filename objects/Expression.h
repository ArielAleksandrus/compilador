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

#include "../circular_dep.h"

#include "../Token.h"
#include "../SymbolTable.h"
#include "../Unimplemented.h"

#include "Variable.h"
#include "FuncCall.h"

class Expression {
public:
	static const int LITERAL = 0;
	static const int VARIABLE = 1;
	static const int FUNC_CALL = 2;
	static const int UOP_LVAL = 3;
	static const int LVAL_DOP_RVAL = 4;
	static const int LVAL_TOP1_RVAL1_TOP2_RVAL2 = 5;
	
	// EXPRESSION TYPES
	static const int INT = 0;
	static const int CAR = 1;
	static const int INT_ARR = 2;
	static const int CAR_ARR = 3;
	
	
	Expression(Token* literal);
	
	Expression(Variable* variable);
	
	Expression(FuncCall* call);
	
	Expression(Token* unary_op, Expression* lval);
	
	Expression(Expression* lval, Token* dual_op, Expression* rval);
	
	Expression(Expression* lval, Token* ter_op1, Expression* rval1, 
	Token* ter_op2, Expression * rval2);
	
	void printExpression();
	
	void semanticAnalysis(SymbolTable* st);
	
	/**
	 * Gets the type of expression by analyzing it's components.
	 * 
	 * @param st symbol table to check for variable use.
	 * @return expression type constant declared above.
	 */
	int getExpressionType(SymbolTable* st);
	
	/**
	 * 
	 * @param st symbol table to be passed to getExpressionType.
	 * @return 
	 */
	std::string getExpressionTypeString(SymbolTable* st);
	
	Token* getAToken();
	
	virtual ~Expression();
	
	Token *literal = NULL, *unary_op = NULL, *dual_op = NULL, *ter_op1 = NULL,
	*ter_op2 = NULL;
	Variable *variable = NULL;
	Expression *lval = NULL, *rval = NULL, *rval1 = NULL, *rval2 = NULL;
	FuncCall *call = NULL;
	int type;
private:
};

#endif /* EXPRESSION_H */

