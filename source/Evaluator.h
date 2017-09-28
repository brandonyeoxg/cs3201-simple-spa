#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GlobalTypeDef.h"

class Evaluator {
public:
  virtual SET_OF_RESULTS evaluate();
};