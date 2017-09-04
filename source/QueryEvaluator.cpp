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
#include "PKB.h"

/**
* A function that evaluates the query that has been pre-processed by the QueryPreprocessor.
*/
void QueryEvaluator::evaluateQuery() {
  std::cout << "Evaluating Query...\n";
  getQueryClauses();
  bool hasResult = getResultFromPkb();
  if (hasResult) {
    storeFinalResult(evaluateFinalResult());
  } else if (!hasResult) {
    std::vector<std::string> result;
    storeFinalResult(result);
  }
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
  std::cout << "Getting the queues that stores the clauses of the query from QueryPreProcessor.\n";
  QueryPreProcessor qpp;
  m_selects = qpp.getSelect();
  m_relations = qpp.getSuchThat();
  //m_patterns = qpp.getPattern();
}

/**
* A function that gets the result of the clauses by calling the API from PKB.
* @return true if there are results otherwise false
*/
bool QueryEvaluator::getResultFromPkb() {
  std::cout << "Getting results from PKB...\n";
  int selectSize = m_selects.size();
  int relationSize = m_relations.size();
  int patternSize = m_patterns.size();

  for (int i = 0; i < selectSize; ++i) {
    Grammar grammar = m_selects.front();
    m_selectedSynonym = grammar.getName();
    // Todo: Get results for select by calling the API from PKB
    m_selects.pop();
    m_selects.push(grammar);
  }

  for (int i = 0; i < relationSize; ++i) {
    std::unordered_map<int, std::vector<int>> result;
    DesignAbstraction relation = m_relations.front();
    if (relation.getType() == DesignAbstraction::DAType::FOLLOWS) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        //result = m_pkb->returnFollowTable(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        //result = m_pkb->returnFollowTable(std::stoi(relation.getG1().getName()), VARIABLE_S1);
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        //result = m_pkb->returnFollowTable(VARIABLE_S1, std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        //result = m_pkb->returnFollowTable(VARIABLE_S1, VARIABLE_S2);
      }
    } else if (relation.getType() == DesignAbstraction::DAType::FOLLOWS_) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        //result = m_pkb->returnFollowTable(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        //result = m_pkb->returnFollowTable(std::stoi(relation.getG1().getName()), VARIABLE_S1);
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        //result = m_pkb->returnFollowTable(VARIABLE_S1, std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        //result = m_pkb->returnFollowTable(VARIABLE_S1, VARIABLE_S2);
      }
    } else if (relation.getType() == DesignAbstraction::DAType::PARENT) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        //result = m_pkb->returnParentTable(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        //result = m_pkb->returnParentTable(std::stoi(relation.getG1().getName()), VARIABLE_S1);
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        //result = m_pkb->returnParentTable(VARIABLE_S1, std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        //result = m_pkb->returnParentTable(VARIABLE_S1, VARIABLE_S2);
      }
    } else if (relation.getType() == DesignAbstraction::DAType::PARENT_) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        //result = m_pkb->returnParentTable(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        //result = m_pkb->returnParentTable(std::stoi(relation.getG1().getName()), VARIABLE_S1);
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        //result = m_pkb->returnParentTable(VARIABLE_S1, std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        //result = m_pkb->returnParentTable(VARIABLE_S1, VARIABLE_S2);
      }
    } else {
      cout << "Relation Type: " + relation.getType();
    }

    if (result.empty()) {
      return false;
    }

    if (relation.getG1().getType() == Grammar::GType::STMT || relation.getG2().getType() == Grammar::GType::STMT) {
      if (relation.getG1().getName() == m_selectedSynonym || relation.getG2().getName() == m_selectedSynonym) {
        storeResultFromPkb(result, RELATION);
        m_relations.push(relation);
      }
    }
    
    m_relations.pop();
  }

  for (int i = 0; i < patternSize; ++i) {
    Pattern pattern = m_patterns.front();
    // Todo: Get results for pattern clauses by calling the API from PKB
    m_patterns.pop();
    m_patterns.push(pattern);
  }

  return true;
}

