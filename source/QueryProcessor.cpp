#include "QueryProcessor.h"

std::list<std::string> QueryProcessor::runQueryProcessor(std::string testInput) {
  QueryPreProcessor qpp;
  QueryResultProjector qrp;

  std::cout << "initial test input: " << testInput << std::endl;
  std::string declaration, query;
  std::string result;
  std::list<std::string> resultlist;

  declaration = qpp.splitStringDeclaration(testInput);
  qpp.tokenizeDeclaration(declaration);
  query = qpp.splitStringQuery(testInput);
  qpp.tokenizeQuery(query);

  std::queue<Grammar> selectQueue = qpp.getSelect();
  std::queue<DesignAbstraction> suchThatQueue = qpp.getSuchThat();
  std::queue<Pattern> patternQueue = qpp.getPattern();

  Grammar testGrammar = selectQueue.front();
  std::cout << "This is QueryProcessor testing selectQueue output: " << testGrammar.getName() << std::endl;
  //QueryEvaluator *qe = new QueryEvaluator(m_pkb, selectQueue, suchThatQueue, patternQueue);
  //qe->evaluateQuery();

  qrp.formatResult();
  result = qrp.printResult(resultlist);

  return resultlist;

}