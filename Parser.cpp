/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Parser.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 8:14 PM
 */

#include "Parser.h"

using namespace std;

Parser::Parser(vector<Token*> tokens, Tree* t) {
	this->tokens = tokens;
	int pos = 0;
	
	while(tokens[pos]->type == Token::TYPE){
		Function* f;
		vector<Variable*> globals;
		// is a function
		if(f = checkFunction(&pos), f != NULL)
			t->functions.push_back(f);
		// is a global variable
		else if(globals = getVariableDeclarations(&pos, this->tokens, true),
						globals.size() > 0){
			for(int i = 0; i < globals.size(); i++)
				t->globals.push_back(globals[i]);
			
			// shouldn't fall here
		} else 
			throw new SyntaticException(this->tokens[pos],
							"Expected global variable, function or 'programa' declarations");
	}
	
	// get block programa
	if(tokens[pos]->lexem == "programa"){
		pos++;
		t->programa = getBlock(&pos, tokens);
	} else {
		throw new SyntaticException(tokens[pos],
						"Expected function, variable declaration or 'programa' block, got: '"
						+ tokens[pos]->lexem + "'");
	}
}

Function* Parser::checkFunction(int* position){
	Token *func_type, *func_name;
	vector<Parameter*> args;
	int pos = *position;
	// should be a function
	if(this->tokens[pos]->type == Token::TYPE){
		func_type = this->tokens[pos];
		// it is a function declaration.
		if(func_name = this->tokens[++pos], this->tokens[pos]->type == Token::NAME
						&& this->tokens[++pos]->lexem == "("){
			// Now we will check for the argument's list.
			Token *arg_type, *arg_name;
			bool is_array;
			// Get function's arguments
			while(true){
				is_array = false;
				if(this->tokens[++pos]->lexem == ")" )
					break;
				if(this->tokens[pos]->lexem == ",")
					++pos;
				
				arg_type = this->tokens[pos];
				if(this->tokens[pos]->type != Token::TYPE)
					throw new SyntaticException(this->tokens[pos],
									"Should be Type. Found: " + this->tokens[pos]->type_string()
									+ " '" + this->tokens[pos]->lexem + "'");
				
				arg_name = this->tokens[++pos];
				if(this->tokens[pos]->type != Token::NAME){
					throw new SyntaticException(this->tokens[pos],
									"Should be Name. Found: " + this->tokens[pos]->type_string()
									+ " '" + this->tokens[pos]->lexem + "'");
				}
				if(this->tokens[++pos]->lexem == "["){
					if(pos + 1 >= this->tokens.size())
						throw new SyntaticException(this->tokens[pos],
										"Symbol ']' not found");
					if(this->tokens[++pos]->lexem == "]"){
						is_array = true;
					} else {
						throw new SyntaticException(this->tokens[pos],
										"Expected ']', got: '" + this->tokens[pos]->lexem + "'");
					}
				} else {
					pos--;
				}
				args.push_back(new Parameter(arg_type, arg_name, is_array));
				
			}
			
			// it should be a global variable. so we can return null.
		} else {
			return NULL;
		}
	}
	++pos;
	*position = pos;
	return new Function(func_type, func_name, getBlock(position, this->tokens), args);
}

Block* Parser::getBlock(int* position, vector<Token*> tokens){
	int pos = *position;
	if(tokens[pos]->lexem != "{"){
		throw new SyntaticException(tokens[pos],
						"Should be '{'. Found: '" + tokens[pos]->lexem + "'");
	}
	
	vector<Token*> block_tokens;
	int open_cb = 1;
	while(tokens[++pos]->lexem != "}" || open_cb != 1){
		if(tokens[pos]->lexem == "{")
			open_cb++;
		else if(tokens[pos]->lexem == "}")
			open_cb--;
		
		block_tokens.push_back(tokens[pos]);
		
		if(pos >= tokens.size() - 1)
			throw new SyntaticException(tokens[pos],
							"Unexpected end of block. Should use '}'");
	}
	*position = pos + 1;
	
	int aux = 0;
	vector<Variable*> variables = getVariableDeclarations(&aux, block_tokens);
	vector<Command*> commands = getCommands(&aux, block_tokens);
	
	if(aux != block_tokens.size())
		throw new SyntaticException(block_tokens[aux],string("Unexpected ")
						+ block_tokens[aux]->type_string() + string(" '")
						+ block_tokens[aux]->lexem + string(" '"));
	
	return new Block(variables, commands);
}

