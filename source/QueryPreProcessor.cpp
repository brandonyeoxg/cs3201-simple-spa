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

std::queue<Relation> QueryPreProcessor::getSuchThat(void) {
  return m_suchThatQueue;
}

std::queue<Pattern> QueryPreProcessor::getPattern(void) {
  return m_patternQueue;
}

std::unordered_map<std::string, int> QueryPreProcessor::getSynonym(void) {
  return m_synonymMap;
}

std::vector<Grammar> QueryPreProcessor::getGrammarVector(void) {
  return m_grammarVector;
}

std::string QueryPreProcessor::splitStringDeclaration(std::string t_Input) {
  std::string delimiter = "Select";
  std::string declaration = t_Input.substr(0, t_Input.find(delimiter));

  declaration = m_stringUtil.trimString(declaration);
  return declaration;
}

std::string QueryPreProcessor::splitStringQuery(std::string t_Input) {
  std::string delimiter = "Select";
  std::string query = t_Input.substr(t_Input.find(delimiter), t_Input.size()); //same for this as delimiter is "; Select"

  query = m_stringUtil.trimString(query);
  return query;
}


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
    tempString = m_stringUtil.trimString(tempString);
    //std::cout << tempString << " testing" << std::endl;
    std::string entity = tempString.substr(0, tempString.find(delimiterSpace));
    std::string variables = tempString.substr(tempString.find(delimiterSpace) + 1, tempString.size()); //same for this as delimiter is "; Select" variables split individually

    entity = m_stringUtil.trimString(entity);
    variables = m_stringUtil.trimString(variables);
    //Checks whether there is no entity for the synonym e.g stmt s; s1; variable v;
    if (entity == variables) {
      return false;
    }

    //Check if string is properly named: s, s1, s#..
    char cBack;
    char cFront;
    cFront = variables.front();
    cBack = variables.back();
    if (isalpha(cFront) && isalnum(cBack)
      || isalpha(cFront) && cBack == '#') {
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

      //std::cout << "Size of variable vector: " << variableVector.size() << std::endl;

      int counterL = 0;
      for (auto l = variableVector.begin(); l != variableVector.end(); l++, counterL++) {
        if (entity == "procedure") {
          Grammar g(m_procedure, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == "stmtLst") {
          Grammar g(m_statementList, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == "stmt") {
          Grammar g(m_statement, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == "assign") {
          Grammar g(m_assign, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == "while") {
          Grammar g(m_while, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == "if") {
          Grammar g(m_if, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == "variable") {
          Grammar g(m_variable, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == "constant") {
          Grammar g(m_constant, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == "prog_line") {
          Grammar g(m_progline, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else {
          //do nothing
        }
      }
    } else {
      return false;
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
  std::vector<std::string> tempSynonymVector;
  int mapValue = 1;
  int mapDefaultValue = 1;

  //Check whether semicolon is at the end of the query
  if (t_queryInput.back() == ';') {
    return false;
  }

  if (isContainsOne("such", t_queryInput) == false) {
    return false;
  }

  if (isContainsOne("that", t_queryInput) == false) {
    return false;
  }

  if (isContainsOne("pattern", t_queryInput) == false) {
    return false;
  }

  if (t_queryInput.find("and") != std::string::npos) {
    return false;
  }

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
  } else if (t_queryInput.find(delimiterSuchThat) == std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos) {
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
  }

  //Checks if the select statement synonym is not declared
  if (m_selectQueue.size() == 0) {
    return false;
  }

  //std::cout << "This is select queue size: " << m_selectQueue.size() << std::endl;
  Grammar selectGrammar = m_selectQueue.front();
  m_synonymMap.insert({ selectGrammar.getName(), 1});
  //std::cout << "Select queue front Grammar name: " << selectGrammar.getName() << std::endl;

  //if design abstraction object does not exist
  if (suchThatStatement == "") {
    std::cout << "suchthatStatement is empty" << std::endl;
  }
  //if design abstraction object exists, create Relation object with parameters type, g1 g2
  else {
    //Trim whitespaces for such that statement.
    suchThatStatement = m_stringUtil.trimString(suchThatStatement);
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

    //Check if design abstraction parameter one contains "" and has design entity as below
    if (sTName1.find('"') != std::string::npos && designAbstractionEntity == "Follows"
      || sTName1.find('"') != std::string::npos && designAbstractionEntity == "Follows*"
      || sTName1.find('"') != std::string::npos && designAbstractionEntity == "Parent"
      || sTName1.find('"') != std::string::npos && designAbstractionEntity == "Parent*") {

      return false;
    }

    //Check if design abstraction parameter two contains "" and has design entity as below
    if (sTName2.find('"') != std::string::npos && designAbstractionEntity == "Follows"
      || sTName2.find('"') != std::string::npos && designAbstractionEntity == "Follows*"
      || sTName2.find('"') != std::string::npos && designAbstractionEntity == "Parent"
      || sTName2.find('"') != std::string::npos && designAbstractionEntity == "Parent*") {

      return false;
    }

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
    int counterQ = 0;
    
    //storing designabstractionqueue synonyms
   
    //case: both synonyms are the same. e.g Follows(s, s)
    if ((sTInt1 == sTInt2 && sTInt1 != 0) || (sTName1 == sTName2 && sTName1 != "_")) {
      //return empty list
      std::cout << "return an empty list of strings" << std::endl;
      return false;
    }
    else {
      for (auto k = m_grammarVector.begin(); k != m_grammarVector.end(); k++, counterK++) {
        Grammar tempGrammar = m_grammarVector.at(counterK);
        std::string grammarName = tempGrammar.getName();
        
        counterQ = 0;
        if (m_suchThatQueue.size() == 1) {
          break;
        }
        if (sTName1 == grammarName) {
          Grammar g1 = tempGrammar;
          tempSynonymVector.push_back(g1.getName());
          std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(sTName1);
          if (got == m_synonymMap.end()) {
            m_synonymMap.insert({ sTName1, 1 });
          } else {
            m_synonymMap[sTName1]++;
          }
          //std::cout << "created new grammar1 object: " << g1.getName() << std::endl;
          for (auto q = m_grammarVector.begin(); q != m_grammarVector.end(); q++, counterQ++) {
            Grammar tempGrammar2 = m_grammarVector.at(counterQ);
            std::string grammarName2 = tempGrammar2.getName();
            if (sTName2 == grammarName2) {
              Grammar g2 = tempGrammar2;

              //Checks if Parent contains variables in their parameters and return false if true
              if (designAbstractionEntity == "Parent" && g1.getType() == Grammar::GType::VAR
                || designAbstractionEntity == "Parent*" && g1.getType() == Grammar::GType::VAR
                || designAbstractionEntity == "Parent" && g2.getType() == Grammar::GType::VAR 
                || designAbstractionEntity == "Parent*" && g2.getType() == Grammar::GType::VAR ) {
                return false;
              }

              //Checks if Parent contains assignment statements for the first parameter
              if (designAbstractionEntity == "Parent" && g1.getType() == Grammar::GType::ASGN
                || designAbstractionEntity == "Parent*" && g1.getType() == Grammar::GType::ASGN) {
                return false;
              }

              //Checks if Follow/Follows* contains variables in their parameters and return false if tue
              if (designAbstractionEntity == "Follows" && g1.getType() == Grammar::GType::VAR
                || designAbstractionEntity == "Follows*" && g1.getType() == Grammar::GType::VAR
                || designAbstractionEntity == "Follows" && g2.getType() == Grammar::GType::VAR
                || designAbstractionEntity == "Follows*" && g2.getType() == Grammar::GType::VAR) {
                return false;
              }

              //Checks if Uses/Modifies contains variables as the first parameter or statements in their 2nd parameter and return false if true
              if (designAbstractionEntity == "Uses" && g1.getType() == Grammar::GType::VAR
                || designAbstractionEntity == "Uses" && g2.getType() == Grammar::GType::STMT
                || designAbstractionEntity == "Uses" && g2.getType() == Grammar::GType::ASGN
                || designAbstractionEntity == "Uses" && g2.getType() == Grammar::GType::WHILE
                || designAbstractionEntity == "Modifies" && g1.getType() == Grammar::GType::VAR
                || designAbstractionEntity == "Modifies" && g2.getType() == Grammar::GType::STMT
                || designAbstractionEntity == "Modifies" && g2.getType() == Grammar::GType::ASGN
                || designAbstractionEntity == "Modifies" && g2.getType() == Grammar::GType::WHILE) {
                return false;
              }

              //check is any design abstraction synonyms contains constant c
              if (g1.getType() == Grammar::GType::CONST || g2.getType() == Grammar::GType::CONST) {
                return false;
              }

              //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(sTName2);
              if (got == m_synonymMap.end()) {
                m_synonymMap.insert({ sTName2, 1 });
              } else {
                m_synonymMap[sTName2]++;
              }
              break;
            } else if (sTInt2 > 0) {
              Grammar g2(m_statementNumber, sTName2);
              //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2.find('"') != std::string::npos) {
              //sTName2.erase(std::remove(sTName2.begin(), sTName2.end(), '\"'), sTName2.end());
              removeCharsFromString(sTName2, "\\\"");
              Grammar g2(m_string, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2 == "_") {
              Grammar g2(m_string, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            }
          }
        } else if (sTInt1 > 0) {
          counterQ = 0;
          Grammar g1(m_statementNumber, sTName1);
          //std::cout << "created new grammar1 object: " << g1.getName() << std::endl;
          for (auto q = m_grammarVector.begin(); q != m_grammarVector.end(); q++, counterQ++) {
            Grammar tempGrammar2 = m_grammarVector.at(counterQ);
            std::string grammarName2 = tempGrammar2.getName();
            if (sTName2 == grammarName2) {
              Grammar g2 = tempGrammar2;
              //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(sTName2);
              if (got == m_synonymMap.end()) {
                m_synonymMap.insert({ sTName2, 1 });
              }
              else {
                m_synonymMap[sTName2]++;
              }
              break;
            } else if (sTInt2 > 0) {
              Grammar g2(m_statementNumber, sTName2);
              //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2.find('"') != std::string::npos) {
              //sTName2.erase(std::remove(sTName2.begin(), sTName2.end(), '\"'), sTName2.end());
              removeCharsFromString(sTName2, "\\\"");
              Grammar g2(m_string, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2 == "_") {
              Grammar g2(m_string, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } 
          }
        } else if (sTName1.find('"') != std::string::npos) {
          //sTName1.erase(std::remove(sTName1.begin(), sTName1.end(), '\"'), sTName1.end());
          removeCharsFromString(sTName1, "\\\"");
          counterQ = 0;
          Grammar g1(m_string, sTName1);
          for (auto q = m_grammarVector.begin(); q != m_grammarVector.end(); q++, counterQ++) {
            Grammar tempGrammar2 = m_grammarVector.at(counterQ);
            std::string grammarName2 = tempGrammar2.getName();
            if (sTName2 == grammarName2) {
              Grammar g2 = tempGrammar2;
              //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(sTName2);
              if (got == m_synonymMap.end()) {
                m_synonymMap.insert({ sTName2, 1 });
              } else {
                m_synonymMap[sTName2]++;
              }
              break;
            } else if (sTInt2 > 0) {
              Grammar g2(m_statementNumber, sTName2);
              //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2.find('"') != std::string::npos) {
              //sTName2.erase(std::remove(sTName2.begin(), sTName2.end(), '\"'), sTName2.end());
              removeCharsFromString(sTName2, "\\\"");
              Grammar g2(m_string, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2 == "_") {
              Grammar g2(m_string, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            }
          }
        } else if ((sTName1 == "_")) {
          if (designAbstractionEntity == "Uses" || designAbstractionEntity == "Modifies") {
            return false;
          }
          counterQ = 0;
          Grammar g1(m_string, sTName1);
          for (auto q = m_grammarVector.begin(); q != m_grammarVector.end(); q++, counterQ++) {
            Grammar tempGrammar2 = m_grammarVector.at(counterQ);
            std::string grammarName2 = tempGrammar2.getName();
            if (sTName2 == grammarName2) {
              Grammar g2 = tempGrammar2;
              //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(sTName2);
              if (got == m_synonymMap.end()) {
                m_synonymMap.insert({ sTName2, 1 });
              } else {
                m_synonymMap[sTName2]++;
              }
              break;
            } else if (sTInt2 > 0) {
              Grammar g2(m_statementNumber, sTName2);
              //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2.find('"') != std::string::npos) {
              //sTName2.erase(std::remove(sTName2.begin(), sTName2.end(), '\"'), sTName2.end());
              removeCharsFromString(sTName2, "\\\"");
              Grammar g2(m_string, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2 == "_") {
              
              Grammar g2(m_string, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            }
          }
        }
      }
      //DesignAbstraction class to be discussed: only caters for single DAO
      //test whether designabstractionobject is created properly
      /*Relation object = m_suchThatQueue.front();
      Grammar g10 = object.getG1();
      Grammar g20 = object.getG2();
      std::cout << g10.getName() << std::endl;
      std::cout << g20.getName() << std::endl;*/

      //Grammar getSelectGrammar = m_selectQueue.front();
      //std::cout << getSelectGrammar.getName() << std::endl;
      //std::cout << "This is suchThat queue size: " << m_suchThatQueue.size() << std::endl;
    }
  }
    
  //creation of pattern object
  if (patternStatement == "") {
    std::cout << "pattern statement is empty" << std::endl;
  } else {
    std::string delimiterSpace = " ";
    std::string patternEntity = patternStatement.substr(0, patternStatement.find(delimiterSpace));
    std::string patternObject = patternStatement.substr(patternStatement.find(delimiterSpace), patternStatement.size());

    //std::cout << patternEntity << std::endl;
    //std::cout << patternObject << std::endl;

    patternObject = m_stringUtil.trimString(patternObject);

    std::cout << patternObject << std::endl;

    std::string delimiterBracket1 = "(";
    std::string patternSynonym = patternObject.substr(0, patternObject.find(delimiterBracket1));
    std::string patternParameters = patternObject.substr(patternObject.find(delimiterBracket1), patternObject.size());

    //std::cout << patternSynonym << std::endl;
    patternSynonym = m_stringUtil.trimString(patternSynonym);

    std::vector<std::string> patternVector;
    
    int pos2;
    int prev_pos_2 = 0;
    while ((pos2 = patternParameters.find_first_of("(),;", prev_pos_2)) != std::string::npos) {
      if (pos2 > prev_pos_2) {
        patternVector.push_back(patternParameters.substr(prev_pos_2, pos2 - prev_pos_2));
      }
      prev_pos_2 = pos2 + 1;
    }
    if (prev_pos_2 < patternParameters.length()) {
      patternVector.push_back(patternParameters.substr(prev_pos_2, std::string::npos));
    }
    int counterP = 0;
    Grammar patternOfGrammar;
    Grammar grammarPatternLeft;
    Grammar grammarPatternRight;
    bool isSubTree = false;
    int counterR = 0;
    for (auto r = m_grammarVector.begin(); r != m_grammarVector.end(); r++, counterR++) {
      Grammar tempPatternGrammar = m_grammarVector.at(counterR);
      std::string tempPatternGrammarString = tempPatternGrammar.getName();
      if (patternSynonym == tempPatternGrammarString) {
        patternOfGrammar = tempPatternGrammar;
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(patternSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ patternSynonym, 1 });
        } else {
          m_synonymMap[patternSynonym]++;
        }
      }
    }
    std::string patternLeftName = patternVector.front();
    if (patternLeftName.find('"') != std::string::npos) {
      removeCharsFromString(patternLeftName, "\\\" ");
      grammarPatternLeft = Grammar(m_string, patternLeftName);
    }
    //to work on: other forms of pattern
    else if (patternLeftName == "_") {
      removeCharsFromString(patternLeftName, "\"");
      grammarPatternLeft = Grammar(m_string, patternLeftName);
    } else {
      int counterT = 0;
      removeCharsFromString(patternLeftName, "\\\" ");
      for (auto t = m_grammarVector.begin(); t != m_grammarVector.end(); t++, counterT++) {
        Grammar tempPatternGrammarTemp = m_grammarVector.at(counterT);
        std::string tempPatternGrammarStringTemp = tempPatternGrammarTemp.getName();
        if (patternLeftName == tempPatternGrammarStringTemp) {
          grammarPatternLeft = tempPatternGrammarTemp;
          tempSynonymVector.push_back(grammarPatternLeft.getName());
          //created all entries to be pushed into vector. to work on parsing thru to add into map
          std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(patternLeftName);
          if (got == m_synonymMap.end()) {
            m_synonymMap.insert({ patternLeftName, 1 });
          } else {
            m_synonymMap[patternLeftName]++;
          }
        }
      }
    }
    //to check whether left side can have variables or not. 11/9/2017 5pm
    std::string patternRightName = patternVector.back();
    patternRightName = m_stringUtil.trimString(patternRightName);
    if ((patternRightName.find('"') != std::string::npos) && patternRightName.front() == '_' && patternRightName.back() == '_') {
      removeCharsFromString(patternRightName, "\\\" _");
      grammarPatternRight = Grammar(m_string, patternRightName);
      isSubTree = true;
    } else if (patternRightName.find('"') != std::string::npos && patternRightName.front() != '_' && patternRightName.back() != '_') {
      removeCharsFromString(patternRightName, "\\\" ");
      grammarPatternRight = Grammar(m_string, patternRightName);
    } else if (patternRightName == "_") {
      //invalid
      grammarPatternRight = Grammar(m_string, patternRightName);
    }

    //todo: else condition for outputting invalid input

    //create pattern object
    Pattern patternObjectCreated(patternOfGrammar, grammarPatternLeft, grammarPatternRight, isSubTree);
    m_patternQueue.push(patternObjectCreated);
    
    //debugging statements
    /*std::cout << "this is grammarpatternleft name: " << grammarPatternLeft.getName() << std::endl;
    if (grammarPatternLeft.getType() == Grammar::GType::EXPR) {
      std::cout << "this is grammarpatternleft type: EXPR " << std::endl;
    }*/
    /*
    Pattern tempP = m_patternQueue.front();
    Grammar pGrammerStmt = tempP.getStmt();
    std::string pGrammerStmtString = pGrammerStmt.getName();
    Grammar pGrammerLeft = tempP.getLeft();
    std::string pGrammerLeftString = pGrammerLeft.getName();
    Grammar pGrammerRight = tempP.getRight();
    std::string pGrammerRightString = pGrammerRight.getName();
    
    std::cout << "This is pGrammerStmt: " << pGrammerStmtString << std::endl;
    std::cout << "This is pGrammerLeft: " << pGrammerLeftString << std::endl;
    std::cout << "This is pGrammerRight: " << pGrammerRightString << std::endl;
    printVector(patternVector);*/
    /*int counterU = 0;
    for (auto u = tempSynonymVector.begin(); u != tempSynonymVector.end(); u++, counterU++) {
      std::string tempSynonymString = m_grammarVector.at(counterU);
      std::string tempPatternGrammarStringTemp = tempPatternGrammarTemp.getName();
      if (patternLeftName == tempPatternGrammarStringTemp) {
        grammarPatternLeft = tempPatternGrammarTemp;
        tempSynonymVector.push_back(grammarPatternLeft.getName());
        //created all entries to be pushed into vector. to work on parsing thru to add into map
      }*/
    }

  //std::queue<Grammar> selectQueue = getSelect();
  //std::cout << "This is select queue size: " << selectQueue.size() << std::endl;
  //std::queue<Relation> daoQueue = getSuchThat();
  //std::cout << "This is relation queue size: " << daoQueue.size() << std::endl;

  isTokenized = true;
  return isTokenized;
}

void QueryPreProcessor::removeCharsFromString(std::string &str, char* charsToRemove) {
  for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
    str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
  }
}

void QueryPreProcessor::printVector(std::vector<std::string> t_vector) {
  for (auto i = t_vector.begin(); i != t_vector.end(); ++i) {
    std::cout <<"vector printing function: "<< *i << std::endl;
  }
}

bool QueryPreProcessor::isContainsOne(std::string toFind, std::string t_query) {
  int tempCounter = 0;
  bool isContains = true;
  //Check whether there exists more than one such that clause(check "such" > 2)
  for (size_t position = t_query.find(toFind, 0); position != std::string::npos; position =
    t_query.find(toFind, position + 1)) {
    tempCounter++;
    if (tempCounter > 1) {
      return false;
    }
  }
  return isContains;
}