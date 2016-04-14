/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Variable.h
 * Author: aleksandrus
 *
 * Created on March 19, 2016, 12:21 AM
 */

#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <vector>

#include "../circular_dep.h"

#include "../Token.h"
#include "../SyntaticException.h"
#include "../Unimplemented.h"
#include "../SymbolTable.h"

#include "Expression.h"

class Variable {
public:
	static const int INT_SIZE = 4;
	static const int CAR_SIZE = 1;
	
	
	/**
	 * Use for non-declaration variable use. Good for 4 cases:\n\n
	 * 1) Assignment of value to an already declared variable.
	 * is_array = false, expr2 not present.\n
	 * Example: x = 3;\n\n
	 * 2) Use of an already declared variable.
	 * is_array false, expr and expr2 not present.\n
	 * Example: x;\n\n
	 * 3) Use of an already declared array variable.
	 * is_array = true, expr2 not present\n
	 * Example: x[10];\n\n
	 * 4) Assignment of a value to an already declared array variable.
	 * is_array = true. expr and expr present.\n
	 * Example: x[10] = 20;\n\n
	 * @param name
	 * @param is_array
	 * @param expr
	 */
	Variable(Token* name, bool is_array = false, Expression* expr = NULL,
	Expression* expr2 = NULL);
	
	
	/**
	 * Use for declarations. Good for 3 cases:\n\n
	 * 1) Declaration of a simple variable.\n
	 * is_array is false, expr is not present.\n
	 * Example: int x;\n\n
	 * 2) Declaration of an array variable.\n
	 * is_array = true, expr is present.\n\n
	 * Example: int x[10];\n\n
	 * 3) Declaration of a non-array variable with a value.\n
	 * is_array is false, expr is present.
	 * Example: int x = 10;\n\n
	 * 
	 * Invalid use:\n\n
	 * is_array is true and expr is not present\n\n
	 * 
	 * @param type
	 * @param name
	 * @param is_array 
	 * @param expr Meaning depends on "is_array".if true, expr means
	 * "array's size". if false, expr means "value of the non-array variable".
	 */
	Variable(Token* type, Token* name, bool is_array = false, Expression* expr = NULL);
	
	void semanticAnalysis(SymbolTable* st);
	
	Variable* getDeclaration(SymbolTable* st);
	
	void printVariable();
	
	virtual ~Variable();
	
	Token *type = NULL, *name = NULL;
	Expression *value = NULL, *arr_size = NULL, *arr_pos = NULL;
	
	bool is_parameter = false;
	bool is_array = false;
private:
};

#endif /* VARIABLE_H */