vector<Variable*> Parser::getVariableDeclarations(int* position, 
				vector<Token*> tokens, bool is_global /* = false */) {
	
	vector<Variable*> variables;
	if(tokens.size() == 0)
		return variables;
	
	int pos = *position;
	int last_valid = -1;
	bool found_semicolem;
	// full declaration: type name ...
	while(true){
		if(pos >= tokens.size())
			break;
		// partial declaration: name, name ...
		Token* type = tokens[pos];
		if(type->type != Token::TYPE){
			break;
		}
		found_semicolem = false;
		while(!found_semicolem){
			vector<Token*> var_tokens;
			string lex = tokens[++pos]->lexem;
			var_tokens.push_back(type);
			while(lex != "," && lex != ";"){
				var_tokens.push_back(tokens[pos]);
				if(pos + 1 >= tokens.size())
					throw new SyntaticException(tokens[pos],
									"Unexpected end of variable declaration");
				lex = tokens[++pos]->lexem;
			}
			int pos2 = 0;
			Variable* var = getVariable(var_tokens, &pos2, is_global);
			
			if(var == NULL){
				if(is_global){
					if(variables.size() > 0)
						*position = last_valid;
					return variables;
				} else
					throw new SyntaticException(var_tokens[var_tokens.size() - 1],
									"Variable cannot be null");
			}
			if(pos2 != var_tokens.size())
				throw new SyntaticException(var_tokens[var_tokens.size() - 1],
								"Invalid variable declaration");
			
			var_tokens.clear();
			
			variables.push_back(var);
			
			
			if(lex == ";"){
				pos++;
				found_semicolem = true;
			}
			last_valid = pos;
		}
	}
	
	*position = pos;
	
	return variables;
}


