/**
* Query preprocessing class. This class creates Grammar Objects and Design Abstraction Objects
* from the user input.
*/

#pragma once
#ifndef QUERYPREPROCESSOR_H
#define QUERYPREPROCESSOR_H
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class QueryPreProcessor {
public:
  QueryPreProcessor();
  ~QueryPreProcessor();
  vector<string> QueryPreProcessor::tokenizeInput(string);
};

#endif QUERYPREPROCESSOR_H
