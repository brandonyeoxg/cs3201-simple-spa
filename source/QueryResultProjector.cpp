#include "QueryResultProjector.h"
#include <string>
#include <list>

std::list<std::string> QueryResultProjector::formatResult(std::vector<std::string> result) {
  std::list<std::string> resultList;
  for (std::vector<std::string>::iterator getStmts = result.begin(); getStmts != result.end(); ++getStmts) {
    resultList.push_back(*getStmts);
  }

  return resultList;
}