Variable* Parser::getVariable(vector<Token*> tokens, int* pos,
				bool is_global /* = false */){
	vector<Token*> arr_pos_tokens, arr_size_tokens, value_tokens;
	int jmp = *pos;
	Token *name, *type;
	Expression *arr_size = NULL, *arr_pos = NULL, *value = NULL;
	bool is_array = false;
	bool is_assignment = false;
	
	// for 4 cases:
	// 1- use of variable: a
	// 2- use of array variable: a[0]
	// 3- assignment of a variable: a = 3
	// 4- assignment of an array variable: a[0] = 3
	if(tokens[jmp]->type == Token::NAME){
		name = tokens[jmp];
		if(jmp + 1 < tokens.size()){
			// is array.
			if(tokens[++jmp]->lexem == "["){
				is_array = true;
				while(tokens[++jmp]->lexem != "]"){
					arr_pos_tokens.push_back(tokens[jmp]);
					if(jmp == tokens.size() - 1)
						throw new SyntaticException(tokens[jmp], "Expected ']'. Found: '"
										+ tokens[jmp]->lexem + "'");
				}
				arr_pos_tokens.clear();
				
				// x[10]
				if(jmp == tokens.size() - 1){
					*pos += jmp + 1;
					return new Variable(name, true, arr_pos);
					// may be an assignment too. x[10] = 30
				} else {
					jmp++;
				}
				// is a function call.
			} else if(tokens[jmp]->lexem == "("){
				return NULL;
			}
			// is assignment
			if(tokens[jmp]->lexem == "="){
				is_assignment = true;
				while(++jmp < tokens.size() && tokens[jmp]->lexem != ";")
					value_tokens.push_back(tokens[jmp]);
				
				if(value_tokens.size() == 0)
					throw new SyntaticException(tokens[jmp], "Expression not found");
				value = resolve(value_tokens);
				if(value == NULL)
					throw new SyntaticException(tokens[jmp], "Invalid variable value");
				value_tokens.clear();
				jmp++;
			}
			
			*pos = jmp;
			return new Variable(name, is_array, value);
		} else {
			*pos += 1;
			return new Variable(tokens[jmp]);
		}
		
		// is declaration
	} else if(tokens[jmp]->type == Token::TYPE){
		type = tokens[jmp];
		int i_type = jmp;
		if(jmp + 1 < tokens.size()){
			if(tokens[++jmp]->type != Token::NAME)
				throw new SyntaticException(tokens[jmp], "Expected name of variable, got: "
								+ tokens[jmp]->type_string());
			name = tokens[jmp];
			
			// can be array and/or assignment
			if(jmp + 2 < tokens.size()){
				// is array.
				if(tokens[++jmp]->lexem == "["){
					while(tokens[++jmp]->lexem != "]"){
						arr_size_tokens.push_back(tokens[jmp]);
						if(jmp >= tokens.size()){
							throw new SyntaticException(tokens[jmp-1],
											"Unexpected end of expression");
						}
					}
					arr_size = resolve(arr_size_tokens);
					if(arr_size == NULL)
						throw new SyntaticException(tokens[jmp-1], "Invalid array size expression");
					arr_size_tokens.clear();
					
					*pos += jmp + 1;
					return new Variable(type, name, true, arr_size);
					// is assignment.
				}
				if(tokens[jmp]->lexem == "="){
					while(++jmp < tokens.size() && tokens[jmp]->lexem != ";"
									&& tokens[jmp]->lexem != ","){
						value_tokens.push_back(tokens[jmp]);
					}
					
					value = resolve(value_tokens);
					if(value == NULL)
						throw new SyntaticException(tokens[jmp], "Invalid variable assignment expression");
					
					*pos = jmp;
					return new Variable(type, name, false, value);
					
					// is function call, but not a function declaration. throw error.
				} else if(tokens[jmp]->lexem == "("){
					if(is_global){
						*pos = i_type;
						return NULL;
					} else
						throw new SyntaticException(tokens[jmp], "Invalid function call");
				} else {
					*pos += jmp + 1;
					return new Variable(type, name);
				}
			} else {
				*pos += jmp + 1;
				return new Variable(type, name);
			}
		} else {
			throw new SyntaticException(tokens[0], "Unexpected end of declaration");
		}
	} else {
		return NULL;
	}
}
FuncCall* Parser::getFuncCall(vector<Token*> tokens, int* pos){
	int i = *pos;
	vector<Token*> args_tokens;
	vector<Expression*> args;
	Token* name;
	
	if(i + 2 >= tokens.size() || tokens.size() < 3
					|| tokens[i]->type != Token::NAME || tokens[i+1]->lexem != "(")
		return NULL;
	
	name = tokens[i];
	
	i += 2;
	while(tokens[i]->lexem != ")"){
		if(tokens[i]->lexem == ","){
			if(args_tokens.size() == 0)
				throw new SyntaticException(tokens[i], "Unexpected ','. Expected argument");
			Expression* arg = resolve(args_tokens);
			if(arg == NULL)
				throw new SyntaticException(tokens[i-1], "Invalid argument");
			args.push_back(arg);
			args_tokens.clear();
		} else {
			args_tokens.push_back(tokens[i]);
		}
		
		if(i == tokens.size() - 1)
			throw new SyntaticException(tokens[1], "Expected ')' after arguments list");
		
		i++;
	}
	// last argument
	if(args_tokens.size() > 0){
		Expression* arg = resolve(args_tokens);
		if(arg == NULL)
			throw new SyntaticException(tokens[i-1], "Invalid argument");
		
		args.push_back(arg);
	}
	*pos = i + 1;
	return new FuncCall(name, args);
}
Expression* Parser::resolve(vector<Token*> tokens){
	vector<ExprOrOpToken*> eot;
	vector<Token*> in_parenthesis;
	Variable* v;
	FuncCall* fc;
	
	int i = 0;
	while(i < tokens.size()){
		ExprOrOpToken* node = (ExprOrOpToken*) malloc(sizeof(ExprOrOpToken));
		node->expr = NULL;
		node->op = NULL;
		
		// is '(' and is not a function call
		if(tokens[i]->lexem=="(" && (i == 0||tokens[i - 1]->type != Token::NAME)){
			int open_par = 1;
			while(tokens[++i]->lexem != ")" || open_par != 1){
				if(i >= tokens.size() - 1)
					throw new SyntaticException(tokens[i],
									"Expected ')' to end expression");
				
				if(tokens[i]->lexem == "(")
					open_par++;
				else if(tokens[i]->lexem == ")")
					open_par--;
				
				in_parenthesis.push_back(tokens[i]);
			}
			i++;
			if(in_parenthesis.size() == 0)
				throw new SyntaticException(tokens[i], "Expression cannot be empty");
			node->expr = resolve(in_parenthesis);
			in_parenthesis.clear();
		} else if(tokens[i]->type == Token::OPERATOR ||
						tokens[i]->type == Token::OPERATOR2){
			node->op = tokens[i];
			i++;
		} else if(fc = getFuncCall(tokens, &i)){
			node->expr = new Expression(fc);
			fc = NULL;
		} else if(tokens[i]->type == Token::LITERAL){
			node->expr = new Expression(tokens[i]);
			i++;
		} else if(v = getVariable(tokens, &i)){
			node->expr = new Expression(v);
			v = NULL;
		} else {
			throw new SyntaticException(tokens[i],
							string("Expected expression or operator, got ")
							+ tokens[i]->type_string() + string(" '") + tokens[i]->lexem
							+ string("'"));
		}
		eot.push_back(node);
	}
	
	Expression* res = resolve(eot);
	for(int i = 0; i < eot.size(); i++)
		free(eot[i]);
	return res;
}
Expression* Parser::resolve(vector<ExprOrOpToken*> eot){
	if(eot.size() == 0)
		return NULL;
	vector<ExprOrOpToken*> new_eot, in_parenthesis;
	vector<ExprOrOpToken*> lval_eot, rval_eot;
	vector<ExprOrOpToken*> rval1_eot, rval2_eot;
	int open_parenthesis_index;
	
	if(eot.size() == 1){
		if(eot[0]->expr != NULL)
			return eot[0]->expr;
		else
			throw new SyntaticException(eot[0]->op, string("Invalid expression '")
							+ string(eot[0]->op->lexem) + string("'"));
	}
	
	////////////////////////// resolve parenthesis ///////////////////////////////
	for(int i = 0; i < eot.size(); i++){
		if(eot[i]->op != NULL && eot[i]->op->lexem == "("){
			open_parenthesis_index = i;
			while(eot[++i]->op == NULL || eot[i]->op->lexem != ")"){
				if(i == eot.size())
					throw new SyntaticException(eot[open_parenthesis_index]->op,
									"')' not found.");
				in_parenthesis.push_back(eot[i]);
			}
			ExprOrOpToken* new_eoot = (ExprOrOpToken*) malloc(sizeof(ExprOrOpToken));
			new_eoot->op = NULL;
			new_eoot->expr = resolve(in_parenthesis);
			new_eot.push_back(new_eoot);
		} else {
			new_eot.push_back(eot[i]);
		}
	}
	eot.clear();
	eot = new_eot;
	new_eot.clear();
	//////////////////////////////////////////////////////////////////////////////
	
	////////////////////////// resolve unary ops /////////////////////////////////
	vector<Token*> un_ops;
	for(int i = 0; i < eot.size(); i++){
		// is unary operator.
		if(eot[i]->op != NULL && eot[i]->op->is_op_type(Token::UNARY_OPERATOR)
						&& (i == 0 || eot[i - 1]->expr == NULL)){
			un_ops.push_back(eot[i]->op);
			if(i + 1 < eot.size()){
				if(eot[i + 1]->expr != NULL){
					Expression* e = new Expression(un_ops[un_ops.size()-1], eot[i+1]->expr);
					for(int j = un_ops.size() - 2; j >= 0; j--)
						e = new Expression(un_ops[j], e);
					
					ExprOrOpToken* new_eoot = (ExprOrOpToken*) malloc(sizeof(ExprOrOpToken));
					new_eoot->op = NULL;
					new_eoot->expr = e;
					new_eot.push_back(new_eoot);
					un_ops.clear();
					i++;
				}
			} else {
				throw new SyntaticException(eot[i]->op,
								"Unexpected unary operator. Expected expression");
			}
		} else {
			// if un_ops has elements, eot[i] can't be expr, as the above 'if' would
			// have found expr and cleared un_ops.
			if(un_ops.size() > 0)
				throw new SyntaticException(eot[i]->op,
								"Expected unary operator or expression. Found: " + eot[i]->op->lexem);
			else
				new_eot.push_back(eot[i]);
		}
	}
	eot.clear();
	eot = new_eot;
	new_eot.clear();
	//////////////////////////////////////////////////////////////////////////////
	
	//////////////////////// resolve ternary ops /////////////////////////////////
	Token *t_op1, *t_op2;
	for(int i = 0; i < eot.size(); i++){
		if(eot[i]->op != NULL && eot[i]->op->lexem == "?"){
			t_op1 = eot[i]->op;
			for(int j = 0; j < i; j++)
				lval_eot.push_back(eot[j]);
			
			Expression* lval = resolve(lval_eot);
			
			for(int j = i + 1; j < eot.size(); j++){
				if(eot[j]->op != NULL && eot[j]->op->lexem == ":"){
					t_op2 = eot[j]->op;
					for(int k = i + 1; k < j; k++)
						rval1_eot.push_back(eot[k]);
					
					for(int k = j + 1; k < eot.size(); k++)
						rval2_eot.push_back(eot[k]);
					
					Expression* rval1 = resolve(rval1_eot);
					Expression* rval2 = resolve(rval2_eot);
					if(lval == NULL || rval1 == NULL || rval2 == NULL)
						throw new SyntaticException(eot[i]->op, "Not a valid expression");
					return new Expression(lval, t_op1, rval1, t_op2, rval2);
				}
			}
			throw new SyntaticException(eot[i]->op, "Second ternary op not found");
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	
	/////////////////// resolve high priority dual ops ///////////////////////////
	//resolve comparison ops: >=, <=, >, <, !=, ==
	vector<string> ops = {">=", "<=", ">", "<", "!=", "=="};
	for(int i = 0; i < eot.size(); i++){
		if(eot[i]->op != NULL && find(ops.begin(), ops.end(), eot[i]->op->lexem)
						!= ops.end()){
			for(int j = 0; j < i; j++)
				lval_eot.push_back(eot[j]);
			
			for(int j = i + 1; j < eot.size(); j++)
				rval_eot.push_back(eot[j]);
			
			Expression* lval = resolve(lval_eot);
			Expression* rval = resolve(rval_eot);
			return new Expression(lval, eot[i]->op, rval);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	
	//////////////////// resolve low priority dual ops ///////////////////////////
	// resolve comparison ops: e, ou, &&, ||
	ops = {"e", "ou", "&&", "||"};
	for(int i = 0; i < eot.size(); i++){
		if(eot[i]->op != NULL && find(ops.begin(), ops.end(), eot[i]->op->lexem)
						!= ops.end()){
			for(int j = 0; j < i; j++)
				lval_eot.push_back(eot[j]);
			
			for(int j = i + 1; j < eot.size(); j++)
				rval_eot.push_back(eot[j]);
			
			Expression* lval = resolve(lval_eot);
			Expression* rval = resolve(rval_eot);
			return new Expression(lval, eot[i]->op, rval);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	
	//////////////////// resolve low priority dual ops ///////////////////////////
	// resolve comparison ops: *, /, %
	ops = {"*", "/", "%"};
	for(int i = 0; i < eot.size(); i++){
		if(eot[i]->op != NULL && find(ops.begin(), ops.end(), eot[i]->op->lexem)
						!= ops.end()){
			if(i + 1 >= eot.size())
				throw new SyntaticException(eot[i]->op, "RVal not found");
			
			Expression *lval = eot[i - 1]->expr, *rval = eot[i + 1]->expr;
			if(lval == NULL || rval == NULL)
				throw new SyntaticException(eot[i]->op, "Not a valid expression");
			ExprOrOpToken* eoot = (ExprOrOpToken*) malloc(sizeof(ExprOrOpToken));
			eoot->op = NULL;
			eoot->expr = new Expression(lval, eot[i]->op, rval);
			// remove at pos i-1, i and i + 1
			eot.erase(eot.begin() + i - 1, eot.begin() + i + 2);
			eot.insert(eot.begin() + i - 1, eoot);
			i--;
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	
	//////////////////// resolve low priority dual ops ///////////////////////////
	// resolve comparison ops: +, -
	ops = {"+", "-"};
	for(int i = 0; i < eot.size(); i++){
		if(eot[i]->op != NULL && find(ops.begin(), ops.end(), eot[i]->op->lexem)
						!= ops.end()){
			for(int j = 0; j < i; j++)
				lval_eot.push_back(eot[j]);
			
			for(int j = i + 1; j < eot.size(); j++)
				rval_eot.push_back(eot[j]);
			
			Expression* lval = resolve(lval_eot);
			Expression* rval = resolve(rval_eot);
			if(lval == NULL || rval == NULL)
				throw new SyntaticException(eot[i]->op, "Not a valid expression");
			return new Expression(lval, eot[i]->op, rval);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	
	if(eot.size() == 1 && eot[0]->expr != NULL)
		return eot[0]->expr;
	
	// shouldn't reach this place
	if(eot[0]->expr != NULL)
		throw new SyntaticException(eot[0]->expr, "Couldn't mount expression");
	else
		throw new SyntaticException(eot[0]->op, "Couldn't mount expression");
}

vector<Command*> Parser::getCommands(int* position, vector<Token*> tokens,
				int max /* = -1 */){
	int i = *position;
	vector<Command*> commands;
	vector<Token*> cmd_tokens, expr_tokens;
	
	while(i < tokens.size() && (max == -1 || commands.size() < max)){
		Token* cur = tokens[i];
		// can be assignment, function call, useless expression (like 3 + 5;), etc.
		if(cur->type == Token::NAME){
			if(i == tokens.size() - 1)
				throw new SyntaticException(cur, "Unexpected end of expression");
			cmd_tokens.push_back(cur);
			while(cur = tokens[++i], cur->lexem != ";"){
				cmd_tokens.push_back(cur);
				if(i == tokens.size() - 1)
					throw new SyntaticException(cur, "Unexpected end of expression");
			}
			Expression* expr = resolve(cmd_tokens);
			if(expr == NULL)
				throw new SyntaticException(tokens[i-1], "Invalid command expression");
			commands.push_back(new Command(expr));
			cmd_tokens.clear();
			// is a command, like retorne 1, se(a > 3) entao a = a - 3, etc.
		} else if(cur->type == Token::COMMAND){
			Token* name_token = cur;
			string name = name_token->lexem;
			
			// we guarantee that there will be at least 1 more token.
			if(i >= tokens.size() - 1)
				throw new SyntaticException(cur, "Unexpected end of command");
			
			// this command take no arguments.
			if(name == "novalinha"){
				if(cur = tokens[++i], cur->lexem != ";")
					throw new SyntaticException(cur,
									string("Expected ';' as this command takes no arguments.")
									+ string(" Found '" + cur->lexem + string("'")));
				
				commands.push_back(new Command(name_token));
				
				// this command takes an expression.
			} else if(name == "retorne" || name == "escreva"){
				while(tokens[++i]->lexem != ";"){
					expr_tokens.push_back(tokens[i]);
					if(i >= tokens.size() -1)
						throw new SyntaticException(tokens[i],
										"Unexpected end of expression");
				}
				Expression* expr = resolve(expr_tokens);
				expr_tokens.clear();
				if(expr == NULL)
					throw new SyntaticException(tokens[i], "Invalid expression");
				commands.push_back(new Command(name_token, expr));
				
				// this command takes a variable;
			} else if(name == "leia"){
				Variable* v = getVariable(tokens, &i);
				if(v == NULL || v->value != NULL || v->type != NULL)
					throw new SyntaticException(tokens[i-1],
									"Not a valid variable use");
				if(i >= tokens.size() - 1 || tokens[i+1]->lexem != ";")
					throw new SyntaticException(tokens[i], "Missing ';'");
				commands.push_back(new Command(name_token, v));
				
				// these commands take an expr to be evaluated and another expr, block
				// or command.
			} else if(name == "enquanto" || name == "se") {
				Expression *bool_evaluation, *eaux1 = NULL, *eaux2 = NULL;
				Block *baux1 = NULL, *baux2 = NULL;
				Token *aux_name1 = NULL, *aux_name2 = NULL;
				Command *c1, *c2;
				
				if(tokens[++i]->lexem != "(")
					throw new SyntaticException(tokens[i],
									"Invalid token. Expected '('");
				
				int open_par = 1;
				while(tokens[++i]->lexem != ")" || open_par != 1){
					if(i >= tokens.size() - 1)
						throw new SyntaticException(tokens[i],
										string("Expected ')'. Found '") + tokens[i]->lexem
										+ "'");
					if(tokens[i]->lexem == "(")
						open_par++;
					else if(tokens[i]->lexem == ")")
						open_par--;
					
					expr_tokens.push_back(tokens[i]);
				}
			
				bool_evaluation = resolve(expr_tokens);
				if(bool_evaluation == NULL)
					throw new SyntaticException(name_token,
									string("Invalid expression after '") + name + string("'"));
				expr_tokens.clear();
				
				if(name == "se"){
					if(tokens[++i]->lexem != "entao")
						throw new SyntaticException(tokens[i],
										"Expected keyword 'entao'");
					else
						aux_name1 = tokens[i];
				}
				
				// is block.
				if(tokens[++i]->lexem == "{") {
					baux1 = getBlock(&i, tokens);
					i--; // let's point to '}'
					c1 = new Command(baux1);
					
					// is an expression.
 				} else if(tokens[i]->type == Token::NAME) {
					i--;
					while(tokens[++i]->lexem != ";"){
						expr_tokens.push_back(tokens[i]);
						if(i + 1 >= tokens.size())
							throw new SyntaticException(tokens[i],
											string("Expected ';', got: '") + tokens[i]->lexem
											+ string("'"));
					}
					eaux1 = resolve(expr_tokens);
					c1 = new Command(eaux1);
					expr_tokens.clear();
					
					// is a command
				} else if(tokens[i]->type == Token::COMMAND){
					vector<Command*> following = getCommands(&i, tokens, 1);
					c1 = following[0];
					i--;
				} else if(tokens[i]->lexem == ";") {
					c1 = new Command();
				} else {
					throw new SyntaticException(tokens[i],
									"not a Block, Command or Expression");
				}
				
				if(c1 == NULL)
					throw new SyntaticException(tokens[i-1],
									"Invalid block, command or expression to be executed");
				
				// for "command(expr) command" cases.
				if(name == "enquanto" ||
								(name == "se" && i + 2 <= tokens.size() &&
								tokens[i + 1]->lexem != "senao")){
					commands.push_back(new Command(name_token, bool_evaluation, aux_name1,
									c1));
					
					// for "se(expr) entao command senao se(expr) entao command..."
				} else if(name == "se" && i+2 <= tokens.size()) {
					aux_name2 = tokens[++i];
					// is block.
					if(tokens[++i]->lexem == "{"){
						baux2 = getBlock(&i, tokens);
						c2 = new Command(baux2);
						i--;
						
						// is an expression.
					} else if(tokens[i]->type == Token::NAME) {
						i--;
						while(tokens[++i]->lexem != ";"){
							expr_tokens.push_back(tokens[i]);
							if(i + 1 >= tokens.size())
								throw new SyntaticException(tokens[i],
												string("Expected ';', got: '") + tokens[i]->lexem
												+ string("'"));
						}
						eaux2 = resolve(expr_tokens);
						c2 = new Command(eaux2);
						expr_tokens.clear();
						
						// is another command.
					} else if(tokens[i]->type == Token::COMMAND){
						vector<Command*> following = getCommands(&i, tokens, 1);
						c2 = following[0];
						i--;
					} else if(tokens[i]->lexem == ";") {
						c2 = new Command();
					} else {
						throw new SyntaticException(tokens[i],
										"not a Block, Command or Expression");
					}

					if(c2 == NULL)
						throw new SyntaticException(tokens[i-1],
										"Invalid block, command or expression to be executed");

					commands.push_back(new Command(name_token, bool_evaluation, aux_name1,
									c1, aux_name2, c2));
				}


				
				// nothing else should be considered. Only expressions and commands.
			} else {
				throw new SyntaticException(tokens[i],
								string("Invalid expression or command. Found " )
								+ tokens[i]->type_string() + string(" '")
								+ tokens[i]->lexem + string("'"));
			}
		}
		i++;
	}
	*position = i;
	return commands;
}

Parser::~Parser() {
}

