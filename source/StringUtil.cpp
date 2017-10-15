#include "StringUtil.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<std::string> StringUtil::splitString(STRING t_line, STRING_CHARACTER t_delim) {
  std::vector<std::string> tokens;
  std::stringstream ss(t_line);
  std::string token;

  while (std::getline(ss, token, t_delim)) {
    tokens.push_back(token);
  }

  return tokens;
}

std::string StringUtil::trimString(const STRING& str,
  const STRING& whitespace) {
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == std::string::npos) {
    return ""; // no content
  }

  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
}

std::string StringUtil::reduceString(const STRING& str,
  const STRING& fill,
  const STRING& whitespace) {
  // trim first
  auto result = trimString(str, whitespace);

  // replace sub ranges
  auto beginSpace = result.find_first_of(whitespace);
  while (beginSpace != std::string::npos) {
    const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
    const auto range = endSpace - beginSpace;

    result.replace(beginSpace, range, fill);

    const auto newStart = beginSpace + fill.length();
    beginSpace = result.find_first_of(whitespace, newStart);
  }

  return result;
}

void StringUtil::removeWhiteSpace(STRING &str) {
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}

void StringUtil::toLower(STRING &str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}