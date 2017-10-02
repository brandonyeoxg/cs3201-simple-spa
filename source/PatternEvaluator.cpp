#pragma once

#include "PatternEvaluator.h"

std::vector<std::string> PatternEvaluator::formatListIntToVectorStr(std::list<int> t_listInt) {
  std::vector<std::string> vectorStr;
  for (auto& x : t_listInt) {
    vectorStr.push_back(std::to_string(x));
  }

  return vectorStr;
}

std::vector<std::string> PatternEvaluator::formatVectorIntToVectorStr(std::vector<int> t_vectorInt) {
  std::vector<std::string> vectorStr;
  for (auto& x : t_vectorInt) {
    vectorStr.push_back(std::to_string(x));
  }

  return vectorStr;
}

SET_OF_RESULTS PatternEvaluator::formatMapIntStrToMapStrVectorStr(std::unordered_map<int, std::string> t_mapIntStr) {
  std::unordered_map<std::string, std::vector<std::string>> mapStrVectorStr;
  for (auto& x : t_mapIntStr) {
    std::vector<std::string> vectorStr;
    vectorStr.push_back(x.second);
    mapStrVectorStr[std::to_string(x.first)] = vectorStr;
  }

  return mapStrVectorStr;
}