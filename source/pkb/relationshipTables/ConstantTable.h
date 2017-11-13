#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <unordered_map>

#include "../../GlobalTypeDef.h"

/**
* Represents the constants made in the program.
* The constants are stored in different representation for fast retrieval from PQL through the PKB.
*
* @author pengcheng
*/
class ConstantTable {
public:
  ConstantTable();
  ~ConstantTable() {};

  /**
  * Method that inserts the constant term to the unordered map of vectors containing the index of the term as key.
  * @param t_constant the constant term.
  * @return the index number of the constant term in the table.
  */
  CONSTANT_INDEX insertConstant(std::string t_constant);

  /**
  * Gets all the constants in the source program.
  * @return the list of all constants.
  */
  LIST_OF_RESULTS getAllConstants();
  LIST_OF_CONSTANT_INDICES getAllConstantsByIdx();

  /**
  * Gets the constant index given its string in the Constant table.
  * @param t_constant the constant string.
  * @return the index of the constant.
  */
  CONSTANT_INDEX getConstantIdxFromConstant(CONSTANT_TERM t_constant);

  /**
  * Gets the constant string given its index in the Constant table.
  * @param t_constantIdx the index of the constant.
  * @return the constant string.
  */
  CONSTANT_TERM getConstantFromIdx(int t_constantIdx);

private:
  SET_OF_CONSTANT_TERMS m_constantSet;
  SET_OF_CONSTANT_INDICES m_constantSetByIdx;
  MAP_OF_CONSTANT_INDEX_TO_CONSTANT_TERM m_constantMap;
  MAP_OF_CONSTANT_TERM_TO_CONSTANT_INDEX m_constantByIdxMap;

};