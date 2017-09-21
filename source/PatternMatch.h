#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include ".\GlobalTypeDef.h"

class PatternMatch {
public:
  PatternMatch();

private:
  std::unordered_map<STMT_NUM, std::vector<string>> assignStmts;
  std::unordered_map<STMT_NUM, std::vector<string>> assignStmtsSubtrees;
};