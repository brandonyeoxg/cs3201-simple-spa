#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

#include "VarTable.h"

void VarTable::insert(string key, int lineNum) {
  //inserts the line number to the unordered map of vectors containing the variable as key.
  if (varTable.find(key) == varTable.end) {
    //if the key is not present in varTable
    vector<int> lineNums;
    lineNums.push_back(lineNum);
    varTable.emplace(key, lineNums);
  } else {
    //if not, retrieve the existing vector, append, and put back to varTable.
    vector<int> vect = varTable.at(key);
    vect.push_back(lineNum);
    varTable[key] = vect;
  }

}

vector<int> VarTable::get(string var) {
  //return the vector containing all line numbers that the variable var.
  return;
}

VarTable::VarTable() {
  //constructor
  unordered_map<string, vector<int>> varTable;
}




