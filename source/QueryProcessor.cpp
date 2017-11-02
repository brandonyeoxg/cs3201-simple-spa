#include "QueryProcessor.h"

RESULT_LIST QueryProcessor::runQueryProcessor(STRING t_stringInput) {
  QueryPreProcessor qpp;

  std::string declaration, query;
  std::string result;
  std::list<std::string> resultList;
  std::vector<std::string> evaluatedResults;
  bool isTokenized;

  declaration = qpp.splitStringDeclaration(t_stringInput);
  isTokenized = qpp.tokenizeDeclaration(declaration);
  
  if (isTokenized == true) {
    query = qpp.splitStringQuery(t_stringInput);
    isTokenized = qpp.tokenizeQuery(query);
    
    std::queue<Grammar> selectQueue = qpp.getSelect();
    std::queue<Relation> suchThatQueue = qpp.getSuchThat();
    std::queue<Pattern> patternQueue = qpp.getPattern();
    std::queue<With> withQueue = qpp.getWith();
    std::unordered_map<std::string, int> unorderedMap = qpp.getSynonym();

    if (isTokenized == true) {
      //Grammar testGrammar = selectQueue.front();
      //std::cout << "This is QueryProcessor testing selectQueue output: " << testGrammar.getName() << std::endl;
      QueryEvaluator *qe = new QueryEvaluator(m_pkb, selectQueue, suchThatQueue, patternQueue, withQueue, unorderedMap);
      evaluatedResults = qe->evaluateQuery();
      delete qe;

    }
  }
  
  std::queue<Grammar> selectQueue2 = qpp.getSelect();
  if (!selectQueue2.empty() && selectQueue2.front().getType() == queryType::GType::BOOLEAN && evaluatedResults.empty()) {
    evaluatedResults.push_back(FALSE);
  }

  QueryResultProjector qrp;
  resultList = qrp.formatResult(evaluatedResults);
  return resultList;
}