#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifndef QUERYEVALUATOR_H
#define QUERYEVALUATOR_H

/**
* A class that does the evaluation of the queries.
*/

class QueryEvaluator {
public:
  static vector<string> getResult(vector<string> t_query);
  static void storeResult(vector<string> t_result);
  static vector<string> getFinalResult(vector<string> t_listOfResults);
};

#endif QUERYEVALUATOR_H