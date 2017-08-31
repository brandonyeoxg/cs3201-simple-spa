#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include "QueryEvaluator.h"
#include "QueryPreProcessor.h"
#include "Grammar.h"
#include "DesignAbstraction.h"
#include "Pattern.h"


/**
* Default Constructor
*/
QueryEvaluator::QueryEvaluator() {
  std::cout << "Creating a QueryEvaluator object...\n";
};

/**
* A function that evaluates the query that has been pre-processed by the QueryPreprocessor.
*/
void QueryEvaluator::evaluateQuery() {
  std::cout << "Evaluating Query...\n";
}

/**
* A function that gets the result of the query.
* @return the result of the query
*/
std::vector<std::string> QueryEvaluator::getQueryResult() { 
  std::cout << "Getting the query result...\n";
  return m_queryResult;
}

/**
* A function that gets the clauses of the query stored in a data structure in QueryPreProcessor.
*/
void QueryEvaluator::getQueryClauses() {
  std::cout << "Getting the queue that stores the clauses of the query from QueryPreProcessor.\n";
}

/**
* A function that store the clauses of the query in a data structure in QueryEvaluator.
*/
void QueryEvaluator::storeQueryClauses() {
  std::cout << "Storing the clauses of the query from QueryPreProcessor into different queues.\n";
}

/**
* A function that gets the result of the clauses by calling the API from PKB.
* @param t_query a vector<string> argument
* @return the result of the clauses
*/
std::vector<std::string> QueryEvaluator::getResultFromPkb(std::vector<std::string> t_query) {
  std::cout << "Getting results from PKB...\n";
  std::vector<std::string> result;

  //This part needs to be re-implement after discussing with group...
  for (int i = 0; i < t_query.size(); i++) {
    if (t_query[i].find("Follows") != std::string::npos) {
      //call the API from PKB - suchThat()
      std::cout << "Calling suchThat(Follows) API from PKB...\n";
    } else if (t_query[i].find("Parent") != std::string::npos) {
      //call the API from PKB - suchThat()
      std::cout << "Calling suchThat(Parent) API from PKB...\n";
    } else {
      //call the API from PKB - select() or pattern()
      std::cout << "Calling select() or pattern() API from PKB...\n";
    }
  }

  return result;
}

/**
* A function that stores the result in a data structure.
* @param t_result a vector<string> argument
*/
void QueryEvaluator::storeResultFromPkb(std::vector<std::string> t_result) {
  //This part also needs to be re-implement after discussing with group...
  std::cout << "Storing the result from PKB to different queues...\n";
}

/**
* A function that evaluates the final result of the query by comparing the results based on what the query wants.
* @param t_listOfResults a vector<string> argument
* @return The query results
*/
std::vector<std::string> QueryEvaluator::evaluateFinalResult(std::vector<std::string> t_listOfResults) {
  std::cout << "Evaluating the final result...\n";
  std::vector<std::string> finalResult;

  return finalResult;
}

/**
* A function that stores the final result in a data structure.
* @param t_result a vector<string> argument
*/
void QueryEvaluator::storeFinalResult(std::vector<std::string> t_result) {
  std::cout << "Storing the final result...\n";
}