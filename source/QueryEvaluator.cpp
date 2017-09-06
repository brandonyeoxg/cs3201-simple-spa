#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>

#include "QueryEvaluator.h"
#include "QueryPreProcessor.h"
#include "Grammar.h"
#include "DesignAbstraction.h"
#include "Pattern.h"
#include "PKB.h"

/**
* A function that evaluates the query that has been pre-processed by the QueryPreprocessor.
*/
std::list<std::string> QueryEvaluator::evaluateQuery() {
  std::cout << "Evaluating Query...\n";
  bool hasResult = getResultFromPkb();
  if (hasResult) {
    return evaluateFinalResult();
  } else if (!hasResult) {
    std::list<std::string> result;
    return result;
  }
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
        result = m_pkb->returnFollowTable(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        result = m_pkb->returnFollowTable(std::stoi(relation.getG1().getName()), VARIABLE_S1);
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        result = m_pkb->returnFollowTable(VARIABLE_S1, std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        result = m_pkb->returnFollowTable(VARIABLE_S1, VARIABLE_S2);
      }
    } else if (relation.getType() == DesignAbstraction::DAType::FOLLOWS_) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        result = m_pkb->returnFollowTable(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        result = m_pkb->returnFollowTable(std::stoi(relation.getG1().getName()), VARIABLE_S1);
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        result = m_pkb->returnFollowTable(VARIABLE_S1, std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        result = m_pkb->returnFollowTable(VARIABLE_S1, VARIABLE_S2);
      }
    } else if (relation.getType() == DesignAbstraction::DAType::PARENT) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        result = m_pkb->returnParentTable(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        result = m_pkb->returnParentTable(std::stoi(relation.getG1().getName()), VARIABLE_S1);
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        result = m_pkb->returnParentTable(VARIABLE_S1, std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        result = m_pkb->returnParentTable(VARIABLE_S1, VARIABLE_S2);
      }
    } else if (relation.getType() == DesignAbstraction::DAType::PARENT_) {
      if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::CONST) {
        result = m_pkb->returnParentTable(std::stoi(relation.getG1().getName()), std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::CONST && relation.getG2().getType() == Grammar::GType::STMT) {
        result = m_pkb->returnParentTable(std::stoi(relation.getG1().getName()), VARIABLE_S1);
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::CONST) {
        result = m_pkb->returnParentTable(VARIABLE_S1, std::stoi(relation.getG2().getName()));
      } else if (relation.getG1().getType() == Grammar::GType::STMT && relation.getG2().getType() == Grammar::GType::STMT) {
        result = m_pkb->returnParentTable(VARIABLE_S1, VARIABLE_S2);
      }
    } else {
      cout << "Relation Type: " + relation.getType();
    }

    if (result.empty()) {
      return false;
    }

    if (relation.getG1().getType() == Grammar::GType::STMT || relation.getG2().getType() == Grammar::GType::STMT) {
      if (relation.getG1().getName() == m_selectedSynonym || relation.getG2().getName() == m_selectedSynonym) {
        storeResultFromPkb(result, queryType::RELATION);
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
void QueryEvaluator::storeResultFromPkb(std::unordered_map<int, std::vector<int>> t_result, queryType t_type) {
  //This part also needs to be re-implement after discussing with group...
  std::cout << "Storing the result from PKB to different queues...\n";

  if (t_type == queryType::SELECT) {
    //m_selectResults.push(t_result);
  } else if (t_type == queryType::RELATION) {
    m_relationResults.push(t_result);
  } else if (t_type == queryType::PATTERN) {
    m_patternResults.push(t_result);
  } else {
    cout << "Result Type: " + t_type;
  }
}

/**
* A function that evaluates the final result of the query by comparing the results based on what the query wants.
* @return The query results
*/
std::list<std::string> QueryEvaluator::evaluateFinalResult() {
  std::cout << "Evaluating the final result...\n";
  std::list<std::string> finalResult;

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