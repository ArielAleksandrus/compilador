/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SyntaticException.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 8:54 PM
 */

#include "SyntaticException.h"

using namespace std;

SyntaticException::SyntaticException(Token* token, string msg) {
	SyntaticException(token->line_number, token->col_number, msg);
}
SyntaticException::SyntaticException(int line_number, int col_number,
				string msg, bool terminate /* = true */) {
	cout << "ERROR => Line " << line_number << ", col " << col_number
					<< ": " << msg << endl;
	
	if(terminate)
		exit(1);
}

SyntaticException::~SyntaticException() {
}

