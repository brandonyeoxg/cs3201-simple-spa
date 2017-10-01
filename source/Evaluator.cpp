#pragma once

#include "Evaluator.h"

std::vector<std::string> Evaluator::filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar) {
  std::vector<std::string> stmtVector;

  if (t_grammar.getType() == queryType::GType::STMT || t_grammar.getType() == queryType::GType::PROG_LINE) {
    if (t_typeOfStmts[t_stmtNo] == queryType::GType::STMT || t_typeOfStmts[t_stmtNo] == queryType::GType::ASGN || t_typeOfStmts[t_stmtNo] == queryType::GType::WHILE || t_typeOfStmts[t_stmtNo] == queryType::GType::PROG_LINE) {
      stmtVector.push_back(std::to_string(t_stmtNo));
    }
  } else if (t_grammar.getType() == queryType::GType::ASGN || t_grammar.getType() == queryType::GType::WHILE) {
    if (t_typeOfStmts[t_stmtNo] == t_grammar.getType()) {
      stmtVector.push_back(std::to_string(t_stmtNo));
    }
  }

  return stmtVector;
}

std::vector<std::string> Evaluator::filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::vector<int> t_stmtIntVector, Grammar t_grammar) {
  std::vector<std::string> stmtVector;

  for (auto& stmtNo : t_stmtIntVector) {
    if (t_grammar.getType() == queryType::GType::STMT || t_grammar.getType() == queryType::GType::PROG_LINE) {
      if (t_typeOfStmts[stmtNo] == queryType::GType::STMT || t_typeOfStmts[stmtNo] == queryType::GType::ASGN || t_typeOfStmts[stmtNo] == queryType::GType::WHILE || t_typeOfStmts[stmtNo] == queryType::GType::PROG_LINE) {
        stmtVector.push_back(std::to_string(stmtNo));
      }
    } else if (t_grammar.getType() == queryType::GType::ASGN || t_grammar.getType() == queryType::GType::WHILE) {
      if (t_typeOfStmts[stmtNo] == t_grammar.getType()) {
        stmtVector.push_back(std::to_string(stmtNo));
      }
    }
  }

  return stmtVector;
}

std::vector<std::string> Evaluator::filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar, std::vector<std::string> t_stmtVector) {
  std::vector<std::string> result;

  if (t_grammar.getType() == queryType::GType::STMT || t_grammar.getType() == queryType::GType::PROG_LINE) {
    if (t_typeOfStmts[t_stmtNo] == queryType::GType::STMT || t_typeOfStmts[t_stmtNo] == queryType::GType::ASGN || t_typeOfStmts[t_stmtNo] == queryType::GType::WHILE || t_typeOfStmts[t_stmtNo] == queryType::GType::PROG_LINE) {
      result = t_stmtVector;
    }
  } else if (t_grammar.getType() == queryType::GType::ASGN || t_grammar.getType() == queryType::GType::WHILE) {
    if (t_typeOfStmts[t_stmtNo] == t_grammar.getType()) {
      result = t_stmtVector;
    }
  }

  return result;
}