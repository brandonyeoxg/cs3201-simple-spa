#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "QueryEvaluator.h"

/**
* A function that gets the result of the query by calling the API from PKB.
* @param t_query a vector<string> argument
* @return The query results
*/
vector<string> QueryEvaluator::getResult(vector<string> t_query) {
  vector<string> result;

  //This part needs to be re-implement after discussing with group...
  for (int i = 0; i < t_query.size(); i++) {
    if (t_query[i].find("Follows") != string::npos) {
      //call the API from PKB - suchThat()
      cout << "Calling suchThat(Follows) API from PKB...\n";
    }
    else if (t_query[i].find("Parent") != string::npos) {
      //call the API from PKB - suchThat()
      cout << "Calling suchThat(Parent) API from PKB...\n";
    }
    else {
      //call the API from PKB - select() or pattern()
      cout << "Calling select() or pattern() API from PKB...\n";
    }
  }

  return result;
}

/**
* A function that stores the result in a data structure.
* @param t_result a vector<string> argument
*/
void QueryEvaluator::storeResult(vector<string> t_result) {
  //This part also needs to be re-implement after discussing with group...
  cout << "Storing the result to a suitable data structure...\n";
}

/**
* A function that gets the final result of the query by comparing the results based on what the query wants.
* @param t_listOfResults a vector<string> argument
* @return The query results
*/
vector<string> QueryEvaluator::getFinalResult(vector<string> t_listOfResults) {
  vector<string> finalResult;

  cout << "Compare all the results and get the final result based on select()...\n";

  return finalResult;
}