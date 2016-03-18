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
			cout << getWord(line, line_number, &start) << endl;
		}
	}
}

string FileReader::getWord(string line, int line_number, int* start /*= 0*/){
	if(*start < 0 || *start > line.length()){
		*start = -1;
		return "";
	}
	
	string partial = *start == 0 ? line : line.substr(*start);
	if(partial.empty()){
		*start = -1;
		return "";
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
			return word;
		}
		if(c == ' ' || c == '\t')
			return word;
		if(c == '\n'){
			*start = -1;
			return word;
		}
	}
	return word;
}

string FileReader::getString(string partial, int line_number, int i, int* start){
	if(partial.at(i) != '\"'){
		cout << "Error at FileReader::getString(string,int,int,int*)\n";
		cout << "\tString does not begin with \" \" \" character" << endl;
		exit(1);
	}
	string res = "\"";
	for(int j = i+1; j < partial.length(); j++){
		*start += 1;
		if(partial.at(j) == '\n'){
			cout << "Unterminated string at line " << line_number << ", col: " << *start << endl;
			exit(1);
		} else {
			res += partial.at(j);
		}
		
		if(res.at(j) == '\"')
			return res;
	}
}
string FileReader::getCar(string partial, int line_number, int i, int* start) {
	if(partial.at(i) != '\''){
		cout << "Error at FileReader::getCar(string,int,int,int*)\n";
		cout << "\tChar does not begin with \" \' \" character" << endl;
		exit(1);
	}
	
	string res = "\'";
	*start += 1;
	if(partial.at(i+1) == '\n'){
		cout << "Unterminated character at line " << line_number << ", col: " << *start << endl;
		exit(1);
	}
	if(partial.at(i+1) == '\\'){
		*start += 1;
		res += partial.at(i+1);
		if(partial.at(i+2) == '\''){
			cout << "Invalid character at line " << line_number << ", col: " << *start << endl;
			exit(1);
		}
		res += partial.at(i+2);
		*start += 1;
		if(partial.at(i+3) != '\''){
			cout << "Invalid character at line " << line_number << ", col: " << *start << endl;
			exit(1);
		}
		res += partial.at(i+3);
		return res;
	}
	*start += 1;
	res += partial.at(i+1);
	*start += 1;
	if(partial.at(i+2) != '\''){
		cout << "Invalid character at line " << line_number << ", col: " << *start << endl;
		exit(1);
	}
	res += partial.at(i+2);
	return res;
}

string FileReader::getNumber(string partial, int line_number, int i, int* start){
	string res = "";
	for(int j = i; j < partial.length(); j++){
		switch(partial.at(j)){
			case ' ':
			case '\t':
			case '\n':
				return res;
			default:{
				if(is_operator(partial.at(j))){
					*start -= 1;
					return res;
				}
				if(partial.at(j) < '0' || partial.at(j) > '9'){
					cout << "Invalid number with '" << partial.at(j) << "' at line "
									<< line_number << ", col: " << *start << endl;
					exit(1);
				} else {
					res += partial.at(j);
				}
			}
		}
		*start += 1;
	}
}

string FileReader::getOperator(string partial, int line_number, int i, int* start){
	char first = partial.at(i);
	if(!is_operator(first)){
		cout << first << " is not an operator, at line: " << line_number << ", col: " << *start << endl;
		exit(1);
	}
	*start += 1;
	if(partial.length() <= i+1){
		*start -= 1;
		return string(1, first);
	}
	char second = partial.at(i+1);
	if(is_operator(second)){
		string operator2 = string({first, second, '\0'});
		for(vector<string>::iterator it = this->operators2.begin();
						it < this->operators2.end();
						it++){
			if(*it == operator2){
				return operator2;
			}
		}
		*start -= 1;
		return string(1, first);
	} else {
		*start -= 1;
		return string(1, first);
	}
}

bool FileReader::is_keyword(string word){
	for(vector<string>::iterator it = this->keywords.begin();
					it != this->keywords.end();
					it++){
		if(*it == word)
			return true;
	}
	return false;
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

vector<Token> FileReader::getTokens(){
	return this->tokens;
}

FileReader::~FileReader() {
	this->source.close();
}

