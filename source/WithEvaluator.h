#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"
#include "Grammar.h"
#include "PkbReadOnly.h"
#include "EvaluatorUtil.h"

class WithEvaluator {
public:
  WithEvaluator(PkbReadOnly *t_pkb, Grammar t_left, Grammar t_right)
    : m_pkb(t_pkb),
      m_left(t_left),
      m_right(t_right) {};


  BOOLEAN isEquals();
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_INTEGERS evaluateSynWithInt();
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_STRING evaluateSynWithStr();
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_SYNONYMS evaluateSynWithSyn();
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_INTEGERS evaluateIntAttrWithIntAttr();
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_STRINGS evaluateStrAttrWithStrAttr();
  
private:
  PkbReadOnly *m_pkb;
  Grammar m_left;
  Grammar m_right;
};