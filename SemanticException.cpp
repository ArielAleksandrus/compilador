/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SemanticException.cpp
 * Author: aleksandrus
 * 
 * Created on April 6, 2016, 10:51 AM
 */

#include "SemanticException.h"

using namespace std;

SemanticException::SemanticException(Token* token, string msg,
				bool terminate /* = true */) {
	cout << "ERROR => Line " << token->line_number << ", col " << token->col_number
					<< ": " << msg << endl;
	
	if(terminate)
		exit(1);
}

SemanticException::SemanticException(int type, Token* token, Token* aux_token,
				string msg, bool terminate /* = true */){
	cout << "ERROR => Line " << token->line_number << ", col " << token->col_number
					<< ": " << msg;
	
	string s = "";
	switch(type){
		case (PREVIOUSLY_FOUND): {
			s = "'" + aux_token->lexem + "' previously found in ";
			break;
		}
		default:
			throw new Unimplemented("Unimplemented Semantic Exception type '"
							+ to_string(type) + "'");
	}
	
	cout << ". " << s << "line " << aux_token->line_number << ", col "
					<< aux_token->col_number << endl;
	
	if(terminate)
		exit(1);
}

SemanticException::~SemanticException() {
}

