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
  static void evaluateQuery();
  static std::vector<std::string> getQueryResult();

private:
  static std::vector<std::string> m_queryResult;
  static std::vector<std::string> getResultFromPkb(std::vector<std::string> t_query);
  static void storeResultFromPkb(std::vector<std::string> t_result);
  static std::vector<std::string> evaluateFinalResult(std::vector<std::string> t_listOfResults);
  static void storeFinalResult(std::vector<std::string> t_result);
};

#endif QUERYEVALUATOR_H