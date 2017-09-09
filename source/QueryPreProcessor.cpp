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

std::queue<Pattern> QueryPreProcessor::getPattern(void) {
  return m_patternQueue;
}

std::vector<Grammar> QueryPreProcessor::getGrammarVector(void) {
  return m_grammarVector;
}

std::string QueryPreProcessor::splitStringDeclaration(std::string t_Input) {
  std::string delimiter = "Select";
  std::string declaration = t_Input.substr(0, t_Input.find(delimiter));  //.find(delimiter) finds starting position of the delimiter, hence need to + 1
  std::string query = t_Input.substr(t_Input.find(delimiter), t_Input.size()); //same for this as delimiter is "; Select"
  
  declaration = m_stringUtil.trimString(declaration);
  //std::cout << "this is splitString debugging declaration: " << declaration << std::endl;
  return declaration;
}

std::string QueryPreProcessor::splitStringQuery(std::string t_Input) {
  std::string delimiter = "Select";
  std::string declaration = t_Input.substr(0, t_Input.find(delimiter));  //.find(delimiter) finds starting position of the delimiter, hence need to + 1
  std::string query = t_Input.substr(t_Input.find(delimiter), t_Input.size()); //same for this as delimiter is "; Select"

  //std::cout << "this is splitString debugging query: " << query << std::endl;
  query = m_stringUtil.trimString(query);
  return query;
}

/** This is the tokenizing function for declarations
@param t_Input string input for declaration
@return results This returns a vector for tokens in declarations
*/
bool QueryPreProcessor::tokenizeDeclaration(std::string t_declarationInput) {
  bool isTokenized = false;
  std::vector<std::string> declarationVector;
  //std::cout << t_declarationInput << "test1" << std::endl;

  std::string starterString;
  //tokens are split by ;
  std::size_t prev_pos = 0, pos;
  starterString = m_stringUtil.trimString(t_declarationInput);
  //std::cout << "Starter String: " << starterString << std::endl;

  //for cases with only one relation(such that)
  if ((pos = starterString.find(";", prev_pos)) == std::string::npos) {
    declarationVector.push_back(starterString.substr(prev_pos, pos - prev_pos));
  }

  //rest of cases
  else {
    while ((pos = starterString.find(";", prev_pos)) != std::string::npos) {
      if (prev_pos == 0) {
        declarationVector.push_back(starterString.substr(prev_pos, pos - prev_pos));
        //printVector(declarationVector);
        //std::cout << "above is the declaration initial object" << std::endl;
        prev_pos = pos + 1;
      }
      if (pos > prev_pos) {
        declarationVector.push_back(starterString.substr(prev_pos + 1, pos - prev_pos - 1)); //prev pos + 1 so that we can delete the trailing space in "; "
      } prev_pos = pos + 1;
    }
    if (prev_pos < starterString.length()) {
      declarationVector.push_back(starterString.substr(prev_pos + 1, std::string::npos));  //prev pos + 1 so that we can delete the trailing space in "; "
    }
  }
  //printVector(declarationVector);

  //tokens are split by entities and split by variables
  //int numberOfVectors = declarationVector.size();
  //std::cout << numberOfVectors << '\n';

  std::string delimiterSpace = " ";
  std::string tempString;

  for (std::size_t j = 0; j != declarationVector.size(); ++j) {
    tempString = declarationVector.at(j);
    //std::cout << tempString << " testing" << std::endl;
    std::string entity = tempString.substr(0, tempString.find(delimiterSpace));
    std::string variables = tempString.substr(tempString.find(delimiterSpace) + 1, tempString.size()); //same for this as delimiter is "; Select" variables split individually

    entity = m_stringUtil.trimString(entity);
    variables = m_stringUtil.trimString(variables);
    
    //std::cout << "This is entity: " << entity << std::endl;
    //std::cout << "This is variables: " << variables << std::endl;

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

    int counterL=0;
    for (auto l = variableVector.begin(); l != variableVector.end(); l++, counterL++) {
      if (entity == "procedure") {
        Grammar g(0, variableVector.at(counterL));
        m_grammarVector.push_back(g);
      } else if (entity == "stmtLst") {
        Grammar g(1, variableVector.at(counterL));
        m_grammarVector.push_back(g);
      } else if (entity == "stmt") {
        Grammar g(2, variableVector.at(counterL));
        m_grammarVector.push_back(g);
      } else if (entity == "assign") {
        Grammar g(3, variableVector.at(counterL));
        m_grammarVector.push_back(g);
      } else if (entity == "while") {
        Grammar g(4, variableVector.at(counterL));
        m_grammarVector.push_back(g);
      } else if (entity == "if") {
        Grammar g(5, variableVector.at(counterL));
        m_grammarVector.push_back(g);
      } else if (entity == "expression") {
        Grammar g(6, variableVector.at(counterL));
        m_grammarVector.push_back(g);
      } else if (entity == "variable") {
        Grammar g(7, variableVector.at(counterL));
        m_grammarVector.push_back(g);
      } else {
        //do nothing
      }
    }
  }
  isTokenized = true;
  return isTokenized;
}

