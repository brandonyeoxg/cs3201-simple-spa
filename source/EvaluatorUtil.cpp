#include "EvaluatorUtil.h"

std::vector<std::string> EvaluatorUtil::filterKeyResults(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::unordered_map<std::string, std::vector<std::string>> t_results, queryType::GType t_selectedType) {
  std::vector<std::string> stmtVector;

  for (auto& x : t_results) {
    if (!x.second.empty()) {
      if (Grammar::isStmt(t_selectedType) || Grammar::isProgLine(t_selectedType)) {
        if (t_typeOfStmts[std::stoi(x.first)] == t_selectedType || Grammar::isAssign(t_typeOfStmts[std::stoi(x.first)]) || Grammar::isWhile(t_typeOfStmts[std::stoi(x.first)]) || Grammar::isIf(t_typeOfStmts[std::stoi(x.first)]) || Grammar::isCall(t_typeOfStmts[std::stoi(x.first)])) {
          stmtVector.push_back(x.first);
        }
      } else if (Grammar::isAssign(t_selectedType) || Grammar::isWhile(t_selectedType) || Grammar::isIf(t_selectedType) || Grammar::isCall(t_selectedType)) {
        if (t_typeOfStmts[std::stoi(x.first)] == t_selectedType) {
          stmtVector.push_back(x.first);
        }
      } else {
        stmtVector.push_back(x.first);
      }
    }
  }

  return stmtVector;
}

std::vector<std::string> EvaluatorUtil::filterValueResults(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::unordered_map <std::string, std::vector<std::string>> t_results, queryType::GType t_selectedType) {
  std::vector<std::string> stmtVector;

  for (auto& x : t_results) {
    for (auto& stmtNo : x.second) {
      if (Grammar::isStmt(t_selectedType) || Grammar::isProgLine(t_selectedType)) {
        if (t_typeOfStmts[std::stoi(stmtNo)] == t_selectedType || Grammar::isAssign(t_typeOfStmts[std::stoi(stmtNo)]) || Grammar::isWhile(t_typeOfStmts[std::stoi(stmtNo)]) || Grammar::isIf(t_typeOfStmts[std::stoi(stmtNo)]) || Grammar::isCall(t_typeOfStmts[std::stoi(stmtNo)])) {
          stmtVector.push_back(stmtNo);
        }
      } else if (Grammar::isAssign(t_selectedType) || Grammar::isWhile(t_selectedType) || Grammar::isIf(t_selectedType) || Grammar::isCall(t_selectedType)) {
        if (t_typeOfStmts[std::stoi(stmtNo)] == t_selectedType) {
          stmtVector.push_back(stmtNo);
        }
      } else {
        stmtVector.push_back(stmtNo);
      }
    }
  }

  return stmtVector;
}

std::vector<std::string> EvaluatorUtil::filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar) {
  std::vector<std::string> stmtVector;

  if (t_grammar.getType() == queryType::GType::STMT || t_grammar.getType() == queryType::GType::PROG_LINE) {
    if (t_typeOfStmts[t_stmtNo] == queryType::GType::STMT || t_typeOfStmts[t_stmtNo] == queryType::GType::PROG_LINE || t_typeOfStmts[t_stmtNo] == queryType::GType::ASGN || t_typeOfStmts[t_stmtNo] == queryType::GType::WHILE || t_typeOfStmts[t_stmtNo] == queryType::GType::IF || t_typeOfStmts[t_stmtNo] == queryType::GType::CALL) {
      stmtVector.push_back(std::to_string(t_stmtNo));
    }
  } else if (t_grammar.getType() == queryType::GType::ASGN || t_grammar.getType() == queryType::GType::WHILE || t_grammar.getType() == queryType::GType::IF || t_grammar.getType() == queryType::GType::CALL) {
    if (t_typeOfStmts[t_stmtNo] == t_grammar.getType()) {
      stmtVector.push_back(std::to_string(t_stmtNo));
    }
  }

  return stmtVector;
}

std::vector<std::string> EvaluatorUtil::filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::vector<int> t_stmtIntVector, Grammar t_grammar) {
  std::vector<std::string> stmtVector;

  for (auto& stmtNo : t_stmtIntVector) {
    if (t_grammar.getType() == queryType::GType::STMT || t_grammar.getType() == queryType::GType::PROG_LINE) {
      if (t_typeOfStmts[stmtNo] == queryType::GType::STMT || t_typeOfStmts[stmtNo] == queryType::GType::PROG_LINE || t_typeOfStmts[stmtNo] == queryType::GType::ASGN || t_typeOfStmts[stmtNo] == queryType::GType::WHILE || t_typeOfStmts[stmtNo] == queryType::GType::IF || t_typeOfStmts[stmtNo] == queryType::GType::CALL) {
        stmtVector.push_back(std::to_string(stmtNo));
      }
    } else if (t_grammar.getType() == queryType::GType::ASGN || t_grammar.getType() == queryType::GType::WHILE || t_grammar.getType() == queryType::GType::IF || t_grammar.getType() == queryType::GType::CALL) {
      if (t_typeOfStmts[stmtNo] == t_grammar.getType()) {
        stmtVector.push_back(std::to_string(stmtNo));
      }
    }
  }

  return stmtVector;
}

std::vector<std::string> EvaluatorUtil::filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar, std::vector<std::string> t_stmtVector) {
  std::vector<std::string> result;

  if (t_grammar.getType() == queryType::GType::STMT || t_grammar.getType() == queryType::GType::PROG_LINE) {
    if (t_typeOfStmts[t_stmtNo] == queryType::GType::STMT || t_typeOfStmts[t_stmtNo] == queryType::GType::PROG_LINE || t_typeOfStmts[t_stmtNo] == queryType::GType::ASGN || t_typeOfStmts[t_stmtNo] == queryType::GType::WHILE || t_typeOfStmts[t_stmtNo] == queryType::GType::IF || t_typeOfStmts[t_stmtNo] == queryType::GType::CALL) {
      result = t_stmtVector;
    }
  } else if (t_grammar.getType() == queryType::GType::ASGN || t_grammar.getType() == queryType::GType::WHILE || t_grammar.getType() == queryType::GType::IF || t_grammar.getType() == queryType::GType::CALL) {
    if (t_typeOfStmts[t_stmtNo] == t_grammar.getType()) {
      result = t_stmtVector;
    }
  }

  return result;
}

std::vector<std::string> EvaluatorUtil::getCommonResults(std::vector<std::string> t_resultVector1, std::vector<std::string> t_resultVector2) {
  std::vector<std::string> commonResultVector;

  sort(t_resultVector1.begin(), t_resultVector1.end());
  sort(t_resultVector2.begin(), t_resultVector2.end());

  set_intersection(t_resultVector1.begin(), t_resultVector1.end(), t_resultVector2.begin(), t_resultVector2.end(), back_inserter(commonResultVector));

  return commonResultVector;
}