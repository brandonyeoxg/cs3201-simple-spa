#include "QueryProcessor.h"

std::list<std::string> QueryProcessor::runQueryProcessor(std::string testInput) {
  QueryPreProcessor qpp;

  std::cout << "initial test input: " << testInput << std::endl;
  std::string declaration, query;
  std::string result;
  std::list<std::string> resultList;
  std::vector<std::string> evaluatedResults;


  declaration = qpp.splitStringDeclaration(testInput);
  qpp.tokenizeDeclaration(declaration);
  query = qpp.splitStringQuery(testInput);
  qpp.tokenizeQuery(query);

  std::queue<Grammar> selectQueue = qpp.getSelect();
  std::queue<Relation> suchThatQueue = qpp.getSuchThat();
  std::queue<Pattern> patternQueue = qpp.getPattern();
  std::unordered_map<std::string, int> unorderedMap = qpp.getSynonym();

  Grammar testGrammar = selectQueue.front();
  //std::cout << "This is QueryProcessor testing selectQueue output: " << testGrammar.getName() << std::endl;
  //QueryEvaluator *qe = new QueryEvaluator(m_pkb, selectQueue, suchThatQueue, patternQueue);
  //evaluatedResults = qe->evaluateQuery();

  QueryResultProjector qrp;
  resultList = qrp.formatResult(evaluatedResults);
  
  return resultList;
}