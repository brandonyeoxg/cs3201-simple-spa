#pragma once

#include "QueryEvaluator.h"

/**
* A function that evaluates the query that has been pre-processed by the QueryPreprocessor.
*/
std::vector<std::string> QueryEvaluator::evaluateQuery() {
  //printDivider();
  //std::cout << "Evaluating Query...\n";

  // Debug: Check the queues
  /*printSelectQueue();
  printRelationQueue();
  printPatternQueue();*/

  bool hasResult = getResultFromPkb();
  if (hasResult) {
    return evaluateFinalResult();
  } else if (!hasResult) {
    std::vector<std::string> result;
    /*printDivider();
    std::cout << "No Query Result: \n";
    for (std::vector<std::string>::iterator getStmts = result.begin(); getStmts != result.end(); ++getStmts) {
      std::cout << *getStmts << ", ";
    }
    printDivider();*/
    return result;
  }
}

std::vector<std::string> QueryEvaluator::formatVectorIntToVectorString(std::vector<int> t_vectorInt) {
  std::vector<std::string> vectorStr;
  for (std::vector<int>::iterator getStmts = t_vectorInt.begin(); getStmts != t_vectorInt.end(); ++getStmts) {
    vectorStr.push_back(std::to_string(*getStmts));
  }

  return vectorStr;
}

std::vector<std::string> QueryEvaluator::getCommonResults(std::vector<std::string> t_resultVector1, std::vector<std::string> t_resultVector2) {
  std::vector<std::string> commonResultVector;

  sort(t_resultVector1.begin(), t_resultVector1.end());
  sort(t_resultVector2.begin(), t_resultVector2.end());

  set_intersection(t_resultVector1.begin(), t_resultVector1.end(), t_resultVector2.begin(), t_resultVector2.end(), back_inserter(commonResultVector));

  return commonResultVector;
}

bool QueryEvaluator::getSelectResultFromPkb(Grammar t_select) {
  if (t_select.getType() != Grammar::GType::VAR && t_select.getType() != Grammar::GType::CONST) {
    // Call the PKB API getStatementTypeTable().
    std::unordered_map<Grammar::GType, std::vector<int>> allStmts = m_pkb->getStatementTypeTable();

    // Check if there are results else return false.
    if (allStmts.empty()) {
      return false;
    }

    // Get all the statements of the type of statements that has been selected.
    std::vector<int> allSelectedStmtsInInt;
    if (t_select.getType() == Grammar::GType::STMT || t_select.getType() == Grammar::GType::PROG_LINE) {
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[Grammar::GType::ASGN].begin(), allStmts[Grammar::GType::ASGN].end());
      allSelectedStmtsInInt.insert(allSelectedStmtsInInt.end(), allStmts[Grammar::GType::WHILE].begin(), allStmts[Grammar::GType::WHILE].end());
    } else if (t_select.getType() == Grammar::GType::ASGN) {
      allSelectedStmtsInInt = allStmts[Grammar::GType::ASGN];
    } else if (t_select.getType() == Grammar::GType::WHILE) {
      allSelectedStmtsInInt = allStmts[Grammar::GType::WHILE];
    }

    // Change from vector<int> to vector<string>.
    std::vector<std::string> allSelectedStmts = formatVectorIntToVectorString(allSelectedStmtsInInt);

    // Push into the selectResults queue.
    storeSelectResultFromPkb(allSelectedStmts);
  } else if (t_select.getType() == Grammar::GType::VAR) {
    std::vector<std::string> allVariables = m_pkb->getAllVariables();
    if (allVariables.empty()) {
      return false;
    }
    storeSelectResultFromPkb(allVariables);
  } else if (t_select.getType() == Grammar::GType::CONST) {
    std::list<std::string> constantsList = m_pkb->getAllConstants();
    std::vector<std::string> allConstants;
    for (auto& x : constantsList) {
      allConstants.push_back(x);
    }

    if (allConstants.empty()) {
      return false;
    }
    storeSelectResultFromPkb(allConstants);
  }

  return true;
}

