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
	while(tokens[pos]->type == Token::TYPE)
		t->functions.push_back(checkFunction(&pos));
	
	if(tokens[pos]->lexem == "programa"){
		// get block programa
	} else {
		throw new SyntaticException(tokens[pos],
						"Expected function or 'programa' block, got: '" + tokens[pos]->lexem + "'");
	}
	
	cout << "Position: " << pos << endl;
	
}

Function* Parser::checkFunction(int* position){
	Token *func_type, *func_name;
	vector<Parameter*> args;
	int pos = *position;
	// should be a function
	if(this->tokens[pos]->type == Token::TYPE){
		func_type = this->tokens[pos];
		if(func_name = this->tokens[++pos], this->tokens[pos]->type == Token::NAME && this->tokens[++pos]->lexem == "("){
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
		}
	}
	++pos;
	*position = pos;
	return new Function(func_type, func_name, getBlock(position), args);
}

// INCOMPLETE!!!
Block* Parser::getBlock(int* position){
	int pos = *position;
	if(this->tokens[pos]->lexem != "{"){
		throw new SyntaticException(this->tokens[pos],
						"Should be '{'. Found: '" + this->tokens[pos]->lexem + "'");
	}
	
	pos++;
	vector<Variable*> variables = getVariableDeclarations(&pos);
	vector<Command*> commands = getCommands(&pos);
	pos--;
	
	cout << "VARIABLES DETECTED: " << variables.size() << endl << endl;
	for(int i = 0; i < variables.size(); i++){
		variables[i]->printVariable();
	}
	cout << "\nEND OF VARIABLES DETECTION\n\n";
	cout << "COMMANDS DETECTED: " << commands.size() << endl << endl;
	for(int i = 0; i < commands.size(); i++){
		commands[i]->printCommand();
	}
	cout << "\nEND OF COMMANDS DETECTION\n\n";
	
	while(this->tokens[++pos]->lexem != "}"){
		cout << "I Have: " << this->tokens[pos]->lexem << endl;
		if(pos == this->tokens.size() - 1)
			throw new SyntaticException(this->tokens[pos],
							string("Expected '}', got: '") + this->tokens[pos]->lexem
							+ string("'"));
	}
	
	++pos;
	*position = pos;
	
	return NULL;
}

vector<Variable*> Parser::getVariableDeclarations(int* position) {
	vector<Variable*> variables;
	int pos = *position;
	bool found_semicolem;
	// full declaration: type name ...
	while(true){
		// partial declaration: name, name ...
		Token* type = this->tokens[pos];
		if(type->type != Token::TYPE){
			break;
		}
		found_semicolem = false;
		while(!found_semicolem){
			vector<Token*> var_tokens;
			string lex = this->tokens[++pos]->lexem;
			var_tokens.push_back(type);
			while(lex != "," && lex != ";"){
				var_tokens.push_back(this->tokens[pos]);
				if(pos + 1 >= this->tokens.size())
					throw new SyntaticException(this->tokens[pos],
									"Unexpected end of variable declaration");
				lex = this->tokens[++pos]->lexem;
			}
			int pos2 = 0;
			Variable* var = getVariable(var_tokens, &pos2);
			
			if(pos2 != var_tokens.size())
				throw new SyntaticException(var_tokens[var_tokens.size() - 1],
								"Invalid variable declaration");
			
			var_tokens.clear();
			
			if(var == NULL)
				throw new SyntaticException(var_tokens[var_tokens.size() - 1],
								"Variable cannot be null");
			variables.push_back(var);
			if(lex == ";"){
				pos++;
				found_semicolem = true;
			}
		}
	}
	
	*position = pos;
	
	return variables;
}


Variable* Parser::getVariable(vector<Token*> tokens, int* pos){
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
	Variable* v;
	FuncCall* fc;
	
	int i = 0;
	while(i < tokens.size()){
		ExprOrOpToken* node = (ExprOrOpToken*) malloc(sizeof(ExprOrOpToken));
		node->expr = NULL;
		node->op = NULL;
		if(tokens[i]->type == Token::OPERATOR || tokens[i]->type == Token::OPERATOR2){
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
	
	// shouldn't reach this place
	if(eot[0]->expr != NULL)
		throw new SyntaticException(eot[0]->expr, "Couldn't mount expression");
	else
		throw new SyntaticException(eot[0]->op, "Couldn't mount expression");
}

vector<Command*> Parser::getCommands(int* position){
	int i = *position;
	vector<Command*> commands;
	vector<Token*> cmd_tokens, expr_tokens, block_tokens;
	
	while(true){
		if(i >= this->tokens.size() || this->tokens[i]->lexem == "}"){
			*position = i;
			return commands;
		}
		
		Token* cur = this->tokens[i];
		// can be assignment, function call, useless expression (like 3 + 5;), etc.
		if(cur->type == Token::NAME){
			if(i == this->tokens.size() - 1)
				throw new SyntaticException(cur, "Unexpected end of expression");
			cmd_tokens.push_back(cur);
			while(cur = this->tokens[++i], cur->lexem != ";"){
				cmd_tokens.push_back(cur);
				if(i == this->tokens.size() - 1)
					throw new SyntaticException(cur, "Unexpected end of expression");
			}
			commands.push_back(new Command(resolve(cmd_tokens)));
			cmd_tokens.clear();
			// is a command, like retorne 1, se(a > 3) entao a = a - 3, etc.
		} else if(cur->type == Token::COMMAND){
			Token* name_token = cur;
			string name = name_token->lexem;
			
			// we guarantee that there will be at least 1 more token.
			if(i + 1 >= this->tokens.size() - 1)
				throw new SyntaticException(cur, "Unexpected end of command");
			
			// this command take no arguments.
			if(name == "novalinha"){
				if(cur = this->tokens[++i], cur->lexem != ";")
					throw new SyntaticException(cur,
									string("Expected ';' as this command takes no arguments.")
									+ string(" Found '" + cur->lexem + string("'")));
				
				// this command takes an expression.
			} else if(name == "retorne" || name == "escreva"){
				while(this->tokens[++i]->lexem != ";"){
					expr_tokens.push_back(this->tokens[i]);
					if(i >= this->tokens.size() -1)
						throw new SyntaticException(this->tokens[i],
										"Unexpected end of expression");
				}
				Expression* expr = resolve(expr_tokens);
				expr_tokens.clear();
				if(expr == NULL)
					throw new SyntaticException(this->tokens[i], "Invalid expression");
				commands.push_back(new Command(name_token, expr));
				
				// this command takes a variable;
			} else if(name == "leia"){
				Variable* v = getVariable(this->tokens, &i);
				if(v == NULL || v->value != NULL || v->type != NULL)
					throw new SyntaticException(this->tokens[i-1],
									"Not a valid variable use");
				if(i >= this->tokens.size() - 1 || this->tokens[i+1]->lexem != ";")
					throw new SyntaticException(this->tokens[i], "Missing ';'");
				commands.push_back(new Command(name_token, v));
				
				// INCOMPLETE!!! missing enquanto, se entao senao, etc.
				
				// nothing else should be considered. Only expressions and commands.
			} else {
				throw new SyntaticException(this->tokens[i],
								string("Expected expression or command. Found " )
								+ this->tokens[i]->type_string() + string(" '")
								+ this->tokens[i]->lexem + string("'"));
			}
		}
		i++;
	}
	
	*position = i;
	return commands;
}

Command* Parser::getCommand(vector<Token*> tokens){
	Command* command;
	for(int i = 0; i < tokens.size(); i++)
		tokens[i]->printToken();
	return command;
}

Parser::~Parser() {
}