bool QueryPreProcessor::tokenizeQuery(std::string t_queryInput) {
  bool isTokenized = false;
  std::vector<std::string> queryVector;
  std::string delimiterSelect = "Select";
  std::string delimiterSuchThat = "such that";
  std::string delimiterPattern = "pattern";
  std::string selectStatement;
  std::string suchThatStatement;
  std::string patternStatement;
  std::string tempStatement;

  //test comment
  //t_queryInput = "Select s such that Uses(2, \"x\")";

  if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos) {
    if (t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterPattern)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      suchThatStatement = tempStatement.substr(tempStatement.find(delimiterSuchThat), tempStatement.find(delimiterPattern));
      patternStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
    } else if (t_queryInput.find(delimiterPattern) < t_queryInput.find(delimiterSuchThat)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
      patternStatement = tempStatement.substr(tempStatement.find(delimiterPattern), tempStatement.find(delimiterSuchThat));
      suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      //std::cout << "abc2" << std::endl;
    } else {
      //std::cout << "" << std::endl;
    }
  }
  if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos) {
    selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
    suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
    //std::cout << "abc3" << std::endl;
  } else if (t_queryInput.find(delimiterSuchThat) == std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos) {
    selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
    patternStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
    //std::cout << "abc4" << std::endl;
  } else {
     selectStatement = t_queryInput;
     //std::cout << "abc5" << std::endl;
     //std::cout << selectStatement << std::endl;
  }

  //std::cout << "This is the selectStatement: " << selectStatement << std::endl;
  //std::cout << patternStatement << ": check patternstatement" << std::endl;

  //parsing selectStatement: this code will only work for iteration 1. use find_first_of for future iterations
  std::string synonym = selectStatement.substr(selectStatement.find(" "), selectStatement.size());
  //std::cout << synonym << " before trimming " << std::endl;
  synonym = m_stringUtil.trimString(synonym);
  //std::cout << synonym << " after trimming " << std::endl;
  
  //std::cout << "before adding anything into selectqueue: " << m_selectQueue.size() << std::endl;
  //storing select queue synonyms
  int counterM = 0;
  for (auto m = m_grammarVector.begin(); m != m_grammarVector.end(); m++, counterM++) {
    Grammar g1 = m_grammarVector.at(counterM);
    std::string grammarName = g1.getName();
    //std::cout << grammarName << " this is the grammarName" << std::endl;
    if (grammarName == synonym) {
        m_selectQueue.push(g1);
        //std::cout << "This is select queue size currently: " << m_selectQueue.size() << std::endl;
        //std::cout << "pushed " << grammarName << " into select queue" << std::endl;
      }
      else {
        //std::cout << "skip" << std::endl;
      }
    }
  //std::cout << "This is select queue size: " << m_selectQueue.size() << std::endl;
  Grammar selectGrammar = m_selectQueue.front();
  //std::cout << "Select queue front Grammar name: " << selectGrammar.getName() << std::endl;

  //if design abstraction object does not exist
  if (suchThatStatement == "") {
    std::cout << "suchthatStatement is empty" << std::endl;
  } 

  //if design abstraction object exists, create designabstractionobject with parameters type, g1 g2
  else {
    std::string delimiterDesignAbstraction = "such that";
    std::string designAbstractions = suchThatStatement.substr(suchThatStatement.find(delimiterDesignAbstraction) + 10, suchThatStatement.size()); //"such that " is 10 characters
                                                                                                                                                  //IMPT: to be worked on in the future for more complex queries
    designAbstractions = m_stringUtil.trimString(designAbstractions);

    std::string delimiterBracket = "(";
    std::string designAbstractionEntity = designAbstractions.substr(0, designAbstractions.find(delimiterBracket));
    std::string designAbstractionObject = designAbstractions.substr(designAbstractions.find(delimiterBracket), designAbstractions.size());

    designAbstractionEntity = m_stringUtil.trimString(designAbstractionEntity);
    designAbstractionObject = m_stringUtil.trimString(designAbstractionObject);

    //std::cout << designAbstractionEntity << std::endl;

    int pos1;
    int prev_pos_new = 0;
    std::vector<std::string> designAbstractionVectorNew;
    while ((pos1 = designAbstractionObject.find_first_of("() ,;\\", prev_pos_new)) != std::string::npos) {
      if (pos1 > prev_pos_new) {
        designAbstractionVectorNew.push_back(designAbstractionObject.substr(prev_pos_new, pos1 - prev_pos_new));
      }
      prev_pos_new = pos1 + 1;
    }
    if (prev_pos_new < designAbstractionObject.length()) {
      designAbstractionVectorNew.push_back(designAbstractionObject.substr(prev_pos_new, std::string::npos));
    }

    //test method to print vector string values
    //printVector(designAbstractionVectorNew);

    std::string sTName1 = designAbstractionVectorNew.front();
    std::string sTName2 = designAbstractionVectorNew.back();

    //std::cout << sTName1 << " This is sTName1 " << std::endl;
    //std::cout << sTName2 << " This is sTName2 " << std::endl;

    int sTInt1 = 0;
    int sTInt2 = 0;

    std::stringstream convert(sTName1);
    if (!(convert >> sTInt1)) {
      sTInt1 = 0;
    }
    std::stringstream convert2(sTName2);
    if (!(convert2 >> sTInt2)) {
      sTInt2 = 0;
    }

    int counterK = 0;
    
    //storing designabstractionqueue synonyms
   
    //case: both synonyms are the same. e.g Follows(s, s)
    if (sTInt1 == sTInt2 || sTName1 == sTName2) {
      //return empty list
      std::cout << "return an empty list of strings" << std::endl;
    }
    else {
      for (auto k = m_grammarVector.begin(); k != m_grammarVector.end(); k++, counterK++) {
        Grammar tempGrammar = m_grammarVector.at(counterK);
        std::string grammarName = tempGrammar.getName();

        if (sTName1 == grammarName) {
          Grammar g1 = tempGrammar;
          //std::cout << "created new grammar1 object: " << g1.getName() << std::endl;
          if (sTName2 == grammarName) {
            Grammar g2 = tempGrammar;
            //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
            DesignAbstraction DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
          else if (sTInt2 > 0) {
            Grammar g2(8, sTName2);
            //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
            DesignAbstraction DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
          else if (sTName2.find("\\")) {
            //sTName2.erase(std::remove(sTName2.begin(), sTName2.end(), '\"'), sTName2.end());
            removeCharsFromString(sTName2, "\\\"");
            Grammar g2(6, sTName2);
            DesignAbstraction DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
        }
        else if (sTInt1 > 0) {
          Grammar g1(8, sTName1);
          //std::cout << "created new grammar1 object: " << g1.getName() << std::endl;
          if (sTName2 == grammarName) {
            Grammar g2 = tempGrammar;
            //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
            DesignAbstraction DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
          else if (sTInt2 > 0) {
            Grammar g2(8, sTName2);
            //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
            DesignAbstraction DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
          else if (sTName2.find("\\")) {
            //sTName2.erase(std::remove(sTName2.begin(), sTName2.end(), '\"'), sTName2.end());
            removeCharsFromString(sTName2, "\\\"");
            Grammar g2(6, sTName2);
            DesignAbstraction DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
        }
        else if (sTName1.find("\\")) {
          //sTName1.erase(std::remove(sTName1.begin(), sTName1.end(), '\"'), sTName1.end());
          removeCharsFromString(sTName2, "\\\"");
          Grammar g1(6, sTName1);
          if (sTName2 == grammarName) {
            Grammar g2 = tempGrammar;
            //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
            DesignAbstraction DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
          else if (sTInt2 > 0) {
            Grammar g2(8, sTName2);
            //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
            DesignAbstraction DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
          else if (sTName2.find("\\")) {
            //sTName2.erase(std::remove(sTName2.begin(), sTName2.end(), '\"'), sTName2.end());
            removeCharsFromString(sTName2, "\\\"");
            Grammar g2(6, sTName2);
            DesignAbstraction DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
        }
      }
      //DesignAbstraction class to be discussed: only caters for single DAO
      //test whether designabstractionobject is created properly
      /**DesignAbstraction object = m_suchThatQueue.front();
      Grammar g10 = object.getG1();
      Grammar g20 = object.getG2();
      std::cout << g10.getName() << std::endl;
      std::cout << g20.getName() << std::endl;

      Grammar getSelectGrammar = m_selectQueue.front();
      std::cout << getSelectGrammar.getName() << std::endl;*/
    }
  }
    
  //creation of pattern object
  if (patternStatement == "") {
    std::cout << "pattern statement is empty" << std::endl;
  }
  else {
    std::string delimiterSpace = " ";
    std::string patternEntity = patternStatement.substr(0, patternStatement.find(delimiterSpace));
    std::string patternObject = patternStatement.substr(patternStatement.find(delimiterSpace), patternStatement.size());

    //std::cout << patternEntity << std::endl;
    //std::cout << patternObject << std::endl;

    patternObject = m_stringUtil.trimString(patternObject);

    //std::cout << patternObject << std::endl;

    std::string delimiterBracket1 = "(";
    std::string patternSynonym = patternObject.substr(0, patternObject.find(delimiterBracket1));
    std::string patternParameters = patternObject.substr(patternObject.find(delimiterBracket1), patternObject.size());

    //std::cout << patternSynonym << std::endl;

    std::vector<std::string> patternVector;
    
    int pos2;
    int prev_pos_2 = 0;
    while ((pos2 = patternParameters.find_first_of("() ,;", prev_pos_2)) != std::string::npos) {
      if (pos2 > prev_pos_2) {
        patternVector.push_back(patternParameters.substr(prev_pos_2, pos2 - prev_pos_2));
      }
      prev_pos_2 = pos2 + 1;
    }
    if (prev_pos_2 < patternParameters.length()) {
      patternVector.push_back(patternParameters.substr(prev_pos_2, std::string::npos));
    }
    //printVector(patternVector);
  }

  std::queue<Grammar> selectQueue = getSelect();
  //std::cout << "This is select queue size: " << selectQueue.size() << std::endl;
  std::queue<DesignAbstraction> daoQueue = getSuchThat();
  //std::cout << "This is relation queue size: " << selectQueue.size() << std::endl;

  isTokenized = true;
  return isTokenized;
}

void QueryPreProcessor::removeCharsFromString(std::string &str, char* charsToRemove) {
  for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
    str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
  }
}

void QueryPreProcessor::printVector(std::vector<std::string>declarationVector) {
  for (auto i = declarationVector.begin(); i != declarationVector.end(); ++i) {
    std::cout << *i << ":  vectorPrinter" << std::endl;
  }
}