bool QueryEvaluator::getRelationResultFromPkb(Relation t_relation) {
  std::unordered_map<std::string, std::vector<std::string>> result;
  std::unordered_map<int, Grammar::GType> typeOfStmts = m_pkb->getTypeOfStatementTable();
  Evaluator *eval = Relationship::createEvaluator(t_relation.getType());
  Grammar g1 = t_relation.getG1();
  Grammar g2 = t_relation.getG2();

  if (g1.getType() == Grammar::GType::STR && g1.getName() == "_" && g2.getType() == Grammar::GType::STR && g2.getName() == "_") {
    return eval->hasRelationship(m_pkb, g1, g2); //Only underscores
  } else if ((g1.getType() == Grammar::GType::STMT_NO || g1.getType() == Grammar::GType::STR) && (g2.getType() == Grammar::GType::STMT_NO || g2.getType() == Grammar::GType::STR)) {
    return eval->isRelationTrue(m_pkb, g1, g2); //No synonyms
  } else if ((g1.getType() == Grammar::GType::STMT_NO || g1.getType() == Grammar::GType::STR) && g2.getType() != Grammar::GType::STMT_NO && g2.getType() != Grammar::GType::STR) {
    result = eval->evaluateRightSynonym(m_pkb, g1, g2); //One right synonym
  } else if (g1.getType() != Grammar::GType::STMT_NO && g1.getType() != Grammar::GType::STR && (g2.getType() == Grammar::GType::STMT_NO || g2.getType() == Grammar::GType::STR)) {
    result = eval->evaluateLeftSynonym(m_pkb, g1, g2); //One left synonym
  } else if (g1.getType() != Grammar::GType::STMT_NO && g1.getType() != Grammar::GType::STR && g2.getType() != Grammar::GType::STMT_NO && g2.getType() != Grammar::GType::STR) {
    result = eval->evaluateBothSynonyms(m_pkb, g1, g2); //Two synonyms
  } 

  // Call the PKB API to get the results of the relation clauses
  if (t_relation.getType() == Relation::RType::PARENT) {
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      if (m_pkb->isParent(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        //std::cout << "Parent: True\n";
        return true;
      } else if (!m_pkb->isParent(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        //std::cout << "Parent: False\n";
        return false;
      } else {
        //std::cout << "Exception in QueryEvaluator.cpp Line 93\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      std::vector<int> stmtIntVector = m_pkb->getChildrenOf(std::stoi(t_relation.getG1().getName()));
      if (stmtIntVector.empty()) {
        return false;
      }
      std::vector<std::string> stmtStrVector;
      
      for (auto& x : stmtIntVector) {
        if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
          if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
            stmtStrVector.push_back(std::to_string(x));
          }
        } else if (t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE) {
          if (typeOfStmts[x] == t_relation.getG2().getType()) {
            stmtStrVector.push_back(std::to_string(x));
          }
        }
      }

      result[t_relation.getG2().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      int stmtNo;
      try {
        stmtNo = m_pkb->getParentOf(std::stoi(t_relation.getG2().getName()));
      } catch (const std::invalid_argument& ia) {
        //std::cout << "Invalid Argument Exception - No Results for getParentOf(s2)\n";
        return false;
      }
      std::vector<std::string> stmtVector;
      
      if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
        if (typeOfStmts[stmtNo] == Grammar::GType::STMT || typeOfStmts[stmtNo] == Grammar::GType::ASGN || typeOfStmts[stmtNo] == Grammar::GType::WHILE || typeOfStmts[stmtNo] == Grammar::GType::PROG_LINE) {
          stmtVector.push_back(std::to_string(stmtNo));
        }
      } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
        if (typeOfStmts[stmtNo] == t_relation.getG1().getType()) {
          stmtVector.push_back(std::to_string(stmtNo));
        }
      }

      result[t_relation.getG1().getName()] = stmtVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      //std::cout << "PARENT (s1, s2) WORKS!\n";
      std::unordered_map<int, std::vector<int>> allParents = m_pkb->getAllParents();
      if (allParents.empty()) {
        return false;
      }

      for (auto& x : allParents) {
        std::vector<std::string> stmtVector;
        for (auto& y : x.second) {
          if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
            if (typeOfStmts[y] == Grammar::GType::STMT || typeOfStmts[y] == Grammar::GType::ASGN || typeOfStmts[y] == Grammar::GType::WHILE || typeOfStmts[y] == Grammar::GType::PROG_LINE) {
              stmtVector.push_back(std::to_string(y));
            }
          } else if (t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE) {
            if (typeOfStmts[y] == t_relation.getG2().getType()) {
              stmtVector.push_back(std::to_string(y));
            }
          }
        }

        if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
          if (typeOfStmts[x.first] == Grammar::GType::STMT || typeOfStmts[x.first] == Grammar::GType::ASGN || typeOfStmts[x.first] == Grammar::GType::WHILE || typeOfStmts[x.first] == Grammar::GType::PROG_LINE) {
            result[std::to_string(x.first)] = stmtVector;
          }
        } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
          if (typeOfStmts[x.first] == t_relation.getG1().getType()) {
            result[std::to_string(x.first)] = stmtVector;
          }
        }
      }
    } else if (t_relation.getG1().getName() == "_" && t_relation.getG2().getName() == "_") {
      if (m_pkb->hasParentRelationship()) {
        //std::cout << "Has Parent Relationship!\n";
        return true;
      } else {
        //std::cout << "No Parent Relationship\n";
        return false;
      }
    } else if (t_relation.getG2().getName() == "_") {
      if (t_relation.getG1().getType() == Grammar::GType::STMT_NO) {
        if (m_pkb->isParentOfAnything(std::stoi(t_relation.getG1().getName()))) {
          //std::cout << "Is Parent of Anything!\n";
          return true;
        } else {
          //std::cout << "Is not Parent of Anything!\n";
          return false;
        }
      } else if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
        std::vector<int> stmtIntVector = m_pkb->getParentOfAnything();
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;

        for (auto& x : stmtIntVector) {
          if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
            if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
              stmtStrVector.push_back(std::to_string(x));
            }
          } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
            if (typeOfStmts[x] == t_relation.getG1().getType()) {
              stmtStrVector.push_back(std::to_string(x));
            }
          }
        }

        result[t_relation.getG1().getName()] = stmtStrVector;
      }
    } else if (t_relation.getG1().getName() == "_") {
      if (t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
        if (m_pkb->isChildrenOfAnything(std::stoi(t_relation.getG2().getName()))) {
          //std::cout << "Is Children of Anything!\n";
          return true;
        } else if (!m_pkb->isChildrenOfAnything(std::stoi(t_relation.getG2().getName()))) {
          //std::cout << "Is not Children of Anything!\n";
          return false;
        }
      } else if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
        std::vector<int> stmtIntVector = m_pkb->getChildrenOfAnything();
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;

        for (auto& x : stmtIntVector) {
          if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
            if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
              stmtStrVector.push_back(std::to_string(x));
            }
          } else if (t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE) {
            if (typeOfStmts[x] == t_relation.getG2().getType()) {
              stmtStrVector.push_back(std::to_string(x));
            }
          }
        }

        result[t_relation.getG2().getName()] = stmtStrVector;
      }
    }
  } else if (t_relation.getType() == Relation::RType::PARENT_) {
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      if (m_pkb->isParentStar(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        //std::cout << "Parent*: True\n";
        return true;
      } else if (!m_pkb->isParentStar(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
        //std::cout << "Parent*: False\n";
        return false;
      } else {
        //std::cout << "Exception in QueryEvaluator.cpp Line 93\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      std::vector<int> stmtIntVector = m_pkb->getChildrenStarOf(std::stoi(t_relation.getG1().getName()));
      if (stmtIntVector.empty()) {
        return false;
      }
      std::vector<std::string> stmtStrVector;
      
      for (auto& x : stmtIntVector) {
        if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
          if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
            stmtStrVector.push_back(std::to_string(x));
          }
        } else if (t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE) {
          if (typeOfStmts[x] == t_relation.getG2().getType()) {
            stmtStrVector.push_back(std::to_string(x));
          }
        }
      }

      result[t_relation.getG2().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
      std::vector<int> stmtIntVector = m_pkb->getParentStarOf(std::stoi(t_relation.getG2().getName()));
      if (stmtIntVector.empty()) {
        return false;
      }
      std::vector<std::string> stmtStrVector;
      
      for (auto& x : stmtIntVector) {
        if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
          if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
            stmtStrVector.push_back(std::to_string(x));
          }
        } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
          if (typeOfStmts[x] == t_relation.getG1().getType()) {
            stmtStrVector.push_back(std::to_string(x));
          }
        }
      }

      result[t_relation.getG1().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
      //std::cout << "PARENT* (s1, s2) WORKS!\n";
      std::unordered_map<int, std::vector<int>> allParents = m_pkb->getAllParentsStar();
      if (allParents.empty()) {
        return false;
      }

      for (auto& x : allParents) {
        std::vector<std::string> stmtVector;
        for (auto& y : x.second) {
          if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
            if (typeOfStmts[y] == Grammar::GType::STMT || typeOfStmts[y] == Grammar::GType::ASGN || typeOfStmts[y] == Grammar::GType::WHILE || typeOfStmts[y] == Grammar::GType::PROG_LINE) {
              stmtVector.push_back(std::to_string(y));
            }
          } else if (t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE) {
            if (typeOfStmts[y] == t_relation.getG2().getType()) {
              stmtVector.push_back(std::to_string(y));
            }
          }
        }

        if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
          if (typeOfStmts[x.first] == Grammar::GType::STMT || typeOfStmts[x.first] == Grammar::GType::ASGN || typeOfStmts[x.first] == Grammar::GType::WHILE || typeOfStmts[x.first] == Grammar::GType::PROG_LINE) {
            result[std::to_string(x.first)] = stmtVector;
          }
        } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
          if (typeOfStmts[x.first] == t_relation.getG1().getType()) {
            result[std::to_string(x.first)] = stmtVector;
          }
        }
      }
    } else if (t_relation.getG1().getName() == "_" && t_relation.getG2().getName() == "_") {
      if (m_pkb->hasParentStarRelationship()) {
        //std::cout << "Has Parent* Relationship!\n";
        return true;
      } else {
        //std::cout << "No Parent* Relationship\n";
        return false;
      }
    } else if (t_relation.getG2().getName() == "_") {
      if (t_relation.getG1().getType() == Grammar::GType::STMT_NO) {
        if (m_pkb->isParentOfAnything(std::stoi(t_relation.getG1().getName()))) {
          //std::cout << "Is Parent of Anything!\n";
          return true;
        } else {
          //std::cout << "Is not Parent of Anything!\n";
          return false;
        }
      } else if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
        std::vector<int> stmtIntVector = m_pkb->getParentStarOfAnything();
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;

        for (auto& x : stmtIntVector) {
          if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
            if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
              stmtStrVector.push_back(std::to_string(x));
            }
          } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
            if (typeOfStmts[x] == t_relation.getG1().getType()) {
              stmtStrVector.push_back(std::to_string(x));
            }
          }
        }

        result[t_relation.getG1().getName()] = stmtStrVector;
      }
    } else if (t_relation.getG1().getName() == "_") {
      if (t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
        if (m_pkb->isChildrenOfAnything(std::stoi(t_relation.getG2().getName()))) {
          //std::cout << "Is Children of Anything!\n";
          return true;
        } else {
          //std::cout << "Is not Children of Anything!\n";
          return false;
        }
      } else if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
        std::vector<int> stmtIntVector = m_pkb->getChildrenStarOfAnything();
        if (stmtIntVector.empty()) {
          return false;
        }

        std::vector<std::string> stmtStrVector;

        for (auto& x : stmtIntVector) {
          if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
            if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
              stmtStrVector.push_back(std::to_string(x));
            }
          } else if (t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE) {
            if (typeOfStmts[x] == t_relation.getG2().getType()) {
              stmtStrVector.push_back(std::to_string(x));
            }
          }
        }

        result[t_relation.getG2().getName()] = stmtStrVector;
      }
    }
  } else if (t_relation.getType() == Relation::RType::USES) {
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getName() != "_" && t_relation.getG2().getType() == Grammar::GType::STR) {
      if (m_pkb->isUses(std::stoi(t_relation.getG1().getName()), t_relation.getG2().getName())) {
        //std::cout << "Uses: True\n";
        return true;
      } else if (!m_pkb->isUses(std::stoi(t_relation.getG1().getName()), t_relation.getG2().getName())) {
        //std::cout << "Uses: False\n";
        return false;
      } else {
        //std::cout << "Exception in QueryEvaluator.cpp for isUses(s, v)\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::VAR) {
      //std::cout << "USES (STMT_NO, VAR) WORKS!\n";
      std::vector<std::string> varUsedByStmt = m_pkb->getUses((std::stoi(t_relation.getG1().getName())));
      if (varUsedByStmt.empty()) {
        return false;
      }

      result[t_relation.getG2().getName()] = varUsedByStmt;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getName() != "_" && t_relation.getG2().getType() == Grammar::GType::STR) {
      //std::cout << "USES (STMT, STR) WORKS!\n";
      std::vector<int> stmtIntVector = m_pkb->getStmtUses(t_relation.getG2().getName());
      if (stmtIntVector.empty()) {
        return false;
      }

      std::vector<std::string> stmtStrVector;
      
      for (auto& x : stmtIntVector) {
        if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
          if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
            stmtStrVector.push_back(std::to_string(x));
          }
        } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
          if (typeOfStmts[x] == t_relation.getG1().getType()) {
            stmtStrVector.push_back(std::to_string(x));
          }
        }
      }

      result[t_relation.getG1().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::VAR) {
      //std::cout << "USES (STMT, VAR) WORKS!\n";
      std::unordered_map<std::string, std::vector<int>> stmtsUsedVar = m_pkb->getAllStmtUses();
      for (auto& x : stmtsUsedVar) {
        std::vector<std::string> stmtStrVector;
        if (!x.second.empty()) {
          for (auto& y : x.second) {
            if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
              if (typeOfStmts[y] == Grammar::GType::STMT || typeOfStmts[y] == Grammar::GType::ASGN || typeOfStmts[y] == Grammar::GType::WHILE || typeOfStmts[y] == Grammar::GType::PROG_LINE) {
                stmtStrVector.push_back(std::to_string(y));
              }
            } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
              if (typeOfStmts[y] == t_relation.getG1().getType()) {
                stmtStrVector.push_back(std::to_string(y));
              }
            }
          }
          result[x.first] = stmtStrVector;
        }
      }
    } else if (t_relation.getG2().getName() == "_") {
      if (t_relation.getG1().getType() == Grammar::GType::STMT_NO) {
        if (m_pkb->isUsesAnything(std::stoi(t_relation.getG1().getName()))) {
          //std::cout << "Is Uses Anything!\n";
          return true;
        } else {
          //std::cout << "Does not Uses Anything!\n";
          return false;
        }
      } else if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
        std::vector<int> stmtIntVector = m_pkb->getStmtUsesAnything();
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;

        for (auto& x : stmtIntVector) {
          if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
            if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
              stmtStrVector.push_back(std::to_string(x));
            }
          } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
            if (typeOfStmts[x] == t_relation.getG1().getType()) {
              stmtStrVector.push_back(std::to_string(x));
            }
          }
        }

        result[t_relation.getG1().getName()] = stmtStrVector;
      }
    }
  } else if (t_relation.getType() == Relation::RType::MODIFIES) {
    if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getName() != "_" && t_relation.getG2().getType() == Grammar::GType::STR) {
      if (m_pkb->isModifies(std::stoi(t_relation.getG1().getName()), t_relation.getG2().getName())) {
        //std::cout << "Modifies: True\n";
        return true;
      } else if (!m_pkb->isModifies(std::stoi(t_relation.getG1().getName()), t_relation.getG2().getName())) {
        //std::cout << "Modifies: False\n";
        return false;
      } else {
        //std::cout << "Exception in QueryEvaluator.cpp for Modifies(s, v)\n";
        return false;
      }
    } else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::VAR) {
      //std::cout << "Modifies (STMT_NO, VAR) WORKS!\n";
      std::vector<std::string> varModifiedByStmt = m_pkb->getModifies((std::stoi(t_relation.getG1().getName())));
      if (varModifiedByStmt.empty()) {
        return false;
      }

      result[t_relation.getG2().getName()] = varModifiedByStmt;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getName() != "_" && t_relation.getG2().getType() == Grammar::GType::STR) {
      //std::cout << "Modifies (STMT, STR) WORKS!\n";
      std::vector<int> stmtIntVector = m_pkb->getStmtModifies(t_relation.getG2().getName());
      if (stmtIntVector.empty()) {
        return false;
      }

      std::vector<std::string> stmtStrVector;
      
      for (auto& x : stmtIntVector) {
        if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
          if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
            stmtStrVector.push_back(std::to_string(x));
          }
        } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
          if (typeOfStmts[x] == t_relation.getG1().getType()) {
            stmtStrVector.push_back(std::to_string(x));
          }
        }
      }

      result[t_relation.getG1().getName()] = stmtStrVector;
    } else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::VAR) {
      //std::cout << "Modifies (STMT, VAR) WORKS!\n";
      std::unordered_map<std::string, std::vector<int>> stmtsUsedVar = m_pkb->getAllStmtModifies();
      for (auto& x : stmtsUsedVar) {
        std::vector<std::string> stmtStrVector;
        if (!x.second.empty()) {
          for (auto& y : x.second) {
            if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
              if (typeOfStmts[y] == Grammar::GType::STMT || typeOfStmts[y] == Grammar::GType::ASGN || typeOfStmts[y] == Grammar::GType::WHILE || typeOfStmts[y] == Grammar::GType::PROG_LINE) {
                stmtStrVector.push_back(std::to_string(y));
              }
            } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
              if (typeOfStmts[y] == t_relation.getG1().getType()) {
                stmtStrVector.push_back(std::to_string(y));
              }
            }
          }
          result[x.first] = stmtStrVector;
        }
      }
    } else if (t_relation.getG2().getName() == "_") {
      if (t_relation.getG1().getType() == Grammar::GType::STMT_NO) {
        if (m_pkb->isModifiesAnything(std::stoi(t_relation.getG1().getName()))) {
          //std::cout << "Is Modifies Anything!\n";
          return true;
        } else {
          //std::cout << "Does not Modifies Anything!\n";
          return false;
        }
      } else if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
        std::vector<int> stmtIntVector = m_pkb->getStmtModifiesAnything();
        if (stmtIntVector.empty()) {
          return false;
        }
        std::vector<std::string> stmtStrVector;

        for (auto& x : stmtIntVector) {
          if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
            if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
              stmtStrVector.push_back(std::to_string(x));
            }
          } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
            if (typeOfStmts[x] == t_relation.getG1().getType()) {
              stmtStrVector.push_back(std::to_string(x));
            }
          }
        }

        result[t_relation.getG1().getName()] = stmtStrVector;
      }
    }
  } else {
    //std::cout << "Relation Type: " + t_relation.getType() << "\n";
  }

  if (result.empty()) {
    return false;
  }

  // Store the result
  std::unordered_map<std::string, int>::const_iterator got;
  if ((t_relation.getG1().getType() != Grammar::GType::STMT_NO || t_relation.getG1().getType() != Grammar::GType::STR) && (t_relation.getG2().getType() == Grammar::GType::STMT_NO || t_relation.getG2().getType() == Grammar::GType::STR)) {
    got = m_synonymsUsedInQuery.find(t_relation.getG1().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::RELATION);
        m_relations.push(t_relation);
      }
    }
  } else if ((t_relation.getG1().getType() == Grammar::GType::STMT_NO || t_relation.getG1().getType() == Grammar::GType::STR) && t_relation.getG2().getType() != Grammar::GType::STMT_NO && t_relation.getG2().getType() != Grammar::GType::STR) {
    got = m_synonymsUsedInQuery.find(t_relation.getG2().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::RELATION);
        m_relations.push(t_relation);
      }
    }
  } else if (t_relation.getG1().getType() != Grammar::GType::STMT_NO && t_relation.getG1().getType() != Grammar::GType::STR && t_relation.getG2().getType() != Grammar::GType::STMT_NO && t_relation.getG2().getType() != Grammar::GType::STR) {
    got = m_synonymsUsedInQuery.find(t_relation.getG1().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::RELATION);
        m_relations.push(t_relation);
      } else {
        got = m_synonymsUsedInQuery.find(t_relation.getG2().getName());
        if (got != m_synonymsUsedInQuery.end()) {
          if (got->second > 1) {
            storeResultFromPkb(result, queryType::RELATION);
            m_relations.push(t_relation);
          }
        }
      }
    }
  }

  return true;
}

