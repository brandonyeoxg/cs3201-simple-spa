#pragma once
#ifndef QRP_H
#define QRP_H
#include <string>
#include <list>
#include <vector>

#include "../GlobalTypeDef.h"

/**
*  A class used to format the output string vector into a string list.
*/
class QueryResultProjector
{
public:
  /**
  * Formats the string vector into a list string.
  * @params result is a vector<string> from the QueryEvaluator.
  * @return The same result but as a list<string> instead.
  */
  RESULT_LIST formatResult(LIST_OF_RESULTS result);
};

#endif