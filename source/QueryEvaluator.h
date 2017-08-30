#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#ifndef QUERYEVALUATOR_H
#define QUERYEVALUATOR_H

/**
* A class that does the evaluation of the queries.
*/

class QueryEvaluator {
public:
  static std::vector<std::string> getResult(std::vector<std::string> t_query);
  static void storeResult(std::vector<std::string> t_result);
  static std::vector<std::string> getFinalResult(std::vector<std::string> t_listOfResults);
};

#endif QUERYEVALUATOR_H