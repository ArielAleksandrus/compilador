/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   circular_dep.h
 * Author: aleksandrus
 *
 * Created on April 6, 2016, 8:08 PM
 */

#ifndef CIRCULAR_DEP_H
#define CIRCULAR_DEP_H
#include <fstream>
typedef struct Utils{
    std::ofstream out;
}Utils;
class Block;
class Command;
class Expression;
class FileReader;
class FuncCall;
class Function;
class Parameter;
class Parser;
class SemanticException;
class SyntaticException;
class SymbolTable;
class Tree;
class Token;
class Unimplemented;
class Variable;

#endif /* CIRCULAR_DEP_H */

