#include "QueryProcessor.h"

std::string QueryProcessor::runQueryProcessor(std::string testInput) {
  QueryPreProcessor qpp;
  QueryEvaluator *qe = new QueryEvaluator(m_pkb);
  QueryResultProjector qrp;

  //std::string testInput = "assign a; Select a such that Parent*(s,v)";
  std::string declaration, query;
  std::string result;
  std::list<std::string> resultlist;

  declaration = qpp.splitStringDeclaration(testInput);
  qpp.tokenizeDeclaration(declaration);
  query = qpp.splitStringQuery(testInput);
  qpp.tokenizeQuery(query);

  qe->evaluateQuery();

  qrp.formatResult();
  result = qrp.printResult(resultlist);

  return result;

}