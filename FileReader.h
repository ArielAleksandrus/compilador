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

class FileReader {
public:
	/**
	 * 
	 * 	 * @param path path to the source code.
	 */
	FileReader(std::string path);
	std::vector<Token> getTokens();
	virtual ~FileReader();
	
private:
	
	void genTokens();
	std::string getWord(std::string line, int* start);
	bool is_keyword(std::string word);
	bool is_operator(char c);
	
	std::vector<Token> tokens;
	std::ifstream source;
	
	std::vector<char> operators = {
		'(', ')', '[', ']', '{', '}',
		'+', '-', '*', '/',
		'=', '!',
		';'
	};
	std::vector<std::string> operators2 = {
		"==", "<=", ">=", "!=",
		"&&", "||"
	};
	std::vector<std::string> keywords = {
		"programa",
		"int",
		"car"
	};
};

#endif /* FILEREADER_H */

