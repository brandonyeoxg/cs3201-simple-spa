#include "QueryProcessor.h"

RESULT_LIST QueryProcessor::runQueryProcessor(STRING t_stringInput) {
  QueryPreProcessor qpp;

  std::string declaration, query;
  std::string result;
  std::list<std::string> resultList;
  std::vector<std::string> evaluatedResults;
  bool isTokenized;
  bool isTokenizedDeclaration;

  declaration = qpp.splitStringDeclaration(t_stringInput);
  isTokenizedDeclaration = qpp.tokenizeDeclaration(declaration);
  
    query = qpp.splitStringQuery(t_stringInput);
    isTokenized = qpp.tokenizeQuery(query);
    
    //Old Queues to be passed to QE
    std::queue<Grammar> selectQueue = qpp.getSelect();
    std::queue<Relation> suchThatQueue = qpp.getSuchThat();
    std::queue<Pattern> patternQueue = qpp.getPattern();
    std::queue<With> withQueue = qpp.getWith();

    //New Vectors to be passed to QE
    std::vector<Grammar> selectVector = qpp.getSelectVector();
    std::vector<Relation> suchThatVector = qpp.getSuchThatVector();
    std::vector<Pattern> patternVector = qpp.getPatternVector();
    std::vector<With> withVector = qpp.getWithVector();
    bool isSelectBooleanExists = qpp.isSelectBoolean();

    std::unordered_map<std::string, int> unorderedMap = qpp.getSynonym();

    if (isTokenized == true && isTokenizedDeclaration == true) {
      //Grammar testGrammar = selectQueue.front();
      //std::cout << "This is QueryProcessor testing selectQueue output: " << testGrammar.getName() << std::endl;
      //QueryEvaluator *qe = new QueryEvaluator(m_pkb, selectQueue, suchThatQueue, patternQueue, withQueue, unorderedMap);
      
      //New QueryEvaluator Constructor
      QueryEvaluator *qe = new QueryEvaluator(m_pkb, selectVector, suchThatVector, patternVector, withVector, unorderedMap);
      evaluatedResults = qe->evaluateQuery();
      delete qe;

    }
  
  std::vector<Grammar> selectVectorValidate = qpp.getSelectVector();
  if (!selectVectorValidate.empty() && selectVectorValidate.front().getType() == queryType::GType::BOOLEAN && evaluatedResults.empty()\
    || (isTokenizedDeclaration == false && isTokenized == true)
    || (isTokenizedDeclaration == true && isTokenized == false && selectVectorValidate.empty()) && isSelectBooleanExists) {
    evaluatedResults.push_back(FALSE);
  }

  QueryResultProjector qrp;
  resultList = qrp.formatResult(evaluatedResults);
  return resultList;
}