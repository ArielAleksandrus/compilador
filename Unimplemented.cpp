/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Unimplemented.cpp
 * Author: aleksandrus
 * 
 * Created on March 18, 2016, 9:57 PM
 */

#include "Unimplemented.h"

using namespace std;

Unimplemented::Unimplemented(string msg, bool terminate /* = true */) {
	cout << "DEVELOPPER ERROR => " + msg << endl;
	exit(1);
}

Unimplemented::~Unimplemented() {
}

