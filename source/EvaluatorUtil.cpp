#include "EvaluatorUtil.h"

std::vector<std::string> EvaluatorUtil::filterKeyResults(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::unordered_map<std::string, std::vector<std::string>> t_results, queryType::GType t_selectedType) {
  std::vector<std::string> stmtVector;

  for (auto& x : t_results) {
    if (!x.second.empty()) {
      if (Grammar::isStmt(t_selectedType) || Grammar::isProgLine(t_selectedType)) {
        if (Grammar::isSameGType(t_typeOfStmts[std::stoi(x.first)], t_selectedType)
          || Grammar::isAssign(t_typeOfStmts[std::stoi(x.first)]) 
          || Grammar::isWhile(t_typeOfStmts[std::stoi(x.first)]) 
          || Grammar::isIf(t_typeOfStmts[std::stoi(x.first)]) 
          || Grammar::isCall(t_typeOfStmts[std::stoi(x.first)])) {
          stmtVector.push_back(x.first);
        }
      } else if (Grammar::isAssign(t_selectedType) || Grammar::isWhile(t_selectedType) 
        || Grammar::isIf(t_selectedType) || Grammar::isCall(t_selectedType)) {
        if (Grammar::isSameGType(t_typeOfStmts[std::stoi(x.first)], t_selectedType)) {
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
        if (Grammar::isSameGType(t_typeOfStmts[std::stoi(stmtNo)], t_selectedType)
          || Grammar::isAssign(t_typeOfStmts[std::stoi(stmtNo)]) 
          || Grammar::isWhile(t_typeOfStmts[std::stoi(stmtNo)]) 
          || Grammar::isIf(t_typeOfStmts[std::stoi(stmtNo)]) 
          || Grammar::isCall(t_typeOfStmts[std::stoi(stmtNo)])) {
          stmtVector.push_back(stmtNo);
        }
      } else if (Grammar::isAssign(t_selectedType) || Grammar::isWhile(t_selectedType) 
        || Grammar::isIf(t_selectedType) || Grammar::isCall(t_selectedType)) {
        if (Grammar::isSameGType(t_typeOfStmts[std::stoi(stmtNo)], t_selectedType)) {
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

  if (Grammar::isStmt(t_grammar.getType()) || Grammar::isProgLine(t_grammar.getType())) {
    if (Grammar::isStmt(t_typeOfStmts[t_stmtNo]) || Grammar::isProgLine(t_typeOfStmts[t_stmtNo]) 
      || Grammar::isAssign(t_typeOfStmts[t_stmtNo]) || Grammar::isWhile(t_typeOfStmts[t_stmtNo]) 
      || Grammar::isIf(t_typeOfStmts[t_stmtNo]) || Grammar::isCall(t_typeOfStmts[t_stmtNo])) {
      stmtVector.push_back(std::to_string(t_stmtNo));
    }
  } else if (Grammar::isAssign(t_grammar.getType()) || Grammar::isWhile(t_grammar.getType()) 
    || Grammar::isIf(t_grammar.getType()) || Grammar::isCall(t_grammar.getType())) {
    if (Grammar::isSameGType(t_typeOfStmts[t_stmtNo], t_grammar.getType())) {
      stmtVector.push_back(std::to_string(t_stmtNo));
    }
  }

  return stmtVector;
}

std::vector<std::string> EvaluatorUtil::filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, std::vector<int> t_stmtIntVector, Grammar t_grammar) {
  std::vector<std::string> stmtVector;

  for (auto& stmtNo : t_stmtIntVector) {
    if (Grammar::isStmt(t_grammar.getType()) || Grammar::isProgLine(t_grammar.getType())) {
      if (Grammar::isStmt(t_typeOfStmts[stmtNo]) || Grammar::isProgLine(t_typeOfStmts[stmtNo]) 
        || Grammar::isAssign(t_typeOfStmts[stmtNo]) || Grammar::isWhile(t_typeOfStmts[stmtNo]) 
        || Grammar::isIf(t_typeOfStmts[stmtNo]) || Grammar::isCall(t_typeOfStmts[stmtNo])) {
        stmtVector.push_back(std::to_string(stmtNo));
      }
    } else if (Grammar::isAssign(t_grammar.getType()) || Grammar::isWhile(t_grammar.getType()) 
      || Grammar::isIf(t_grammar.getType()) || Grammar::isCall(t_grammar.getType())) {
      if (Grammar::isSameGType(t_typeOfStmts[stmtNo], t_grammar.getType())) {
        stmtVector.push_back(std::to_string(stmtNo));
      }
    }
  }

  return stmtVector;
}

std::vector<std::string> EvaluatorUtil::filterStmts(std::unordered_map<int, queryType::GType> t_typeOfStmts, int t_stmtNo, Grammar t_grammar, std::vector<std::string> t_stmtVector) {
  std::vector<std::string> result;

  if (Grammar::isStmt(t_grammar.getType()) || Grammar::isProgLine(t_grammar.getType())) {
    if (Grammar::isStmt(t_typeOfStmts[t_stmtNo]) || Grammar::isProgLine(t_typeOfStmts[t_stmtNo]) 
      || Grammar::isAssign(t_typeOfStmts[t_stmtNo]) || Grammar::isWhile(t_typeOfStmts[t_stmtNo]) 
      || Grammar::isIf(t_typeOfStmts[t_stmtNo]) || Grammar::isCall(t_typeOfStmts[t_stmtNo])) {
      result = t_stmtVector;
    }
  } else if (Grammar::isAssign(t_grammar.getType()) || Grammar::isWhile(t_grammar.getType()) 
    || Grammar::isIf(t_grammar.getType()) || Grammar::isCall(t_grammar.getType())) {
    if (Grammar::isSameGType(t_typeOfStmts[t_stmtNo], t_grammar.getType())) {
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

SET_OF_RESULTS EvaluatorUtil::getCommonProgLineAndConstant(std::vector<std::string> t_allConstants, int t_maxProgLine) {
  SET_OF_RESULTS commonProgLineAndConstant;

  for (auto& constant : t_allConstants) {
    if (std::stoi(constant) <= t_maxProgLine) {
      LIST_OF_RESULTS constants;
      constants.push_back(constant);
      commonProgLineAndConstant[constant] = constants;
    }
  }
  
  return commonProgLineAndConstant;
}

Grammar EvaluatorUtil::rewriteSynonym(Grammar t_grammar, std::unordered_map<SYNONYM_NAME, Grammar> t_rewriteList) {
  std::unordered_map<SYNONYM_NAME, Grammar>::const_iterator got;
  got = t_rewriteList.find(t_grammar.getName());
  if (got != t_rewriteList.end()) {
    return got->second;
  }

  return t_grammar;
}