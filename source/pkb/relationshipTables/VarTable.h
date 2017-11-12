#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include "../../GlobalTypeDef.h"

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

  /**
  * Method that inserts the variable to the unordered map of vectors containing the index of the term as key.
  * @param t_name the variable name.
  * @return the index number of the variable in the table.
  */
  VAR_INDEX insertVar(VAR_NAME t_name);

  /**
  * Method that returns the index of the variable t_varName in VarTable.
  * @param t_varName a string argument.
  * @return the index of variable.
  */
  VAR_NAME getVarNameFromIdx(VAR_INDEX t_idx);

  /**
  * Method that returns the index of the variable t_varName in VarTable.
  * @param t_varName a string argument.
  * @return the index of variable.
  */
  VAR_INDEX getVarIdxFromName(VAR_NAME t_varName);

  /**
  * Method that returns the vector of variables that are stored within VarTable.
  * @return a vector of statement numbers.
  */
  LIST_OF_VAR_NAMES& getAllVarNames();
  LIST_OF_VAR_INDICES& getAllVarIndices();

private:
  LIST_OF_VAR_INDICES m_allVarIndices;
  LIST_OF_VAR_NAMES m_varIdxToName;
  MAP_OF_VAR_NAME_TO_VAR_INDEX m_varNameToIdx;
};
