#include "Formatter.h"

std::vector<std::string> Formatter::formatVectorIntToVectorStr(std::vector<int> t_vectorInt) {
  std::vector<std::string> vectorStr;
  for (auto& x : t_vectorInt) {
    vectorStr.push_back(std::to_string(x));
  }

  return vectorStr;
}

std::vector<int> Formatter::formatVectorStrToVectorInt(std::vector<std::string> t_vectorStr) {
  std::vector<int> vectorInt;
  for (auto& x : t_vectorStr) {
    vectorInt.push_back(std::stoi(x));
  }

  return vectorInt;
}

std::vector<std::string> Formatter::formatListStrToVectorStr(std::list<std::string> t_listStr) {
  std::vector<std::string> vectorStr;
  for (auto& x : t_listStr) {
    vectorStr.push_back(x);
  }

  return vectorStr;
}

std::vector<std::string> Formatter::formatListIntToVectorStr(std::list<int> t_listInt) {
  std::vector<std::string> vectorStr;
  for (auto& x : t_listInt) {
    vectorStr.push_back(std::to_string(x));
  }

  return vectorStr;
}

SET_OF_RESULTS Formatter::formatMapIntStrToMapStrVectorStr(std::unordered_map<int, std::string> t_mapIntStr) {
  std::unordered_map<std::string, std::vector<std::string>> mapStrVectorStr;
  for (auto& x : t_mapIntStr) {
    std::vector<std::string> vectorStr;
    vectorStr.push_back(x.second);
    mapStrVectorStr[std::to_string(x.first)] = vectorStr;
  }

  return mapStrVectorStr;
}

SET_OF_RESULTS_INDICES Formatter::formatMapIntIntToMapIntVectorInt(std::unordered_map<int, int> t_mapIntInt) {
  SET_OF_RESULTS_INDICES mapIntVectorInt;
  for (auto& x : t_mapIntInt) {
    LIST_OF_RESULTS_INDICES vectorInt;
    vectorInt.push_back(x.second);
    mapIntVectorInt[x.first] = vectorInt;
  }

  return mapIntVectorInt;
}

SET_OF_RESULTS Formatter::formatVectorStrToMapStrVectorStr(LIST_OF_RESULTS t_vectorStr) {
  std::unordered_map<std::string, std::vector<std::string>> mapStrVectorStr;
  for (auto& x : t_vectorStr) {
    std::vector<std::string> vectorStr;
    vectorStr.push_back(x);
    mapStrVectorStr[x] = vectorStr;
  }

  return mapStrVectorStr;
}

LIST_OF_RESULTS Formatter::formatMapStrVectorStrToVectorStr(SET_OF_RESULTS t_mapStrVectorStr) {
  std::vector<std::string> vectorStr;
  for (auto& x : t_mapStrVectorStr) {
    vectorStr.push_back(x.first);
  }

  return vectorStr;
}

LIST_OF_RESULTS_INDICES Formatter::formatMapIntVectorIntToVectorInt(SET_OF_RESULTS_INDICES t_mapIntVectorInt) {
  std::vector<int> vectorInt;
  for (auto& x : t_mapIntVectorInt) {
    vectorInt.push_back(x.first);
  }

  return vectorInt;
}