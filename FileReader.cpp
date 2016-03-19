/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileReader.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 11:07 AM
 */

#include <valarray>
#include <stdexcept>

#include "FileReader.h"

using namespace std;

FileReader::FileReader(string path) {
	this->source.open(path.c_str());
	if(!source.is_open()){
		cout << "File '" + path + "' not found" << endl;
		exit(1);
	}
	genTokens();
}

void FileReader::genTokens(){
	string line;
	int line_number = 0;
	while(getline(this->source, line)){
		line_number++;
		int start = 0;
		while(start >= 0){
			Token* t = getWord(line, line_number, &start);
			if(t != NULL)
				this->tokens.push_back(t);
		}
	}
}

Token* FileReader::getWord(string line, int line_number, int* start /*= 0*/){
	if(*start < 0 || *start > line.length()){
		*start = -1;
		return NULL;
	}
	
	string partial = *start == 0 ? line : line.substr(*start);
	if(partial.empty()){
		*start = -1;
		return NULL;
	}
	string word = "";
	for(int i = 0; i < partial.length(); i++){
		*start += 1;
		char c = partial.at(i);
		if(word == ""){
			if(c == '\"')
				return getString(partial, line_number, i, start);
			else if(c == '\'')
				return getCar(partial, line_number, i, start);
			else if(c >= '0' && c <= '9')
				return getNumber(partial, line_number, i, start);
			else if(is_operator(c))
				return getOperator(partial, line_number, i, start);
		}
		
		if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
			word += c;
		if(is_operator(c) && !word.empty()){
			*start -= 1;
			return new Token(line_number, *start, word, keyword(word));
		}
		if(c == ' ' || c == '\t'){
			if(word.empty())
				return NULL;
			return new Token(line_number, *start - 1, word, keyword(word));
		}
		if(c == '\n'){
			*start = -1;
			if(word.empty())
				return NULL;
			return new Token(line_number, *start, word, keyword(word));
		}
	}
	
	return NULL;
}

Token* FileReader::getString(string partial, int line_number, int i, int* start){
	string res = "\"";
	for(int j = i+1; j < partial.length(); j++){
		*start += 1;
		res += partial.at(j);
		
		if(res.at(j) == '\"'){
			return new Token(line_number, *start - 1, res, Token::LITERAL);
		}
	}
	
	throw new SyntaticException(line_number, *start, "Unterminated string");
}
Token* FileReader::getCar(string partial, int line_number, int i, int* start) {
	string res = "\'";
	*start += 1;
	try{
		if(partial.at(i+1) == '\\'){
			*start += 1;
			res += partial.at(i+1);
			if(partial.at(i+2) == '\''){
				throw new SyntaticException(line_number, *start - 1, "Invalid character");
			}
			res += partial.at(i+2);
			*start += 1;
			if(partial.at(i+3) != '\''){
				throw new SyntaticException(line_number, *start - 1, "Invalid character");
			}
			res += partial.at(i+3);
			return new Token(line_number, *start - 1, res, Token::LITERAL);
		}
		*start += 1;
		res += partial.at(i+1);
		*start += 1;
		if(partial.at(i+2) != '\''){
			throw new SyntaticException(line_number, *start - 1, "Invalid character");
		}
		res += partial.at(i+2);
	} catch(const out_of_range& e){
		throw new SyntaticException(line_number, *start - 1, "Unfinished character declaration");
	}
	return new Token(line_number, *start - 1, res, Token::LITERAL);
}

Token* FileReader::getNumber(string partial, int line_number, int i, int* start){
	string res = "";
	for(int j = i; j < partial.length(); j++){
		switch(partial.at(j)){
			case ' ':
			case '\t':
			case '\n':
				return new Token(line_number, *start - 1, res, Token::LITERAL);
			default:{
				if(is_operator(partial.at(j))){
					*start -= 1;
					return new Token(line_number, *start, res, Token::LITERAL);
				}
				if(partial.at(j) < '0' || partial.at(j) > '9'){
					throw new SyntaticException(line_number, *start, "Invalid number");
				} else {
					res += partial.at(j);
				}
			}
		}
		*start += 1;
	}
	return NULL;
}

Token* FileReader::getOperator(string partial, int line_number, int i, int* start){
	char first = partial.at(i);
	if(!is_operator(first)){
		throw new SyntaticException(line_number, *start,
						"'" + string(1, first) + "' is not an operator.");
	}
	*start += 1;
	if(partial.length() <= i+1){
		*start -= 1;
		return new Token(line_number, *start, string(1, first), Token::OPERATOR);
	}
	char second = partial.at(i+1);
	if(is_operator(second)){
		string operator2 = string({first, second, '\0'});
		for(vector<string>::iterator it = this->operators2.begin();
						it < this->operators2.end();
						it++){
			if(*it == operator2){
				return new Token(line_number, *start, operator2, Token::OPERATOR2);
			}
		}
		*start -= 1;
		return new Token(line_number, *start, string(1, first), Token::OPERATOR);
	} else {
		*start -= 1;
		return new Token(line_number, *start, string(1, first), Token::OPERATOR);
	}
}

int FileReader::keyword(string word){
	for(vector<string>::iterator it = this->types.begin();
					it != this->types.end();
					it++){
		if(*it == word)
			return Token::TYPE;
	}
	for(vector<string>::iterator it = this->commands.begin();
					it != this->commands.end();
					it++){
		if(*it == word)
			return Token::COMMAND;
	}
	return Token::NAME;
}

bool FileReader::is_operator(char c){
	for(vector<char>::iterator it = this->operators.begin();
					it != this->operators.end();
					it++){
		if(*it == c)
			return true;
	}
	return false;
}

vector<Token*> FileReader::getTokens(){
	return this->tokens;
}

FileReader::~FileReader() {
	this->source.close();
}

void FileReader::printTokens(){
	int i = 0;
	cout << "Token <number>: (line_number, col_number, lexem, type)" << endl;
	for(vector<Token*>::iterator it = this->tokens.begin();
					it < this->tokens.end();
					it++){
		Token* t = *it;
		cout << "Token " << ++i << ": (" << t->line_number << ", " 
						<< t->col_number << ", " << t->lexem << ", " << t->type << ")" << endl;
	}
}