bool QueryEvaluator::getPatternResultFromPkb(Pattern t_pattern) {
  std::unordered_map<std::string, std::vector<std::string>> result;
  if (t_pattern.getStmt().getType() == Grammar::GType::ASGN) {
    if (t_pattern.getLeft().getName() != "_" && t_pattern.getRight().getName() != "_") {
      if (t_pattern.getLeft().getType() == Grammar::GType::VAR) {
        std::unordered_map<int, std::string> assignStmtsWithVar = m_pkb->getAllAssignStmtAndVarByPattern(t_pattern.getRight().getName(), !t_pattern.isSubtree());
        if (assignStmtsWithVar.empty()) {
          return false;
        }

        for (auto& x : assignStmtsWithVar) {
          std::vector<std::string> varVector;
          varVector.push_back(x.second);
          result[std::to_string(x.first)] = varVector;
        }
      } else {
        std::list<int> assignStmts = m_pkb->getAssignStmtByVarPattern(t_pattern.getLeft().getName(), t_pattern.getRight().getName(), !t_pattern.isSubtree());
        if (assignStmts.empty()) {
          return false;
        }

        std::vector<std::string> stmtVector;
        for (auto& x : assignStmts) {
          stmtVector.push_back(std::to_string(x));  
        }
        result["a"] = stmtVector;
      }
    } else if (t_pattern.getLeft().getName() == "_" && t_pattern.getRight().getName() == "_") {
      std::list<int> allAssignStmts = m_pkb->getAllAssignStmtList();
      if (allAssignStmts.empty()) {
        return false;
      }

      std::vector<std::string> stmtVector;
      for (auto& x : allAssignStmts) {
        stmtVector.push_back(std::to_string(x));
      }
      result["a"] = stmtVector;
    } else if (t_pattern.getRight().getName() == "_") {
      if (t_pattern.getLeft().getType() == Grammar::GType::VAR) {
        std::unordered_map<int, std::string> assignStmtsWithVar = m_pkb->getAllAssignStmtWithVarName();
        if (assignStmtsWithVar.empty()) {
          return false;
        }

        for (auto& x : assignStmtsWithVar) {
          std::vector<std::string> varVector;
          varVector.push_back(x.second);
          result[std::to_string(x.first)] = varVector;
        }      
      } else {
        std::list<int> assignStmts = m_pkb->getAllAssignStmtListByVar(t_pattern.getLeft().getName());
        if (assignStmts.empty()) {
          return false;
        }

        std::vector<std::string> stmtVector;
        for (auto& x : assignStmts) {
          stmtVector.push_back(std::to_string(x));
        }
        result["a"] = stmtVector;
      }
    } else if (t_pattern.getLeft().getName() == "_") {
      std::list<int> assignStmts;
      if (!t_pattern.isSubtree()) {
        assignStmts = m_pkb->getAllAssignStmtByExactPattern(t_pattern.getRight().getName());
      } else {
        assignStmts = m_pkb->getAllAssignStmtBySubtreePattern(t_pattern.getRight().getName());
      }
      
      if (assignStmts.empty()) {
        return false;
      }

      std::vector<std::string> stmtVector;
      for (auto& x : assignStmts) {
        //std::cout << "X: x\n";
        stmtVector.push_back(std::to_string(x));
      }
      result["a"] = stmtVector;
    }
  }

  // Store the result
  std::unordered_map<std::string, int>::const_iterator got;
  if (t_pattern.getLeft().getType() != Grammar::GType::VAR) {
    got = m_synonymsUsedInQuery.find(t_pattern.getStmt().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::PATTERN);
        m_patterns.push(t_pattern);
      }
    }
  } else if (t_pattern.getLeft().getType() == Grammar::GType::VAR) {
    got = m_synonymsUsedInQuery.find(t_pattern.getStmt().getName());
    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second > 1) {
        storeResultFromPkb(result, queryType::PATTERN);
        m_patterns.push(t_pattern);
      } else if (got->second == 1) {
        got = m_synonymsUsedInQuery.find(t_pattern.getLeft().getName());
        if (got != m_synonymsUsedInQuery.end()) {
          if (got->second > 1) {
            storeResultFromPkb(result, queryType::PATTERN);
            m_patterns.push(t_pattern);
          }
        }
      }
    }
  }

  return true;
}

