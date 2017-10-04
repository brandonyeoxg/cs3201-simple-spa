#include "QueryPreProcessor.h"

std::string QueryPreProcessor::PROCEDURE = "procedure";
std::string QueryPreProcessor::STMTLST = "stmtLst";
std::string QueryPreProcessor::STMT = "stmt";
std::string QueryPreProcessor::ASSIGN = "assign";
std::string QueryPreProcessor::WHILE = "while";
std::string QueryPreProcessor::IF = "if";
std::string QueryPreProcessor::VARIABLE = "variable";
std::string QueryPreProcessor::CONSTANT = "constant";
std::string QueryPreProcessor::PROG_LINE = "prog_line";
std::string QueryPreProcessor::BOOLEAN = "BOOLEAN";

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
        if (entity == PROCEDURE) {
          Grammar g(queryType::GType::PROC, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == STMTLST) {
          Grammar g(queryType::GType::ST_LST, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == STMT) {
          Grammar g(queryType::GType::STMT, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == ASSIGN) {
          Grammar g(queryType::GType::ASGN, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == WHILE) {
          Grammar g(queryType::GType::WHILE, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == IF) {
          Grammar g(queryType::GType::IF, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == VARIABLE) {
          Grammar g(queryType::GType::VAR, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == CONSTANT) {
          Grammar g(queryType::GType::CONST, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == PROG_LINE) {
          Grammar g(queryType::GType::PROG_LINE, variableVector.at(counterL));
          m_grammarVector.push_back(g);
        } else if (entity == BOOLEAN) {
          Grammar g(queryType::GType::BOOLEAN, variableVector.at(counterL));
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
    } else if (synonym == BOOLEAN) {
      m_selectQueue.push(g1);
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
    std::vector<std::string> designAbstractionVectorNew;

    designAbstractionVectorNew = stringVectorTokenizer("() ,;\\", designAbstractionObject, designAbstractionVectorNew);

    //test method to print vector string values

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

    //Checks if Uses/Modifies contains strings as the first parameter and return false if true
    if (designAbstractionEntity == "Uses" && sTName1.find('"') != std::string::npos
      || designAbstractionEntity == "Modifies" && sTName1.find('"') != std::string::npos) {
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

    Grammar g1;
    Grammar g2;
   
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
          g1 = tempGrammar;

          //Checks if Parent contains assignment statements for the first parameter
          if (designAbstractionEntity == "Parent" && g1.getType() == queryType::GType::ASGN
            || designAbstractionEntity == "Parent*" && g1.getType() == queryType::GType::ASGN) {
            return false;
          }

          //Checks if Follow/Follows* contains variables in their parameters and return false if tue
          if (designAbstractionEntity == "Follows" && g1.getType() == queryType::GType::VAR
            || designAbstractionEntity == "Follows*" && g1.getType() == queryType::GType::VAR) {
            return false;
          }

          //Checks if Uses/Modifies contains variables as the first parameter or statements in their 2nd parameter and return false if true
          if (designAbstractionEntity == "Uses" && g1.getType() == queryType::GType::VAR
            || designAbstractionEntity == "Modifies" && g1.getType() == queryType::GType::VAR) {
            return false;
          }

          //Checks if Calls contains the correct parameters: Procedure condition
          if (designAbstractionEntity == "Calls" && g1.getType() != queryType::GType::PROC
            || designAbstractionEntity == "Calls*" && g1.getType() != queryType::GType::PROC) {
            return false;
          }

          //Checks if Next contains the correct parameters
          if (designAbstractionEntity == "Next" && g1.getType() != queryType::GType::STMT
            || designAbstractionEntity == "Next" && g1.getType() != queryType::GType::ASGN
            || designAbstractionEntity == "Next" && g1.getType() != queryType::GType::WHILE
            || designAbstractionEntity == "Next" && g1.getType() != queryType::GType::IF
            || designAbstractionEntity == "Next" && g1.getType() != queryType::GType::CALL
            || designAbstractionEntity == "Next" && g1.getType() != queryType::GType::PROG_LINE) {
            return false;
          }

          //Checks if Next* contains the correct parameters
          if (designAbstractionEntity == "Next*" && g1.getType() != queryType::GType::STMT
            || designAbstractionEntity == "Next*" && g1.getType() != queryType::GType::ASGN
            || designAbstractionEntity == "Next*" && g1.getType() != queryType::GType::WHILE
            || designAbstractionEntity == "Next*" && g1.getType() != queryType::GType::IF
            || designAbstractionEntity == "Next*" && g1.getType() != queryType::GType::CALL
            || designAbstractionEntity == "Next*" && g1.getType() != queryType::GType::PROG_LINE) {
            return false;
          }

          //check is any design abstraction synonyms contains constant c
          if (g1.getType() == queryType::GType::CONST) {
            return false;
          }

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
              g2 = tempGrammar2;

              //Checks if Calls contains the correct parameters: procedure condition
              if (designAbstractionEntity == "Calls" && g2.getType() != queryType::GType::PROC
                || designAbstractionEntity == "Calls*" && g2.getType() != queryType::GType::PROC) {
                return false;
              }

              //Checks if Next contains the correct parameters
              if (designAbstractionEntity == "Next" && g2.getType() != queryType::GType::STMT
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::ASGN
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::WHILE
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::IF
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::CALL
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::PROG_LINE) {
                return false;
              }

              //Checks if Next* contains the correct parameters
              if (designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::STMT
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::ASGN
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::WHILE
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::IF
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::CALL
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::PROG_LINE) {
                return false;
              }

              //Checks if Parent contains variables in their parameters and return false if true
              if (designAbstractionEntity == "Parent" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Parent*" && g2.getType() == queryType::GType::VAR ) {
                return false;
              }

              //Checks if Follow/Follows* contains variables in their parameters and return false if tue
              if (designAbstractionEntity == "Follows" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Follows*" && g2.getType() == queryType::GType::VAR) {
                return false;
              }
              
              //Checks if Uses/Modifies contains statements in their 2nd parameter and return false if true
              if (designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::STMT
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::ASGN
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::WHILE
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::PROG_LINE
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::STMT
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::ASGN
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::WHILE
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::PROG_LINE) {
                return false;
              }

              //check is any design abstraction synonyms contains constant c
              if (g1.getType() == queryType::GType::CONST || g2.getType() == queryType::GType::CONST) {
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

              //Checks if Calls contains the correct parameters: no numbers
              if (designAbstractionEntity == "Calls"
                || designAbstractionEntity == "Calls*") {
                return false;
              }

              g2 = Grammar (queryType::GType::STMT_NO, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;

            } else if (sTName2.find('"') != std::string::npos) {

              //Checks if Next/Next* contains the correct parameters: no string
              if (designAbstractionEntity == "Next"
                || designAbstractionEntity == "Next*") {
                return false;
              }

              removeCharsFromString(sTName2, "\\\"");
              g2 = Grammar (queryType::GType::STR, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2 == "_") {
              g2 = Grammar (queryType::GType::STR, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            }
          }
        } else if (sTInt1 > 0) {
          counterQ = 0;

          //Checks if Calls contains the correct parameters: no numbers
          if (designAbstractionEntity == "Calls"
            || designAbstractionEntity == "Calls*") {
            return false;
          }

          g1 = Grammar (queryType::GType::STMT_NO, sTName1);
          for (auto q = m_grammarVector.begin(); q != m_grammarVector.end(); q++, counterQ++) {
            Grammar tempGrammar2 = m_grammarVector.at(counterQ);
            std::string grammarName2 = tempGrammar2.getName();
            if (sTName2 == grammarName2) {
              g2 = tempGrammar2;

              //Checks if Next contains the correct parameters
              if (designAbstractionEntity == "Next" && g2.getType() != queryType::GType::STMT
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::ASGN
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::WHILE
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::IF
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::CALL
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::PROG_LINE) {
                return false;
              }

              //Checks if Next* contains the correct parameters
              if (designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::STMT
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::ASGN
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::WHILE
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::IF
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::CALL
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::PROG_LINE) {
                return false;
              }

              //Checks if Parent and Parent* contains the correct parameters
              if (designAbstractionEntity == "Parent" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Parent*" && g2.getType() == queryType::GType::VAR) {
                return false;
              }

              //Checks if Follow/Follows* contains variables in their parameters and return false if tue
              if (designAbstractionEntity == "Follows" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Follows*" && g2.getType() == queryType::GType::VAR) {
                return false;
              }

              //Checks if Uses/Modifies contains statements in their 2nd parameter and return false if true
              if (designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::STMT
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::ASGN
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::WHILE
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::PROG_LINE
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::STMT
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::ASGN
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::WHILE
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::PROG_LINE) {
                return false;
              }

              //check is any design abstraction synonyms contains constant c
              if (g1.getType() == queryType::GType::CONST || g2.getType() == queryType::GType::CONST) {
                return false;
              }
              //check is any design abstraction synonyms contains constant c
              if (g1.getType() == queryType::GType::CONST || g2.getType() == queryType::GType::CONST) {
                return false;
              }

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
              g2 =  Grammar(queryType::GType::STMT_NO, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2.find('"') != std::string::npos) {

              //Checks if Next/Next* contains the correct parameters: no string
              if (designAbstractionEntity == "Next"
                || designAbstractionEntity == "Next*") {
                return false;
              }

              removeCharsFromString(sTName2, "\\\"");
              g2 = Grammar(queryType::GType::STR, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2 == "_") {
              g2 = Grammar(queryType::GType::STR, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } 
          }
        } else if (sTName1.find('"') != std::string::npos) {

          //Checks if Next/Next* contains the correct parameters: no string
          if (designAbstractionEntity == "Next"
            || designAbstractionEntity == "Next*") {
            return false;
          }

          removeCharsFromString(sTName1, "\\\"");
          counterQ = 0;
          g1 = Grammar (queryType::GType::STR, sTName1);
          for (auto q = m_grammarVector.begin(); q != m_grammarVector.end(); q++, counterQ++) {
            Grammar tempGrammar2 = m_grammarVector.at(counterQ);
            std::string grammarName2 = tempGrammar2.getName();
            if (sTName2 == grammarName2) {
              g2 = tempGrammar2;

              //Checks if Next contains the correct parameters
              if (designAbstractionEntity == "Next" && g2.getType() != queryType::GType::STMT
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::ASGN
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::WHILE
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::IF
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::CALL
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::PROG_LINE) {
                return false;
              }

              //Checks if Next* contains the correct parameters
              if (designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::STMT
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::ASGN
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::WHILE
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::IF
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::CALL
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::PROG_LINE) {
                return false;
              }

              //Checks if Calls contains the correct parameters: procedure condition
              if (designAbstractionEntity == "Calls" && g2.getType() != queryType::GType::PROC
                || designAbstractionEntity == "Calls*" && g2.getType() != queryType::GType::PROC) {
                return false;
              }

              //Checks if Parent/Parent* contains variables in their parameters and return false if tue
              if (designAbstractionEntity == "Parent" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Parent*" && g2.getType() == queryType::GType::VAR) {
                return false;
              }

              //Checks if Follow/Follows* contains variables in their parameters and return false if tue
              if (designAbstractionEntity == "Follows" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Follows*" && g2.getType() == queryType::GType::VAR) {
                return false;
              }

              //Checks if Uses/Modifies contains statements in their 2nd parameter and return false if true
              if (designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::STMT
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::ASGN
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::WHILE
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::PROG_LINE
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::STMT
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::ASGN
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::WHILE
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::PROG_LINE) {
                return false;
              }

              //check is any design abstraction synonyms contains constant c
              if (g1.getType() == queryType::GType::CONST || g2.getType() == queryType::GType::CONST) {
                return false;
              }

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

              //Checks if Calls contains the correct parameters: no numbers
              if (designAbstractionEntity == "Calls"
                || designAbstractionEntity == "Calls*") {
                return false;
              }

              g2 = Grammar (queryType::GType::STMT_NO, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2.find('"') != std::string::npos) {

              //Checks if Next/Next* contains the correct parameters: no string
              if (designAbstractionEntity == "Next"
                || designAbstractionEntity == "Next*") {
                return false;
              }

              removeCharsFromString(sTName2, "\\\"");
              g2 = Grammar (queryType::GType::STR, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2 == "_") {
              g2 = Grammar (queryType::GType::STR, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            }
          }
        } else if (sTName1 == "_") {
          if (designAbstractionEntity == "Uses" || designAbstractionEntity == "Modifies") {
            return false;
          }
          counterQ = 0;
          g1 = Grammar (queryType::GType::STR, sTName1);
          for (auto q = m_grammarVector.begin(); q != m_grammarVector.end(); q++, counterQ++) {
            Grammar tempGrammar2 = m_grammarVector.at(counterQ);
            std::string grammarName2 = tempGrammar2.getName();
            if (sTName2 == grammarName2) {
              g2 = tempGrammar2;

              //Checks if Next contains the correct parameters
              if (designAbstractionEntity == "Next" && g2.getType() != queryType::GType::STMT
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::ASGN
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::WHILE
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::IF
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::CALL
                || designAbstractionEntity == "Next" && g2.getType() != queryType::GType::PROG_LINE) {
                return false;
              }

              //Checks if Next* contains the correct parameters
              if (designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::STMT
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::ASGN
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::WHILE
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::IF
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::CALL
                || designAbstractionEntity == "Next*" && g2.getType() != queryType::GType::PROG_LINE) {
                return false;
              }

              //Checks if Calls contains the correct parameters: procedure condition
              if (designAbstractionEntity == "Calls" && g2.getType() != queryType::GType::PROC
                || designAbstractionEntity == "Calls*" && g2.getType() != queryType::GType::PROC) {
                return false;
              }

              //Checks if Parent contains variables in their parameters and return false if true
              if (designAbstractionEntity == "Parent" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Parent*" && g2.getType() == queryType::GType::VAR) {
                return false;
              }

              //Checks if Follow/Follows* contains variables in their parameters and return false if tue
              if (designAbstractionEntity == "Follows" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Follows*" && g2.getType() == queryType::GType::VAR) {
                return false;
              }

              //Checks if Uses/Modifies contains statements in their 2nd parameter and return false if true
              if (designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::STMT
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::ASGN
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::WHILE
                || designAbstractionEntity == "Uses" && g2.getType() == queryType::GType::PROG_LINE
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::STMT
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::ASGN
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::WHILE
                || designAbstractionEntity == "Modifies" && g2.getType() == queryType::GType::PROG_LINE) {
                return false;
              }

              //check is any design abstraction synonyms contains constant c
              if (g1.getType() == queryType::GType::CONST || g2.getType() == queryType::GType::CONST) {
                return false;
              }

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

              //Checks if Calls contains the correct parameters: no numbers
              if (designAbstractionEntity == "Calls"
                || designAbstractionEntity == "Calls*") {
                return false;
              }

              g2 = Grammar(queryType::GType::STMT_NO, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2.find('"') != std::string::npos) {

              //Checks if Next/Next* contains the correct parameters: no string
              if (designAbstractionEntity == "Next"
                || designAbstractionEntity == "Next*") {
                return false;
              }

              removeCharsFromString(sTName2, "\\\"");
              g2 = Grammar(queryType::GType::STR, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            } else if (sTName2 == "_") {
              g2 = Grammar(queryType::GType::STR, sTName2);
              Relation DAO(designAbstractionEntity, g1, g2);
              m_suchThatQueue.push(DAO);
              break;
            }   
          }
        }
      }
      //Relation DAO(designAbstractionEntity, g1, g2);
      //m_suchThatQueue.push(DAO);
    }
    if (m_suchThatQueue.size() == 0) {
      return false;
    }
  }
    
  //creation of pattern object
  if (patternStatement == "") {
    std::cout << "pattern statement is empty" << std::endl;
  } else {
    std::string delimiterSpace = " ";
    std::string patternEntity = patternStatement.substr(0, patternStatement.find(delimiterSpace));
    std::string patternObject = patternStatement.substr(patternStatement.find(delimiterSpace), patternStatement.size());

    patternObject = m_stringUtil.trimString(patternObject);

    std::cout << patternObject << std::endl;

    std::string delimiterBracket1 = "(";
    std::string patternSynonym = patternObject.substr(0, patternObject.find(delimiterBracket1));
    std::string patternParameters = patternObject.substr(patternObject.find(delimiterBracket1), patternObject.size());

    //std::cout << patternSynonym << std::endl;
    patternSynonym = m_stringUtil.trimString(patternSynonym);

    std::vector<std::string> patternVector;
    
    patternVector = stringVectorTokenizer("(),;", patternParameters, patternVector);

    int counterP = 0;
    Grammar patternOfGrammar;
    Grammar grammarPatternLeft;
    Grammar grammarPatternRight;
    bool isSubTree = false;
    int counterR = 0;
    std::vector<std::string> counterVect;
    for (auto r = m_grammarVector.begin(); r != m_grammarVector.end(); r++, counterR++) {
      Grammar tempPatternGrammar = m_grammarVector.at(counterR);
      std::string tempPatternGrammarString = tempPatternGrammar.getName();
      if (patternSynonym == tempPatternGrammarString) {
        patternOfGrammar = tempPatternGrammar;
        counterVect.push_back(tempPatternGrammarString);
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(patternSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ patternSynonym, 1 });
        } else {
          m_synonymMap[patternSynonym]++;
        }
      } 
    }
    if (counterVect.size() == 0) {
      return false;
    }
    std::string patternLeftName = patternVector.front();
    if (patternLeftName.find('"') != std::string::npos) {
      removeCharsFromString(patternLeftName, "\\\" ");
      grammarPatternLeft = Grammar(queryType::GType::STR, patternLeftName);
    }
    //to work on: other forms of pattern
    else if (patternLeftName == "_") {
      removeCharsFromString(patternLeftName, "\"");
      grammarPatternLeft = Grammar(queryType::GType::STR, patternLeftName);
    } else {
      int counterT = 0;
      removeCharsFromString(patternLeftName, "\\\" ");
      std::vector<std::string> counterVector;
      for (auto t = m_grammarVector.begin(); t != m_grammarVector.end(); t++, counterT++) {
        Grammar tempPatternGrammarTemp = m_grammarVector.at(counterT);
        std::string tempPatternGrammarStringTemp = tempPatternGrammarTemp.getName();
        if (patternLeftName == tempPatternGrammarStringTemp) {
          grammarPatternLeft = tempPatternGrammarTemp;
          tempSynonymVector.push_back(grammarPatternLeft.getName());
          counterVector.push_back(grammarPatternLeft.getName());
          //created all entries to be pushed into vector. to work on parsing thru to add into map
          std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(patternLeftName);
          if (got == m_synonymMap.end()) {
            m_synonymMap.insert({ patternLeftName, 1 });
          } else {
            m_synonymMap[patternLeftName]++;
          }
        }
      }
      if (counterVector.size() == 0) {
        return false;
      }   
    }
    //to check whether left side can have variables or not. 11/9/2017 5pm
    std::string patternRightName = patternVector.back();
    patternRightName = m_stringUtil.trimString(patternRightName);
    if ((patternRightName.find('"') != std::string::npos) && patternRightName.front() == '_' && patternRightName.back() == '_') {
      removeCharsFromString(patternRightName, "\\\" _");
      grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
      isSubTree = true;
    } else if (patternRightName.find('"') != std::string::npos && patternRightName.front() != '_' && patternRightName.back() != '_') {
      removeCharsFromString(patternRightName, "\\\" ");
      grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
    } else if (patternRightName == "_") {
      //invalid
      grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
    } else {
      return false;
    }

    //todo: else condition for outputting invalid input

    //create pattern object
    Pattern patternObjectCreated(patternOfGrammar, grammarPatternLeft, grammarPatternRight, isSubTree);
    m_patternQueue.push(patternObjectCreated);
    
    }

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

std::vector<std::string> QueryPreProcessor::stringVectorTokenizer(char* charsToRemove, std::string targetString, std::vector<std::string> vector) {
  int pos1;
  int prev_pos_new = 0;

  while ((pos1 = targetString.find_first_of(charsToRemove, prev_pos_new)) != std::string::npos) {
    if (pos1 > prev_pos_new) {
      vector.push_back(targetString.substr(prev_pos_new, pos1 - prev_pos_new));
    }
    prev_pos_new = pos1 + 1;
  }
  if (prev_pos_new < targetString.length()) {
    vector.push_back(targetString.substr(prev_pos_new, std::string::npos));
  }
  return vector;
}