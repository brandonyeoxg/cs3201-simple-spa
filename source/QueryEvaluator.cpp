#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "QueryEvaluator.h"

/**
* A function that gets the result of the query by calling the API from PKB.
* @param t_query a vector<string> argument
* @return The query results
*/
std::vector<std::string> QueryEvaluator::getResult(std::vector<std::string> t_query) {
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
void QueryEvaluator::storeResult(std::vector<std::string> t_result) {
  //This part also needs to be re-implement after discussing with group...
  std::cout << "Storing the result to a suitable data structure...\n";
}

/**
* A function that gets the final result of the query by comparing the results based on what the query wants.
* @param t_listOfResults a vector<string> argument
* @return The query results
*/
std::vector<std::string> QueryEvaluator::getFinalResult(std::vector<std::string> t_listOfResults) {
  std::vector<std::string> finalResult;

  std::cout << "Compare all the results and get the final result based on select()...\n";

  return finalResult;
}