/**
* A function that gets the result of the clauses by calling the API from PKB.
* @return true if there are results otherwise false
*/
bool QueryEvaluator::getResultFromPkb() {
  //printDivider();
  //std::cout << "Getting results from PKB...\n";
  int selectSize = m_selects.size();
  int relationSize = m_relations.size();
  int patternSize = m_patterns.size();

  //std::cout << "\nSelect Queue";
  for (int i = 0; i < selectSize; ++i) {
    Grammar grammar = m_selects.front();
    m_selectedSynonym = grammar.getName();
    m_selectedType = grammar.getType();
    //std::cout << "\n" << i + 1 << ": " << grammar.getName() << "\n";
    
    // Get Select Results from PKB.
    bool hasResult = getSelectResultFromPkb(grammar);

    // Check if there are results else return false.
    if (!hasResult) {
      return false;
    }

    // Print Select Results Queue.
    //printSelectResultQueue();

    // Move the item to the back of the queue.
    m_selects.pop();
    m_selects.push(grammar);
  }

  //std::cout << "\nRelation Queue";
  for (int i = 0; i < relationSize; ++i) {
    Relation relation = m_relations.front();
    /*std::cout << "\n" << i + 1 << ": " << relation.getType() << " ";
    std::cout << relation.getG1().getName() << " ";
    std::cout << relation.getG2().getName() << "\n";*/

    bool hasResult = getRelationResultFromPkb(relation);
    if (!hasResult) {
      return false;
    }

    //printRelationResultQueue();
    m_relations.pop();
  }

  //std::cout << "\nPattern Queue";
  for (int i = 0; i < patternSize; ++i) {
    Pattern pattern = m_patterns.front();
    /*std::cout << "\n" << i + 1 << ": " << pattern.getStmt().getName() << " ";
    std::cout << pattern.getLeft().getName() << " ";
    std::cout << pattern.getRight().getName() << " ";
    std::cout << pattern.isSubtree() << "\n";*/
    
    bool hasResult = getPatternResultFromPkb(pattern);
    if (!hasResult) {
      return false;
    }

    //printPatternResultQueue();
    m_patterns.pop();
  }

  return true;
}