/**
* A function that stores the result in a data structure.
* @param t_result a vector<string> argument
* @param t_type a string argument
*/
void QueryEvaluator::storeResultFromPkb(std::unordered_map<int, std::vector<int>> t_result, std::string t_type) {
  //This part also needs to be re-implement after discussing with group...
  std::cout << "Storing the result from PKB to different queues...\n";

  if (t_type.compare(SELECT) == 0) {
    //m_selectResults.push(t_result);
  } else if (t_type.compare(RELATION) == 0) {
    m_relationResults.push(t_result);
  } else if (t_type.compare(PATTERN) == 0) {
    m_patternResults.push(t_result);
  } else {
    cout << "Result Type: " + t_type;
  }
}

/**
* A function that evaluates the final result of the query by comparing the results based on what the query wants.
* @return The query results
*/
std::vector<std::string> QueryEvaluator::evaluateFinalResult() {
  std::cout << "Evaluating the final result...\n";
  std::vector<std::string> finalResult;

  if (m_relationResults.empty() && m_patternResults.empty()) {
    if (!m_selectResults.empty()) {
      finalResult = m_selectResults.front();
    } 
  }

  while (!m_relationResults.empty()) {
    //Todo: format result to vector<string>
    unordered_map<int, std::vector<int>> resultMap = m_relationResults.front();
    DesignAbstraction relation = m_relations.front();
    if (relation.getType() == DesignAbstraction::DAType::FOLLOWS || relation.getType() == DesignAbstraction::DAType::FOLLOWS_) {
      if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        std::unordered_map<int, vector<int>>::const_iterator getVector = resultMap.find(std::stoi(relation.getG1().getName()));
        vector<int> stmtVector = getVector->second;
        for (std::vector<int>::iterator getStmts = stmtVector.begin(); getStmts != stmtVector.end(); ++getStmts) {
          finalResult.push_back(std::to_string(*getStmts));
        }
      } else if (relation.getG2().getType() == Grammar::GType::STMT && relation.getG1().getType() == Grammar::GType::CONST) {
        std::unordered_map<int, vector<int>>::const_iterator getVector = resultMap.find(std::stoi(relation.getG2().getName()));
        vector<int> stmtVector = getVector->second;
        for (std::vector<int>::iterator getStmts = stmtVector.begin(); getStmts != stmtVector.end(); ++getStmts) {
          finalResult.push_back(std::to_string(*getStmts));
        }
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        //Todo: Get the results for query Select s1 such that Follows (s1, s2) or Select s2 such that Follows (s1, s2)
      }
    }
    else if (relation.getType() == DesignAbstraction::DAType::PARENT || relation.getType() == DesignAbstraction::DAType::PARENT_) {
      if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        std::unordered_map<int, vector<int>>::const_iterator getVector = resultMap.find(std::stoi(relation.getG1().getName()));
        vector<int> stmtVector = getVector->second;
        for (std::vector<int>::iterator getStmts = stmtVector.begin(); getStmts != stmtVector.end(); ++getStmts) {
          finalResult.push_back(std::to_string(*getStmts));
        }
      }
      else if (relation.getG2().getType() == Grammar::GType::STMT && relation.getG1().getType() == Grammar::GType::CONST) {
        std::unordered_map<int, vector<int>>::const_iterator getVector = resultMap.find(std::stoi(relation.getG2().getName()));
        vector<int> stmtVector = getVector->second;
        for (std::vector<int>::iterator getStmts = stmtVector.begin(); getStmts != stmtVector.end(); ++getStmts) {
          finalResult.push_back(std::to_string(*getStmts));
        }
      }
      else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        //Todo: Get the results for query Select s1 such that Follows (s1, s2) or Select s2 such that Follows (s1, s2)
      }
    } else if (relation.getType() == DesignAbstraction::DAType::USES) {
      //Todo: Get the results for Uses
    } else if (relation.getType() == DesignAbstraction::DAType::MODIFIES) {
      //Todo: Get ther results for Modifies
    } else {
      cout << "Relation Type: " + relation.getType();
    }

    m_relations.pop();
    m_relationResults.pop();
  }

  while (!m_patternResults.empty()) {
    //Todo: format result to vector<string>
  }

  return finalResult;
}

/**
* A function that stores the final result in a data structure.
* @param t_result a vector<string> argument
*/
void QueryEvaluator::storeFinalResult(std::vector<std::string> t_result) {
  std::cout << "Storing the final result...\n";
  m_queryResult = t_result;
}