#pragma once

#include "FollowsEvaluator.h"

SET_OF_RESULTS FollowsEvaluator::evaluate(Relation t_relation) {
  //if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
  //  if (m_pkb->isFollows(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
  //    //std::cout << "Follows: True\n";
  //    return true;
  //  } else if (!m_pkb->isFollows(std::stoi(t_relation.getG1().getName()), std::stoi(t_relation.getG2().getName()))) {
  //    //std::cout << "Follows: False\n";
  //    return false;
  //  } else {
  //    //std::cout << "Exception in QueryEvaluator.cpp for isFollows(s1, s2)\n";
  //    return false;
  //  }
  //} else if (t_relation.getG1().getType() == Grammar::GType::STMT_NO && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
  //  int stmtNo;
  //  try {
  //    stmtNo = m_pkb->getFollows(std::stoi(t_relation.getG1().getName()));
  //    //std::cout << "getFollows - STMT NO: " << stmtNo << "\n";
  //  } catch (const std::invalid_argument& ia) {
  //    //std::cout << "Invalid Argument Exception - No Results for getFollows(s1)\n";
  //    return false;
  //  }

  //  std::vector<std::string> stmtVector;

  //  if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
  //    if (typeOfStmts[stmtNo] == Grammar::GType::STMT || typeOfStmts[stmtNo] == Grammar::GType::ASGN || typeOfStmts[stmtNo] == Grammar::GType::WHILE || typeOfStmts[stmtNo] == Grammar::GType::PROG_LINE) {
  //      stmtVector.push_back(std::to_string(stmtNo));
  //    }
  //  } else if (t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE) {
  //    if (typeOfStmts[stmtNo] == t_relation.getG2().getType()) {
  //      stmtVector.push_back(std::to_string(stmtNo));
  //    }
  //  }

  //  result[t_relation.getG2().getName()] = stmtVector;
  //} else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
  //  int stmtNo;
  //  try {
  //    stmtNo = m_pkb->getFollowedBy(std::stoi(t_relation.getG2().getName()));
  //    //std::cout << "getFollowedBy - STMT NO: " << stmtNo << "\n";
  //  } catch (const std::invalid_argument& ia) {
  //    //std::cout << "Invalid Argument Exception - No Results for getFollowedBy(s2)\n";
  //    return false;
  //  }

  //  std::vector<std::string> stmtVector;

  //  if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
  //    if (typeOfStmts[stmtNo] == Grammar::GType::STMT || typeOfStmts[stmtNo] == Grammar::GType::ASGN || typeOfStmts[stmtNo] == Grammar::GType::WHILE || typeOfStmts[stmtNo] == Grammar::GType::PROG_LINE) {
  //      stmtVector.push_back(std::to_string(stmtNo));
  //    }
  //  } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
  //    if (typeOfStmts[stmtNo] == t_relation.getG1().getType()) {
  //      stmtVector.push_back(std::to_string(stmtNo));
  //    }
  //  }

  //  result[t_relation.getG1().getName()] = stmtVector;
  //} else if ((t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) && (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE)) {
  //  //std::cout << "FOLLOWS (s1, s2) WORKS!\n";
  //  std::unordered_map<int, int> allFollows = m_pkb->getAllFollows();
  //  if (allFollows.empty()) {
  //    return false;
  //  }

  //  for (auto& x : allFollows) {
  //    std::vector<std::string> stmtVector;
  //    if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
  //      if (typeOfStmts[x.second] == Grammar::GType::STMT || typeOfStmts[x.second] == Grammar::GType::ASGN || typeOfStmts[x.second] == Grammar::GType::WHILE || typeOfStmts[x.second] == Grammar::GType::PROG_LINE) {
  //        stmtVector.push_back(std::to_string(x.second));
  //      }
  //    } else if (t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE) {
  //      if (typeOfStmts[x.second] == t_relation.getG2().getType()) {
  //        stmtVector.push_back(std::to_string(x.second));
  //      }
  //    }

  //    if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
  //      if (typeOfStmts[x.first] == Grammar::GType::STMT || typeOfStmts[x.first] == Grammar::GType::ASGN || typeOfStmts[x.first] == Grammar::GType::WHILE || typeOfStmts[x.first] == Grammar::GType::PROG_LINE) {
  //        result[std::to_string(x.first)] = stmtVector;
  //      }
  //    } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
  //      if (typeOfStmts[x.first] == t_relation.getG1().getType()) {
  //        result[std::to_string(x.first)] = stmtVector;
  //      }
  //    }
  //  }
  //} else if (t_relation.getG1().getName() == "_" && t_relation.getG2().getName() == "_") {
  //  if (m_pkb->hasFollowRelationship()) {
  //    //std::cout << "Has Follows Relationship!\n";
  //    return true;
  //  } else {
  //    //std::cout << "No Follows Relationship\n";
  //    return false;
  //  }
  //} else if (t_relation.getG2().getName() == "_") {
  //  if (t_relation.getG1().getType() == Grammar::GType::STMT_NO) {
  //    if (m_pkb->isFollowedByAnything(std::stoi(t_relation.getG1().getName()))) {
  //      //std::cout << "Followed By Anything!\n";
  //      return true;
  //    } else {
  //      //std::cout << "Does not Follow By Anything!\n";
  //      return false;
  //    }
  //  } else if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
  //    std::vector<int> stmtIntVector = m_pkb->getFollowedByAnything();
  //    if (stmtIntVector.empty()) {
  //      return false;
  //    }

  //    std::vector<std::string> stmtStrVector;

  //    for (auto& x : stmtIntVector) {
  //      if (t_relation.getG1().getType() == Grammar::GType::STMT || t_relation.getG1().getType() == Grammar::GType::PROG_LINE) {
  //        if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
  //          stmtStrVector.push_back(std::to_string(x));
  //        }
  //      } else if (t_relation.getG1().getType() == Grammar::GType::ASGN || t_relation.getG1().getType() == Grammar::GType::WHILE) {
  //        if (typeOfStmts[x] == t_relation.getG1().getType()) {
  //          stmtStrVector.push_back(std::to_string(x));
  //        }
  //      }
  //    }

  //    result[t_relation.getG1().getName()] = stmtStrVector;
  //  }
  //} else if (t_relation.getG1().getName() == "_") {
  //  if (t_relation.getG2().getType() == Grammar::GType::STMT_NO) {
  //    if (m_pkb->isFollowsAnything(std::stoi(t_relation.getG2().getName()))) {
  //      //std::cout << "Follows Anything!\n";
  //      return true;
  //    } else {
  //      //std::cout << "Does not Follow Anything!\n";
  //      return false;
  //    }
  //  } else if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
  //    std::vector<int> stmtIntVector = m_pkb->getFollowsAnything();
  //    if (stmtIntVector.empty()) {
  //      return false;
  //    }

  //    std::vector<std::string> stmtStrVector;

  //    for (auto& x : stmtIntVector) {
  //      if (t_relation.getG2().getType() == Grammar::GType::STMT || t_relation.getG2().getType() == Grammar::GType::PROG_LINE) {
  //        if (typeOfStmts[x] == Grammar::GType::STMT || typeOfStmts[x] == Grammar::GType::ASGN || typeOfStmts[x] == Grammar::GType::WHILE || typeOfStmts[x] == Grammar::GType::PROG_LINE) {
  //          stmtStrVector.push_back(std::to_string(x));
  //        }
  //      } else if (t_relation.getG2().getType() == Grammar::GType::ASGN || t_relation.getG2().getType() == Grammar::GType::WHILE) {
  //        if (typeOfStmts[x] == t_relation.getG2().getType()) {
  //          stmtStrVector.push_back(std::to_string(x));
  //        }
  //      }
  //    }

  //    result[t_relation.getG2().getName()] = stmtStrVector;
  //  }
  //}
}