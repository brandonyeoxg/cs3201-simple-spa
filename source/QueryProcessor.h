#pragma once
#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H
#include "PKB.h"
#include "QueryPreProcessor.h"
#include "QueryEvaluator.h"
#include "QueryResultProjector.h"
#include <iostream>
#include <string>
#include <locale>

class QueryProcessor {
public:
  /**
  * QueryProcessor constructor.
  * Takes in the PKB as a pointer to be passed into the constructor for 
  * QueryEvaluator.
  */
  QueryProcessor(PKB *t_pkb)
    : m_pkb(t_pkb) {};

  /**
  * Default QueryProcessor deconstructor.
  */
  ~QueryProcessor() {};

  /**
  * Method that takes in a string from the TestWrapper function to be parsed by
  * the Query segment of the SPA.
  * @param t_stringInput is the string passed from TestWrapper
  * @return a list of strings containing the evaluated answers. return empty list if false.
  */
  std::list<std::string> QueryProcessor::runQueryProcessor(std::string t_stringInput);

private:
  PKB *m_pkb;
  bool isTokenized;
};

#endif QUERYPROCESSOR_H