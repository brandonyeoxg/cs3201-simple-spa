#include "QueryProcessor.h"
#include "QueryPreProcessor.h"
#include "QueryEvaluator.h"
#include "QueryResultProjector.h"
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
  QueryPreProcessor.tokenizeInput();
  QueryPreProcessor.xxx();
  QueryEvaluator.getResult();
  QueryResultProjector.formatResult();
  QueryResultProjector.printResult();
}
