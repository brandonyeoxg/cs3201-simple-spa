#include "QueryProcessor.h"
#include "QueryPreProcessor.h"
//#include "QueryEvaluator.h"
//#include "QueryProjector.h"
#include <iostream>
#include <string>

using namespace std;

QueryProcessor::QueryProcessor()
{
}


QueryProcessor::~QueryProcessor()
{
}

void QueryProcessor::runQueryProcessor(void) {
  QueryPreProcessor q1;

  string testInput = "assign a; Select a such that Parent*(s,v)";

  q1.tokenizeInput(testInput);
  //QueryPreProcessor.xxx();
  //QueryEvaluator.evaluateResult();
  //QueryProjector.formatResult();
  //QueryProjector.printResult();
}
