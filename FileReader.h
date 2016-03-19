/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileReader.h
 * Author: aleksandrus
 *
 * Created on March 18, 2016, 11:07 AM
 */

#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "Token.h"
#include "SyntaticException.h"

class FileReader {
public:
	/**
	 * FileReader reads the source code file and generate Tokens.
	 * 
	 * 	@param path Path to the source code to be compiled.
	 */
	FileReader(std::string path);
	std::vector<Token*> getTokens();
	void printTokens();
	virtual ~FileReader();
	
private:
	
	void genTokens();
	
	Token* getWord(std::string line, int line_number, int* start);
	Token* getString(std::string partial, int line_number, int i, int* start);
	Token* getCar(std::string partial, int line_number, int i, int* start);
	Token* getNumber(std::string partial, int line_number, int i, int* start);
	Token* getOperator(std::string partial, int line_number, int i, int* start);
	
	/**
	 * Checks if it is a keyword. If true, returns the type of the keyword
	 * (e.g.: TYPE, COMMAND), based on the constants declared in Token.h.
	 * 
	 * 	@return Token::NAME If not a keyword
	 *	@return Token::<type of keyword> If is a keyword
	 */
	int keyword(std::string word);
	
	bool is_operator(char c);
	
	std::vector<Token*> tokens;
	std::ifstream source;
	
	std::vector<char> operators = {
		'(', ')', '[', ']', '{', '}',
		'+', '-', '*', '/',
		'=', '!',
		',', ';', '?', ':',
		'>', '<'
	};
	std::vector<std::string> operators2 = {
		"==", "<=", ">=", "!=",
		"&&", "||"
	};
	std::vector<std::string> commands = {
		"programa",
		"retorne",
		"se",
		"enquanto"
	};
	std::vector<std::string> types = {
		"int",
		"car"
	};
};

#endif /* FILEREADER_H */

