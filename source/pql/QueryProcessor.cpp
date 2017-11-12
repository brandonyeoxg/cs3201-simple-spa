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

    //New Vectors to be passed to QE
    std::vector<Grammar> selectVector = qpp.getSelectVector();
    std::vector<Relation> suchThatVector = qpp.getSuchThatVector();
    std::vector<Pattern> patternVector = qpp.getPatternVector();
    std::vector<With> withVector = qpp.getWithVector();
    bool isSelectBooleanExists = qpp.isSelectBoolean();

    std::unordered_map<std::string, int> unorderedMap = qpp.getSynonym();

    if (isTokenized == true && isTokenizedDeclaration == true) {
   
     //New QueryEvaluator Constructor
      QueryEvaluator *qe = new QueryEvaluator(m_pkb, selectVector, suchThatVector, patternVector, withVector, unorderedMap);
      evaluatedResults = qe->evaluateQuery();
      delete qe;

    }
  
  std::vector<Grammar> selectVectorValidate = qpp.getSelectVector();
  if (!selectVectorValidate.empty() && selectVectorValidate.front().getType() == queryType::GType::BOOLEAN && evaluatedResults.empty()
    || (isTokenizedDeclaration == false && isTokenized == true && isSelectBooleanExists)
    || (isTokenizedDeclaration == true && isTokenized == false && isSelectBooleanExists)) {
    evaluatedResults.push_back(FALSE);
  }

  QueryResultProjector qrp;
  resultList = qrp.formatResult(evaluatedResults);
  return resultList;
}