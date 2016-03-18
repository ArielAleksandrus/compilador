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
	while(getline(this->source, line)){
		int start = 0;
		while(start >= 0){
			cout << getWord(line, &start) << endl;
		}
	}
}

string FileReader::getWord(string line, int* start /*= 0*/){
	if(*start < 0)
		return "";
	
	string partial = *start == 0 ? line : line.substr(*start);
	if(line.length() == *start){
		*start = -1;
		return "";
	}
	string word = "";
	for(char c : partial){
		*start += 1;
		
		if(c == '\n' || c == EOF){
			*start = -1;
			return "";
		}
		if(c == '\t' || c == ' ' || c == '\0')
			return word;
		
		if(is_operator(c)){
			if(word == "")
				return string(1, c);
			else
				return word;
		} else {
			word += c;
		}
	}
	return word;
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

