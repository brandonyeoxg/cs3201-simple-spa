#pragma once

#include "WithEvaluator.h"

BOOLEAN WithEvaluator::isEquals() {
  return true;
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_INTEGERS WithEvaluator::evaluateSynWithInt() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_INTEGERS synsToBeRewrittenAsInts;
  return synsToBeRewrittenAsInts;
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_STRING WithEvaluator::evaluateSynWithStr() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_STRING synsToBeRewrittenAsStr;
  return synsToBeRewrittenAsStr;
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_SYNONYMS WithEvaluator::evaluateSynWithSyn() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_SYNONYMS synsToBeRewrittenAsSyns;
  return synsToBeRewrittenAsSyns;
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_INTEGERS WithEvaluator::evaluateIntAttrWithIntAttr() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_INTEGERS synsToBeRewrittenAsListOfInts;
  return synsToBeRewrittenAsListOfInts;
}

MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_STRINGS WithEvaluator::evaluateStrAttrWithStrAttr() {
  MAP_OF_SYNONYMS_TO_BE_REWRITTEN_AS_LIST_OF_STRINGS synsToBeRewrittenAsListOfStrs;
  return synsToBeRewrittenAsListOfStrs;
}