/**
* A function that stores the result in a data structure.
* @param t_result a vector<string> argument
*/
void QueryEvaluator::storeSelectResultFromPkb(std::vector<std::string> t_result) {
  //printDivider();
  //std::cout << "Storing the select result from PKB to the select result queue...\n";
  m_selectResults.push(t_result);
  //printDivider();
}

/**
* A function that stores the result in a data structure.
* @param t_result an unordered_map<string, vector<string>> argument
* @param t_type an enum queryType argument
*/
void QueryEvaluator::storeResultFromPkb(std::unordered_map<std::string, std::vector<std::string>> t_result, queryType::clauseType t_type) {
  //printDivider();
  //std::cout << "Storing the result from PKB to different queues...\n";

  if (t_type == queryType::RELATION) {
    m_relationResults.push(t_result);
  } else if (t_type == queryType::PATTERN) {
    m_patternResults.push(t_result);
  } else {
    //std::cout << "Result Type: " << t_type << "\n";
  }
  //printDivider();
}

/**
* A function that evaluates the final result of the query by comparing the results based on what the query wants.
* @return The query results
*/
std::vector<std::string> QueryEvaluator::evaluateFinalResult() {
  //printDivider();
  //std::cout << "Evaluating the final result...\n";
  std::vector<std::string> finalResult;
  std::unordered_map<int, Grammar::GType> typeOfStmts = m_pkb->getTypeOfStatementTable();

  if (m_relationResults.empty() && m_patternResults.empty()) {
    //std::cout << "CASE 1\n";
    if (!m_selectResults.empty()) {
      finalResult = m_selectResults.front();
    } 
  } else if (!m_relationResults.empty() && m_patternResults.empty()) {
    //std::cout << "CASE 2\n";
    if ((m_relations.front().getG1().getType() == Grammar::GType::STMT_NO || m_relations.front().getG1().getType() == Grammar::GType::STR) && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
      //std::cout << "STMT_NO/_ SYNONYM\n";
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          if (m_selectedType == Grammar::GType::STMT) {
            if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
              finalResult.push_back(y);
            }
          } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
            if (typeOfStmts[stoi(y)] == m_selectedType) {
              finalResult.push_back(y);
            }
          } else {
            finalResult.push_back(y);
          }
        }
      }
    } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && m_relations.front().getG1().getType() != Grammar::GType::STR && (m_relations.front().getG2().getType() == Grammar::GType::STMT_NO || m_relations.front().getG2().getType() == Grammar::GType::STR)) {
      //std::cout << "SYNONYM STMT_NO/STR\n";
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          if (m_selectedType == Grammar::GType::STMT) {
            if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
              finalResult.push_back(y);
            }
          } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
            if (typeOfStmts[stoi(y)] == m_selectedType) {
              finalResult.push_back(y);
            }
          } else {
            finalResult.push_back(y);
          }
        }
      }
    } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && m_relations.front().getG1().getType() != Grammar::GType::STR && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
      //std::cout << "SYNONYM SYNONYM\n";
      std::unordered_map<std::string, std::vector<std::string>> results = m_relationResults.front();
      
      if (m_relations.front().getG1().getName() == m_selectedSynonym) {
        //std::cout << "Selected Synonym 1: " << m_relations.front().getG1().getName() << "\n";
        if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
          for (auto& x : results) {
            for (auto& y : x.second) {
              if (m_selectedType == Grammar::GType::STMT) {
                if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
                  finalResult.push_back(y);
                }
              } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
                if (typeOfStmts[stoi(y)] == m_selectedType) {
                  finalResult.push_back(y);
                }
              } else {
                finalResult.push_back(y);
              }
            }
          }
        } else {
          for (auto& x : results) {
            if (!x.second.empty()) {
              if (m_selectedType == Grammar::GType::STMT) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType || typeOfStmts[stoi(x.first)] == Grammar::GType::ASGN || typeOfStmts[stoi(x.first)] == Grammar::GType::WHILE) {
                  finalResult.push_back(x.first);
                }
              } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType) {
                  finalResult.push_back(x.first);
                }
              } else {
                finalResult.push_back(x.first);
              }
            }
          }
        }   
      } else if (m_relations.front().getG2().getName() == m_selectedSynonym) {
        //std::cout << "Selected Synonym 2: " << m_relations.front().getG2().getName() << "\n";
        if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
          for (auto& x : results) {
            if (!x.second.empty()) {
              if (m_selectedType == Grammar::GType::STMT) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType || typeOfStmts[stoi(x.first)] == Grammar::GType::ASGN || typeOfStmts[stoi(x.first)] == Grammar::GType::WHILE) {
                  finalResult.push_back(x.first);
                }
              } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
                if (typeOfStmts[stoi(x.first)] == m_selectedType) {
                  finalResult.push_back(x.first);
                }
              } else {
                finalResult.push_back(x.first);
              }
            }
          }
        } else {
          for (auto& x : results) {
            for (auto& y : x.second) {
              if (m_selectedType == Grammar::GType::STMT) {
                if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
                  finalResult.push_back(y);
                }
              } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
                if (typeOfStmts[stoi(y)] == m_selectedType) {
                  finalResult.push_back(y);
                }
              } else {
                finalResult.push_back(y);
              }
            }
          }
        }   
      }
    }

    m_relations.pop();
    m_relationResults.pop();
  } else if (m_relationResults.empty() && !m_patternResults.empty()) {
    //std::cout << "CASE 3\n";
    if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
      std::unordered_map<std::string, std::vector<std::string>> results = m_patternResults.front();
      for (auto& x : results) {
        for (auto& y : x.second) {
          if (m_selectedType == Grammar::GType::STMT) {
            if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
              finalResult.push_back(y);
            }
          } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
            if (typeOfStmts[stoi(y)] == m_selectedType) {
              finalResult.push_back(y);
            }
          } else {
            finalResult.push_back(y);
          }
        }
      }
    } else if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
      if (m_patterns.front().getLeft().getName() == m_selectedSynonym) {
        std::unordered_map<std::string, std::vector<std::string>> results = m_patternResults.front();
        for (auto& x : results) {
          for (auto& y : x.second) {
            if (m_selectedType == Grammar::GType::STMT) {
              if (typeOfStmts[stoi(y)] == m_selectedType || typeOfStmts[stoi(y)] == Grammar::GType::ASGN || typeOfStmts[stoi(y)] == Grammar::GType::WHILE) {
                finalResult.push_back(y);
              }
            } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
              if (typeOfStmts[stoi(y)] == m_selectedType) {
                finalResult.push_back(y);
              }
            } else {
              finalResult.push_back(y);
            }
          }
        }
      } else if (m_patterns.front().getStmt().getName() == m_selectedSynonym) {
        std::unordered_map<std::string, std::vector<std::string>> results = m_patternResults.front();
        for (auto& x : results) {
          if (m_selectedType == Grammar::GType::STMT) {
            if (typeOfStmts[stoi(x.first)] == m_selectedType || typeOfStmts[stoi(x.first)] == Grammar::GType::ASGN || typeOfStmts[stoi(x.first)] == Grammar::GType::WHILE) {
              finalResult.push_back(x.first);
            }
          } else if (m_selectedType == Grammar::GType::ASGN || m_selectedType == Grammar::GType::WHILE) {
            if (typeOfStmts[stoi(x.first)] == m_selectedType) {
              finalResult.push_back(x.first);
            }
          } else {
            finalResult.push_back(x.first);
          }
        }
      }
    }

    m_patterns.pop();
    m_patternResults.pop();
  } else if (!m_relationResults.empty() && !m_patternResults.empty()) {
    std::unordered_map<std::string, int>::const_iterator got;
    got = m_synonymsUsedInQuery.find(m_selectedSynonym);

    if (got != m_synonymsUsedInQuery.end()) {
      if (got->second == 1) { // Common Synonym between Relation and Pattern Clause
        //std::cout << "CASE 4\n";
        std::vector<std::string> commonResults;
        if ((m_relations.front().getG1().getType() == Grammar::GType::STMT_NO || m_relations.front().getG1().getType() == Grammar::GType::STR) && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
          if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;
            
            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            } 

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = m_selectResults.front();
            }      
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
                if (!commonResults.empty()) {
                  finalResult = m_selectResults.front();
                }
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && m_relations.front().getG1().getType() != Grammar::GType::STR && (m_relations.front().getG2().getType() == Grammar::GType::STMT_NO || m_relations.front().getG2().getType() == Grammar::GType::STR)) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = m_selectResults.front();
            } 
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG1().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
                if (!commonResults.empty()) {
                  finalResult = m_selectResults.front();
                }
              } 
            }
          }
        } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && m_relations.front().getG1().getType() != Grammar::GType::STR && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }          
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = m_selectResults.front();
            }
          } else if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.push_back(x.first);      
              }
            } else {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = m_selectResults.front();
            }
          } else if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
            if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
              std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
              std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
              std::vector<std::string> result1;
              std::vector<std::string> result2;

              if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
                for (auto& x : relationResults) {
                  result1.push_back(x.first);
                }
              } else {
                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }
              }

              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }

              commonResults = getCommonResults(result1, result2);
              if (!commonResults.empty()) {
                finalResult = m_selectResults.front();
              }
            }
          }
        }
      } else if (got->second == 2) { // Common Synonym between Select + Relation or Select + Pattern and Relation + Pattern Clause
        //std::cout << "CASE 5\n";
        std::vector<std::string> commonResults;
        if ((m_relations.front().getG1().getType() == Grammar::GType::STMT_NO || m_relations.front().getG1().getType() == Grammar::GType::STR) && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
          if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && m_relations.front().getG1().getType() != Grammar::GType::STR && (m_relations.front().getG2().getType() == Grammar::GType::STMT_NO || m_relations.front().getG2().getType() == Grammar::GType::STR)) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG1().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && m_relations.front().getG1().getType() != Grammar::GType::STR && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
          } else if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            } else {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
          } else if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
            if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
              std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
              std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
              std::vector<std::string> result1;
              std::vector<std::string> result2;

              if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
                for (auto& x : relationResults) {
                  result1.push_back(x.first);
                }
              } else {
                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }
              }

              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }

              commonResults = getCommonResults(result1, result2);
            }
          }
        }

        if (!commonResults.empty()) {
          if (m_selectedSynonym == m_relations.front().getG1().getName() && m_relations.front().getG1().getType() == m_selectedType) {
            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : m_relationResults.front()) {
                if (find(commonResults.begin(), commonResults.end(), x.first) != commonResults.end()) {
                  finalResult.insert(finalResult.end(), x.second.begin(), x.second.end());
                }
              }
            } else {
              for (auto& x : m_relationResults.front()) {
                if (!getCommonResults(x.second, commonResults).empty()) {
                  finalResult.push_back(x.first);
                }
              }
            }
          } else if (m_selectedSynonym == m_relations.front().getG2().getName() && m_relations.front().getG2().getType() == m_selectedType) {
            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : m_relationResults.front()) {
                if (!getCommonResults(x.second, commonResults).empty()) {
                  finalResult.push_back(x.first);
                }
              }
            } else {
              for (auto& x : m_relationResults.front()) {
                if (find(commonResults.begin(), commonResults.end(), x.first) != commonResults.end()) {
                  finalResult.insert(finalResult.end(), x.second.begin(), x.second.end());
                }
              }
            }
          } else if (m_selectedSynonym == m_patterns.front().getStmt().getName() && m_patterns.front().getStmt().getType() == m_selectedType) {
            for (auto& x : m_patternResults.front()) {
              if (!getCommonResults(x.second, commonResults).empty()) {
                finalResult.push_back(x.first);
              }
            }
          } else if (m_selectedSynonym == m_patterns.front().getLeft().getName() && m_patterns.front().getLeft().getType() == m_selectedType) {
            for (auto& x : m_patternResults.front()) {
              if (find(commonResults.begin(), commonResults.end(), x.first) != commonResults.end()) {
                finalResult.insert(finalResult.end(), x.second.begin(), x.second.end());
              }
            }
          }
        }
      } else if (got->second > 2) { // Common Synonym between Select, Relation and Pattern Clause
        //std::cout << "CASE 6\n";
        std::vector<std::string> commonResults;
        if ((m_relations.front().getG1().getType() == Grammar::GType::STMT_NO || m_relations.front().getG1().getType() == Grammar::GType::STR) && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
          if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = commonResults;
            }
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
                if (!commonResults.empty()) {
                  finalResult = commonResults;
                }
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && m_relations.front().getG1().getType() != Grammar::GType::STR && (m_relations.front().getG2().getType() == Grammar::GType::STMT_NO || m_relations.front().getG2().getType() == Grammar::GType::STR)) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            for (auto& x : relationResults) {
              result1.insert(result1.end(), x.second.begin(), x.second.end());
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = commonResults;
            }
          } else {
            if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
              if (m_relations.front().getG1().getName() == m_patterns.front().getLeft().getName()) {
                std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
                std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
                std::vector<std::string> result1;
                std::vector<std::string> result2;

                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }

                for (auto& x : patternResults) {
                  result2.insert(result2.end(), x.second.begin(), x.second.end());
                }

                commonResults = getCommonResults(result1, result2);
                if (!commonResults.empty()) {
                  finalResult = commonResults;
                }
              }
            }
          }
        } else if (m_relations.front().getG1().getType() != Grammar::GType::STMT_NO && m_relations.front().getG1().getType() != Grammar::GType::STR && m_relations.front().getG2().getType() != Grammar::GType::STMT_NO && m_relations.front().getG2().getType() != Grammar::GType::STR) {
          if (m_relations.front().getG1().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = commonResults;
            }
          } else if (m_relations.front().getG2().getName() == m_patterns.front().getStmt().getName()) {
            std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
            std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
            std::vector<std::string> result1;
            std::vector<std::string> result2;

            if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
              for (auto& x : relationResults) {
                result1.push_back(x.first);
              }
            } else {
              for (auto& x : relationResults) {
                result1.insert(result1.end(), x.second.begin(), x.second.end());
              }
            }

            if (m_patterns.front().getLeft().getType() != Grammar::GType::VAR) {
              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }
            } else {
              for (auto& x : patternResults) {
                result2.push_back(x.first);
              }
            }

            commonResults = getCommonResults(result1, result2);
            if (!commonResults.empty()) {
              finalResult = commonResults;
            }
          } else if (m_patterns.front().getLeft().getType() == Grammar::GType::VAR) {
            if (m_relations.front().getG2().getName() == m_patterns.front().getLeft().getName()) {
              std::unordered_map<std::string, std::vector<std::string>> relationResults = m_relationResults.front();
              std::unordered_map<std::string, std::vector<std::string>> patternResults = m_patternResults.front();
              std::vector<std::string> result1;
              std::vector<std::string> result2;

              if (m_relations.front().getType() == Relation::RType::USES || m_relations.front().getType() == Relation::RType::MODIFIES) {
                for (auto& x : relationResults) {
                  result1.push_back(x.first);
                }
              } else {
                for (auto& x : relationResults) {
                  result1.insert(result1.end(), x.second.begin(), x.second.end());
                }
              }

              for (auto& x : patternResults) {
                result2.insert(result2.end(), x.second.begin(), x.second.end());
              }

              commonResults = getCommonResults(result1, result2);
              if (!commonResults.empty()) {
                finalResult = commonResults;
              }
            }
          }
        }
      }
    }

    m_relations.pop();
    m_patterns.pop();
    m_relationResults.pop();
    m_patternResults.pop();
  }

  /*std::cout << "Query Result: \n";
  for (std::vector<std::string>::iterator getStmts = finalResult.begin(); getStmts != finalResult.end(); ++getStmts) {
    std::cout << *getStmts << ", ";
  }*/

  //printDivider();
  return finalResult;
}