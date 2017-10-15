#include "QueryResultProjector.h"
#include <string>
#include <list>

RESULT_LIST QueryResultProjector::formatResult(LIST_OF_RESULTS result) {
  std::list<std::string> resultList;
  for (std::vector<std::string>::iterator getStmts = result.begin(); getStmts != result.end(); ++getStmts) {
    resultList.push_back(*getStmts);
  }

  return resultList;
}
