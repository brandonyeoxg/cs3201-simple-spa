#pragma once

#include "WithEvaluator.h"

BOOLEAN WithEvaluator::isEquals() {
  if (Grammar::isStmtNo(m_left.getType()) && Grammar::isStmtNo(m_right.getType())) {
    if (m_left.getName() == m_right.getName()) {
      return true;
    } else {
      return false;
    }
  } else if (Grammar::isString(m_left.getType()) && Grammar::isString(m_right.getType())) {
    if (m_left.getName() == m_right.getName()) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_INTEGERS WithEvaluator::evaluateSynWithInt() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_INTEGERS synsToBeRewrittenAsInts;
  if (Grammar::isStmtNo(m_left.getType())) {
    if (Grammar::isConst(m_right.getType())) {
      LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
      std::vector<std::string>::iterator it;
      it = find(allConstants.begin(), allConstants.end(), m_left.getName());
      if (it != allConstants.end()) {
        synsToBeRewrittenAsInts[m_right.getName()] = std::stoi(*it);
      } else {
        return synsToBeRewrittenAsInts;
      }
    } else {
      synsToBeRewrittenAsInts[m_right.getName()] = std::stoi(m_left.getName());
    }  
  } else if (Grammar::isStmtNo(m_right.getType())) {
    if (Grammar::isConst(m_right.getType())) {
      LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
      std::vector<std::string>::iterator it;
      it = find(allConstants.begin(), allConstants.end(), m_left.getName());
      if (it != allConstants.end()) {
        synsToBeRewrittenAsInts[m_right.getName()] = std::stoi(*it);
      } else {
        return synsToBeRewrittenAsInts;
      }
    } else {
      synsToBeRewrittenAsInts[m_left.getName()] = std::stoi(m_right.getName());
    }  
  }

  return synsToBeRewrittenAsInts;
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_STRING WithEvaluator::evaluateSynWithStr() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_STRING synsToBeRewrittenAsStr;
  if (Grammar::isString(m_left.getType())) {
    if (Grammar::isCall(m_right.getType())) {
      LIST_OF_PROC_NAMES allProcsBeingCalled = m_pkb->getCalledByAnything();
      std::vector<std::string>::iterator it;
      it = find(allProcsBeingCalled.begin(), allProcsBeingCalled.end(), m_left.getName());
      if (it != allProcsBeingCalled.end()) {
        synsToBeRewrittenAsStr[m_right.getName()] = m_left.getName();
      } else {
        return synsToBeRewrittenAsStr;
      }
    } else {
      LIST_OF_PROC_NAMES allProcs = m_pkb->getAllProcsName();
      std::vector<std::string>::iterator it;
      it = find(allProcs.begin(), allProcs.end(), m_left.getName());
      if (it != allProcs.end()) {
        synsToBeRewrittenAsStr[m_right.getName()] = m_left.getName();
      } else {
        return synsToBeRewrittenAsStr;
      }
    } 
  } else if (Grammar::isString(m_right.getType())) {
    if (Grammar::isCall(m_left.getType())) {
      LIST_OF_PROC_NAMES allProcsBeingCalled = m_pkb->getCalledByAnything();
      std::vector<std::string>::iterator it;
      it = find(allProcsBeingCalled.begin(), allProcsBeingCalled.end(), m_right.getName());
      if (it != allProcsBeingCalled.end()) {
        synsToBeRewrittenAsStr[m_left.getName()] = m_right.getName();
      } else {
        return synsToBeRewrittenAsStr;
      }
    } else {
      LIST_OF_PROC_NAMES allProcs = m_pkb->getAllProcsName();
      std::vector<std::string>::iterator it;
      it = find(allProcs.begin(), allProcs.end(), m_left.getName());
      if (it != allProcs.end()) {
        synsToBeRewrittenAsStr[m_left.getName()] = m_right.getName();
      } else {
        return synsToBeRewrittenAsStr;
      }
    }
  }

  return synsToBeRewrittenAsStr;
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_SYNONYMS WithEvaluator::evaluateSynWithSyn() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_SYNONYMS synsToBeRewrittenAsSyns;
  if (Grammar::isProgLine(m_left.getType()) && Grammar::isProgLine(m_right.getType())) {
    std::unordered_map<std::string, int>::const_iterator got1;
    std::unordered_map<std::string, int>::const_iterator got2;
    got1 = m_synonymsUsedInQuery.find(m_left.getName());
    got2 = m_synonymsUsedInQuery.find(m_right.getName());
    if (got1 != m_synonymsUsedInQuery.end() && got2 != m_synonymsUsedInQuery.end()) {
      if (got1->second >= got2->second) {
        synsToBeRewrittenAsSyns[m_right.getName()] = m_left.getName();
      } else {
        synsToBeRewrittenAsSyns[m_left.getName()] = m_right.getName();
      }
    } else {
      return synsToBeRewrittenAsSyns;
    }
  } else if (Grammar::isProcName(m_left.getAttr()) && Grammar::isProcName(m_right.getAttr())) {
    std::unordered_map<std::string, int>::const_iterator got1;
    std::unordered_map<std::string, int>::const_iterator got2;
    got1 = m_synonymsUsedInQuery.find(m_left.getName());
    got2 = m_synonymsUsedInQuery.find(m_right.getName());
    if (got1 != m_synonymsUsedInQuery.end() && got2 != m_synonymsUsedInQuery.end()) {
      if (got1->second >= got2->second) {
        synsToBeRewrittenAsSyns[m_right.getName()] = m_left.getName();
      } else {
        synsToBeRewrittenAsSyns[m_left.getName()] = m_right.getName();
      }
    } else {
      return synsToBeRewrittenAsSyns;
    }
  } else if (Grammar::isVarName(m_left.getAttr()) && Grammar::isVarName(m_right.getAttr())) {
    std::unordered_map<std::string, int>::const_iterator got1;
    std::unordered_map<std::string, int>::const_iterator got2;
    got1 = m_synonymsUsedInQuery.find(m_left.getName());
    got2 = m_synonymsUsedInQuery.find(m_right.getName());
    if (got1 != m_synonymsUsedInQuery.end() && got2 != m_synonymsUsedInQuery.end()) {
      if (got1->second >= got2->second) {
        synsToBeRewrittenAsSyns[m_right.getName()] = m_left.getName();
      } else {
        synsToBeRewrittenAsSyns[m_left.getName()] = m_right.getName();
      }
    } else {
      return synsToBeRewrittenAsSyns;
    }
  } else if (Grammar::isStmtNum(m_left.getAttr()) && Grammar::isStmtNum(m_right.getAttr())) {
    if (!Grammar::isStmt(m_left.getType()) && !Grammar::isStmt(m_right.getType())) {
      return synsToBeRewrittenAsSyns;
    } else if (Grammar::isStmt(m_left.getType()) && Grammar::isStmt(m_right.getType())) {
      std::unordered_map<std::string, int>::const_iterator got1;
      std::unordered_map<std::string, int>::const_iterator got2;
      got1 = m_synonymsUsedInQuery.find(m_left.getName());
      got2 = m_synonymsUsedInQuery.find(m_right.getName());
      if (got1 != m_synonymsUsedInQuery.end() && got2 != m_synonymsUsedInQuery.end()) {
        if (got1->second >= got2->second) {
          synsToBeRewrittenAsSyns[m_right.getName()] = m_left.getName();
        } else {
          synsToBeRewrittenAsSyns[m_left.getName()] = m_right.getName();
        }
      } else {
        return synsToBeRewrittenAsSyns;
      }
    } else {
      if (Grammar::isStmt(m_left.getType())) {
        synsToBeRewrittenAsSyns[m_left.getName()] = m_right.getName();
      } else {
        synsToBeRewrittenAsSyns[m_right.getName()] = m_left.getName();
      }
    }
  } else if (Grammar::isValue(m_left.getAttr()) && Grammar::isValue(m_right.getAttr())) {
    synsToBeRewrittenAsSyns["-"] = "-";
  }

  return synsToBeRewrittenAsSyns;
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_INTEGERS WithEvaluator::evaluateIntAttrWithIntAttr() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_INTEGERS synsToBeRewrittenAsListOfInts;
  if (Grammar::isProgLine(m_left.getType()) && Grammar::isStmtNum(m_right.getAttr())) {
    
  } else if (Grammar::isProgLine(m_right.getType()) && Grammar::isStmtNum(m_left.getAttr())) {

  } else if (Grammar::isStmtNum(m_left.getAttr()) && Grammar::isValue(m_right.getAttr())) {
    MAP_OF_STMT_NUM_TO_GTYPE typeOfStmts = m_pkb->getTypeOfStatementTable();
    LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
    LIST_OF_STMT_NUMS commonNums;

    std::unordered_map<int, queryType::GType>::const_iterator got;
    for (auto& constant : allConstants) {
      got = typeOfStmts.find(std::stoi(constant));
      if (got != typeOfStmts.end()) {
        commonNums.push_back(got->first);
      }
    }
    
    synsToBeRewrittenAsListOfInts[m_left.getName()] = commonNums;
    synsToBeRewrittenAsListOfInts[m_right.getName()] = commonNums;
  } else if (Grammar::isValue(m_left.getAttr()) && Grammar::isStmtNum(m_right.getAttr())) {
    MAP_OF_STMT_NUM_TO_GTYPE typeOfStmts = m_pkb->getTypeOfStatementTable();
    LIST_OF_RESULTS allConstants = m_pkb->getAllConstants();
    LIST_OF_STMT_NUMS commonNums;

    std::unordered_map<int, queryType::GType>::const_iterator got;
    for (auto& constant : allConstants) {
      got = typeOfStmts.find(std::stoi(constant));
      if (got != typeOfStmts.end()) {
        commonNums.push_back(got->first);
      }
    }

    synsToBeRewrittenAsListOfInts[m_right.getName()] = commonNums;
    synsToBeRewrittenAsListOfInts[m_left.getName()] = commonNums;
  } else if (Grammar::isProgLine(m_left.getType()) && Grammar::isValue(m_right.getAttr())) {

  } else if (Grammar::isProgLine(m_right.getType()) && Grammar::isValue(m_left.getAttr())) {

  }

  return synsToBeRewrittenAsListOfInts;
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_STRINGS WithEvaluator::evaluateStrAttrWithStrAttr() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_STRINGS synsToBeRewrittenAsListOfStrs;

  if (Grammar::isProcName(m_left.getAttr())) {
    if (Grammar::isCall(m_left.getType())) {

    } else {
      LIST_OF_PROC_NAMES allProcs = m_pkb->getAllProcsName();
      LIST_OF_VAR_NAMES allVars = m_pkb->getAllVarNames();
      LIST_OF_RESULTS commonNames = EvaluatorUtil::getCommonResults(allProcs, allVars);
      if (commonNames.empty()) {
        return synsToBeRewrittenAsListOfStrs;
      }

      std::unordered_map<std::string, int>::const_iterator got1;
      std::unordered_map<std::string, int>::const_iterator got2;
      got1 = m_synonymsUsedInQuery.find(m_left.getName());
      got2 = m_synonymsUsedInQuery.find(m_right.getName());
      if (got1 != m_synonymsUsedInQuery.end() && got2 != m_synonymsUsedInQuery.end()) {
        if (got1->second >= got2->second) {
          synsToBeRewrittenAsListOfStrs[m_right.getName()] = commonNames;
        } else {
          synsToBeRewrittenAsListOfStrs[m_left.getName()] = commonNames;
        }
      } else {
        return synsToBeRewrittenAsListOfStrs;
      }
    }
  } else if (Grammar::isVarName(m_left.getAttr())) {
    if (Grammar::isCall(m_right.getType())) {

    } else {
      LIST_OF_PROC_NAMES allProcs = m_pkb->getAllProcsName();
      LIST_OF_VAR_NAMES allVars = m_pkb->getAllVarNames();
      LIST_OF_RESULTS commonNames = EvaluatorUtil::getCommonResults(allProcs, allVars);
      if (commonNames.empty()) {
        return synsToBeRewrittenAsListOfStrs;
      }

      std::unordered_map<std::string, int>::const_iterator got1;
      std::unordered_map<std::string, int>::const_iterator got2;
      got1 = m_synonymsUsedInQuery.find(m_left.getName());
      got2 = m_synonymsUsedInQuery.find(m_right.getName());
      if (got1 != m_synonymsUsedInQuery.end() && got2 != m_synonymsUsedInQuery.end()) {
        if (got1->second >= got2->second) {
          synsToBeRewrittenAsListOfStrs[m_right.getName()] = commonNames;
        } else {
          synsToBeRewrittenAsListOfStrs[m_left.getName()] = commonNames;
        }
      } else {
        return synsToBeRewrittenAsListOfStrs;
      }
    }
  }

  return synsToBeRewrittenAsListOfStrs;
}