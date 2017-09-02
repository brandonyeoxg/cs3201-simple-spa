#include "QueryPreProcessor.h"
#include "Grammar.h"

QueryPreProcessor::QueryPreProcessor()
{
}


QueryPreProcessor::~QueryPreProcessor()
{
}

std::queue<std::string> QueryPreProcessor::getSelect(void) {
  return m_selectQueue;
}

std::queue<std::string> QueryPreProcessor::getSuchThat(void) {
  return m_suchThatQueue;
}

std::queue<std::string> QueryPreProcessor::getPattern(void) {
  return m_patternQueue;
}

std::string QueryPreProcessor::splitString(std::string t_Input) {
  std::string delimiter = "; Select";
  std::string declaration = t_Input.substr(0, t_Input.find(delimiter));  //.find(delimiter) finds starting position of the delimiter, hence need to + 1
  std::string query = t_Input.substr(t_Input.find(delimiter) + 2, t_Input.size()); //same for this as delimiter is "; Select"

  return declaration;
}

/** This is the tokenizing function for declarations
@param t_Input string input for declaration
@return results This returns a vector for tokens in declarations
*/
std::vector<std::string> QueryPreProcessor::tokenizeDeclaration(std::string t_declarationInput) {

  std::vector<std::string> declarationVector; 

  std::size_t prev_pos = 0, pos;
  while ((pos = t_declarationInput.find_first_of("; ,", prev_pos)) != std::string::npos) {
    if (pos > prev_pos)
      declarationVector.push_back(t_declarationInput.substr(prev_pos, pos - prev_pos));
    prev_pos = pos + 1;
  }
  if (prev_pos < t_declarationInput.length()) {
    declarationVector.push_back(t_declarationInput.substr(prev_pos, std::string::npos));
  }
  return declarationVector;
}

std::vector<std::string> QueryPreProcessor::tokenizeQuery(std::string t_queryInput) {
  std::vector<std::string> abc;
  std::string delimiterSelect = "Select";
  std::string delimiterSuchThat = "such that";
  std::string delimiterPattern = "pattern";
  std::string selectStatement;
  std::string suchThatStatement;
  std::string patternStatement;
  std::string tempStatement;

  if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos) {
    cout << "abc" << endl;
    cout << t_queryInput.find(delimiterSuchThat) << endl;
    if (t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterPattern)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      suchThatStatement = tempStatement.substr(tempStatement.find(delimiterSuchThat), tempStatement.find(delimiterPattern));
      patternStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
      cout << "abc1" << endl;
    }
    else if (t_queryInput.find(delimiterPattern) < t_queryInput.find(delimiterSuchThat)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
      patternStatement = tempStatement.substr(tempStatement.find(delimiterPattern), tempStatement.find(delimiterSuchThat));
      suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      cout << "abc2" << endl;
    }
  }
  else if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos) {
    selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
    suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
    cout << "abc3" << endl;
  }
  else if (t_queryInput.find(delimiterSuchThat) == std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos) {
    selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
    patternStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
    cout << "abc4" << endl;
  }
  else {
    selectStatement = t_queryInput;
    cout << "abc5" << endl;
  }

  return abc;
}

void QueryPreProcessor::printVector(std::vector<std::string>declarationVector) {
  for (auto i = declarationVector.begin(); i != declarationVector.end(); ++i){
    std::cout << *i << ' ';
  }
}
