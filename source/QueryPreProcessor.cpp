#include "QueryPreProcessor.h"

QueryPreProcessor::QueryPreProcessor()
{
}


QueryPreProcessor::~QueryPreProcessor()
{
}

std::queue<Grammar> QueryPreProcessor::getSelect(void) {
  return m_selectQueue;
}

std::queue<DesignAbstraction> QueryPreProcessor::getSuchThat(void) {
  return m_suchThatQueue;
}

std::queue<std::string> QueryPreProcessor::getPattern(void) {
  return m_patternQueue;
}

std::string QueryPreProcessor::splitStringDeclaration(std::string t_Input) {
  std::string delimiter = "; Select";
  std::string declaration = t_Input.substr(0, t_Input.find(delimiter));  //.find(delimiter) finds starting position of the delimiter, hence need to + 1
  std::string query = t_Input.substr(t_Input.find(delimiter) + 2, t_Input.size()); //same for this as delimiter is "; Select"

  return declaration;
}

std::string QueryPreProcessor::splitStringQuery(std::string t_Input) {
  std::string delimiter = "; Select";
  std::string declaration = t_Input.substr(0, t_Input.find(delimiter));  //.find(delimiter) finds starting position of the delimiter, hence need to + 1
  std::string query = t_Input.substr(t_Input.find(delimiter) + 2, t_Input.size()); //same for this as delimiter is "; Select"

  return query;
}

/** This is the tokenizing function for declarations
@param t_Input string input for declaration
@return results This returns a vector for tokens in declarations
*/
std::vector<std::string> QueryPreProcessor::tokenizeDeclaration(std::string t_declarationInput) {

  std::vector<std::string> declarationVector; 

  //tokens are split by ;
  std::size_t prev_pos = 0, pos;
  while ((pos = t_declarationInput.find("; ", prev_pos)) != std::string::npos) {
    if (prev_pos == 0) {
      declarationVector.push_back(t_declarationInput.substr(prev_pos, pos - prev_pos));
      prev_pos = pos + 1;
    }
    if (pos > prev_pos)
      declarationVector.push_back(t_declarationInput.substr(prev_pos + 1, pos - prev_pos - 1)); //prev pos + 1 so that we can delete the trailing space in "; "
    prev_pos = pos + 1;
  }
  if (prev_pos < t_declarationInput.length()) {
    declarationVector.push_back(t_declarationInput.substr(prev_pos + 1, std::string::npos));  //prev pos + 1 so that we can delete the trailing space in "; "
  }

  //tokens are split by entities and split by variables
  int numberOfVectors = declarationVector.size();
  std::cout << numberOfVectors << '\n';

  std::string delimiterSpace = " ";
  std::string tempString;

  for (std::size_t j = 0; j != declarationVector.size(); ++j) {
    tempString = declarationVector.at(j);
    //cout << tempString << " testing" << endl;
    std::string entity = tempString.substr(0, tempString.find(delimiterSpace));
    std::string variables = tempString.substr(tempString.find(delimiterSpace) + 1, tempString.size()); //same for this as delimiter is "; Select"

    //variables split individually
    prev_pos = 0;
    std::vector<std::string> variableVector;
    while ((pos = variables.find_first_of(", ", prev_pos)) != std::string::npos) {
      if (pos > prev_pos) {
        variableVector.push_back(variables.substr(prev_pos, pos - prev_pos));
      }
      prev_pos = pos + 1;
    }
    if (prev_pos < variables.length()) {
      variableVector.push_back(variables.substr(prev_pos, std::string::npos));
    }

    for (auto l = variableVector.begin(); l != variableVector.end(); ++l) {
      if (entity == "procedure") {
        Grammar g(0, variableVector.at(j));
        m_grammarVector.push_back(g);
      } else if (entity == "stmtLst") {
        Grammar g(1, variableVector.at(j));
        m_grammarVector.push_back(g);
      }
      else if (entity == "stmt") {
        Grammar g(2, variableVector.at(j));
        m_grammarVector.push_back(g);
      }
      else if (entity == "assign") {
        Grammar g(3, variableVector.at(j));
        m_grammarVector.push_back(g);
      }
      else if (entity == "while") {
        Grammar g(4, variableVector.at(j));
        m_grammarVector.push_back(g);
      }
      else if (entity == "if") {
        Grammar g(5, variableVector.at(j));
        m_grammarVector.push_back(g);
      }
      else if (entity == "expression") {
        Grammar g(6, variableVector.at(j));
        m_grammarVector.push_back(g);
      }
      else if (entity == "variable") {
        Grammar g(7, variableVector.at(j));
        m_grammarVector.push_back(g);
      }
      else if (entity == "constant") {
        Grammar g(8, variableVector.at(j));
        m_grammarVector.push_back(g);
      }
      else {
        //do nothing
      }
    }
  }

  return declarationVector;
}


std::vector<std::string> QueryPreProcessor::tokenizeQuery(std::string t_queryInput) {
  std::vector<std::string> queryVector;
  std::string delimiterSelect = "Select";
  std::string delimiterSuchThat = "such that";
  std::string delimiterPattern = "pattern";
  std::string selectStatement;
  std::string suchThatStatement;
  std::string patternStatement;
  std::string tempStatement;

  if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos) 
    if (t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterPattern)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      suchThatStatement = tempStatement.substr(tempStatement.find(delimiterSuchThat), tempStatement.find(delimiterPattern));
      patternStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
    }
    else if (t_queryInput.find(delimiterPattern) < t_queryInput.find(delimiterSuchThat)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
      patternStatement = tempStatement.substr(tempStatement.find(delimiterPattern), tempStatement.find(delimiterSuchThat));
      suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      //cout << "abc2" << endl;
    }
    else if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
      suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      //cout << "abc3" << endl;
  }
  else if (t_queryInput.find(delimiterSuchThat) == std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos) {
    selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
    patternStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
    //cout << "abc4" << endl;
  }
  else {
    selectStatement = t_queryInput;
  }

  std::string delimiterDesignAbstraction = "such that";
  std::string designAbstractions = suchThatStatement.substr(suchThatStatement.find(delimiterDesignAbstraction) + 10, suchThatStatement.size()); //"such that " is 10 characters

  //IMPT: to be worked on in the future for more complex queries
  std::string delimiterBracket = "(";
  std::string designAbstractionEntity = designAbstractions.substr(0, designAbstractions.find(delimiterBracket));
  std::string designAbstractionObject = designAbstractions.substr(designAbstractions.find(delimiterBracket), designAbstractions.size());

  int pos1;
  int prev_pos_new = 0;
  std::vector<std::string> variableVectorNew;
  while ((pos1 = designAbstractionObject.find_first_of("() ,", prev_pos_new)) != std::string::npos) {
    if (pos1 > prev_pos_new) {
      variableVectorNew.push_back(designAbstractionObject.substr(prev_pos_new, pos1 - prev_pos_new));
    }
    prev_pos_new = pos1 + 1;
  }
  if (prev_pos_new < designAbstractionObject.length()) {
    variableVectorNew.push_back(designAbstractionObject.substr(prev_pos_new, std::string::npos));
  }

  Grammar g1(0, "t1"), g2(0, "t2");

  //DesignAbstraction class to be discussed
  DesignAbstraction DAO(delimiterDesignAbstraction, g1, g2);

  m_suchThatQueue.push(DAO);

  return queryVector;
}

void QueryPreProcessor::printVector(std::vector<std::string>declarationVector) {
  for (auto i = declarationVector.begin(); i != declarationVector.end(); ++i){
    std::cout << *i << ' ';
  }
}
