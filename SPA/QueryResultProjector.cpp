#include "QueryResultProjector.h"
#include <string>
#include <list>

using namespace std;

/**
*  A class responsible for formatting the result obtained by the PKB and then sending that result to the UI.
*/
class QueryResultProjector {
public:
  /**
  * A function that formats the result from the PKB into the desired output as specified by the requirements. 
  */
  void formatResult();
  /**
  * A function that prints the desired result and sends that result to the UI.
  */
  string printResult(list<string>);
};

void QueryResultProjector::formatResult(){

}

string QueryResultProjector::printResult(list<string>) {
  return "";
}
