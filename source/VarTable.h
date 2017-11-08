#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include "GlobalTypeDef.h"

/**
* Represents the variable table, which is a vector of unordered_maps to 
* track which variable appear on which line.
*
* @author pengcheng
* @date 8/27/2017
*
*/
class VarTable {
public:
  VAR_INDEX insertVar(VAR_NAME t_name);
  VAR_NAME getVarNameFromIdx(VAR_INDEX t_idx);
  VAR_INDEX getVarIdxFromName(VAR_NAME t_varName);
  LIST_OF_VAR_NAMES& getAllVarNames();
  LIST_OF_VAR_INDICES& getAllVarIndices();

private:
  std::vector<VAR_NAME> m_varIdxToName;
  std::unordered_map<VAR_NAME, VAR_INDEX> m_varNameToIdx;
  LIST_OF_VAR_INDICES m_allVarIndices;
};
