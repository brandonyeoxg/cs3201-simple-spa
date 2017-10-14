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
std::string QueryPreProcessor::CALL = "call";
std::string QueryPreProcessor::PROCNAME = "procName";
std::string QueryPreProcessor::VARNAME = "varName";
std::string QueryPreProcessor::VALUE = "value";
std::string QueryPreProcessor::STMT_NO = "stmt#";

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
  if (t_declarationInput == "") {
    return true;
  } else {
    std::vector<std::string> declarationVector;
    //std::cout << t_declarationInput << "test1" << std::endl;

    std::string starterString;
    //tokens are split by ;
    std::size_t prev_pos = 0, pos;
    starterString = m_stringUtil.trimString(t_declarationInput);

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

    std::string delimiterSpace = " ";
    std::string tempString;

    for (std::size_t j = 0; j != declarationVector.size(); ++j) {
      tempString = declarationVector.at(j);
      tempString = m_stringUtil.trimString(tempString);

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
          } else if (entity == CALL) {
            Grammar g(queryType::GType::CALL, variableVector.at(counterL));
            m_grammarVector.push_back(g);
          } else {
            //do nothing
          }
        }
      } else {
        return false;
      }
    }
  }
  isTokenized = true;
  return isTokenized;
}

bool QueryPreProcessor::tokenizeQuery(std::string t_queryInput) {
  bool isTokenized = false;
  std::vector<std::string> queryVector;
  std::string selectStatement;
  std::string suchThatStatement;
  std::string patternStatement;
  std::string withStatement;
  std::string delimiterSelect = "Select";
  std::string delimiterSuchThat = "such that";
  std::string delimiterPattern = "pattern";
  std::string delimiterWith = "with";
  std::string tempStatement;
  std::string tempStatement1;
  std::string tempStatement2;
  std::string tempStatement3;
  std::vector<std::string> tempSynonymVector;
  int mapValue = 1;
  int mapDefaultValue = 1;

  //Check whether semicolon is at the end of the query
  if (t_queryInput.back() == ';') {
    return false;
  }

  //Cases with Select, such that, with, pattern clause
  if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos 
    && t_queryInput.find(delimiterWith) != std::string::npos) {

    //Case 1: Check clause ordering is: Select, such that, pattern, with
    if (t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterPattern)
      && t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterWith)
      && t_queryInput.find(delimiterPattern) < t_queryInput.find(delimiterWith)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      suchThatStatement = tempStatement.substr(tempStatement.find(delimiterSuchThat), tempStatement.find(delimiterPattern));
      tempStatement2 = tempStatement.substr(tempStatement.find(delimiterPattern), t_queryInput.size());
      patternStatement = tempStatement2.substr(tempStatement2.find(delimiterPattern), tempStatement2.find(delimiterWith));
      withStatement = t_queryInput.substr(t_queryInput.find(delimiterWith), t_queryInput.size());

    //Case 2: Check clause ordering is: Select, such that, with, pattern
    } else if (t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterPattern)
      && t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterWith)
      && t_queryInput.find(delimiterPattern) > t_queryInput.find(delimiterWith)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      suchThatStatement = tempStatement.substr(tempStatement.find(delimiterSuchThat), tempStatement.find(delimiterWith));
      tempStatement2 = tempStatement.substr(tempStatement.find(delimiterWith), t_queryInput.size());
      patternStatement = tempStatement2.substr(tempStatement2.find(delimiterWith), tempStatement2.find(delimiterPattern));
      withStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());

    //Case 3: Check clause ordering is: Select, pattern, such that, with
    } else if (t_queryInput.find(delimiterPattern) < t_queryInput.find(delimiterSuchThat)
      && t_queryInput.find(delimiterPattern) < t_queryInput.find(delimiterWith)
      && t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterWith)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
      patternStatement = tempStatement.substr(tempStatement.find(delimiterPattern), tempStatement.find(delimiterSuchThat));
      tempStatement2 = tempStatement.substr(tempStatement.find(delimiterSuchThat), t_queryInput.size());
      suchThatStatement = tempStatement2.substr(tempStatement2.find(delimiterSuchThat), tempStatement2.find(delimiterWith));
      withStatement = t_queryInput.substr(t_queryInput.find(delimiterWith), t_queryInput.size());

    //Case 4: Check clause ordering is: Select, pattern, with, such that
    } else if (t_queryInput.find(delimiterPattern) < t_queryInput.find(delimiterSuchThat)
      && t_queryInput.find(delimiterPattern) < t_queryInput.find(delimiterWith)
      && t_queryInput.find(delimiterSuchThat) > t_queryInput.find(delimiterWith)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
      patternStatement = tempStatement.substr(tempStatement.find(delimiterPattern), tempStatement.find(delimiterWith));
      tempStatement2 = tempStatement.substr(tempStatement.find(delimiterWith), t_queryInput.size());
      withStatement = tempStatement2.substr(tempStatement2.find(delimiterWith), tempStatement2.find(delimiterSuchThat));
      suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());

    //Case 5: Check clause ordering is: Select, with, such that, pattern
    } else if (t_queryInput.find(delimiterWith) < t_queryInput.find(delimiterPattern)
      && t_queryInput.find(delimiterWith) < t_queryInput.find(delimiterSuchThat)
      && t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterPattern)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterWith) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterWith), t_queryInput.size());
      withStatement = tempStatement.substr(tempStatement.find(delimiterWith), tempStatement.find(delimiterSuchThat));
      tempStatement2 = tempStatement.substr(tempStatement.find(delimiterSuchThat), t_queryInput.size());
      suchThatStatement = tempStatement2.substr(tempStatement2.find(delimiterSuchThat), tempStatement2.find(delimiterPattern));
      patternStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());

    //Case 6: Check clause ordering is: Select, with, pattern, such that
    } else if (t_queryInput.find(delimiterWith) < t_queryInput.find(delimiterPattern)
      && t_queryInput.find(delimiterWith) < t_queryInput.find(delimiterSuchThat)
      && t_queryInput.find(delimiterSuchThat) > t_queryInput.find(delimiterPattern)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterWith) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterWith), t_queryInput.size());
      withStatement = tempStatement.substr(tempStatement.find(delimiterWith), tempStatement.find(delimiterPattern));
      tempStatement2 = tempStatement.substr(tempStatement.find(delimiterPattern), t_queryInput.size());
      patternStatement = tempStatement2.substr(tempStatement2.find(delimiterPattern), tempStatement2.find(delimiterSuchThat));
      suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
    }
  }

  //Cases with just Select, such that, pattern clauses
  if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos && t_queryInput.find(delimiterWith) == std::string::npos) {
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
    } 
  }

  //Cases with just Select, with, such that clauses
  if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterWith) != std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos) {
    if (t_queryInput.find(delimiterSuchThat) < t_queryInput.find(delimiterWith)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
      suchThatStatement = tempStatement.substr(tempStatement.find(delimiterSuchThat), tempStatement.find(delimiterWith));
      withStatement = t_queryInput.substr(t_queryInput.find(delimiterWith), t_queryInput.size());

    } else if (t_queryInput.find(delimiterWith) < t_queryInput.find(delimiterSuchThat)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterWith) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterWith), t_queryInput.size());
      withStatement = tempStatement.substr(tempStatement.find(delimiterWith), tempStatement.find(delimiterSuchThat));
      suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
    }
  }

  //Cases with just Select, with, pattern clauses
  if (t_queryInput.find(delimiterWith) != std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos && t_queryInput.find(delimiterSuchThat) == std::string::npos) {
    if (t_queryInput.find(delimiterWith) < t_queryInput.find(delimiterPattern)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterWith) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterWith), t_queryInput.size());
      withStatement = tempStatement.substr(tempStatement.find(delimiterWith), tempStatement.find(delimiterPattern));
      patternStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());

    } else if (t_queryInput.find(delimiterPattern) < t_queryInput.find(delimiterWith)) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
      tempStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
      patternStatement = tempStatement.substr(tempStatement.find(delimiterPattern), tempStatement.find(delimiterWith));
      withStatement = t_queryInput.substr(t_queryInput.find(delimiterWith), t_queryInput.size());
    }
  }

  //Cases with just Select clause + one other clause and Select clause only
  if (t_queryInput.find(delimiterSuchThat) != std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos && t_queryInput.find(delimiterWith) == std::string::npos) {
    selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSuchThat) - 1);
    suchThatStatement = t_queryInput.substr(t_queryInput.find(delimiterSuchThat), t_queryInput.size());
  } else if (t_queryInput.find(delimiterSuchThat) == std::string::npos && t_queryInput.find(delimiterPattern) != std::string::npos && t_queryInput.find(delimiterWith) == std::string::npos) {
    selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
    patternStatement = t_queryInput.substr(t_queryInput.find(delimiterPattern), t_queryInput.size());
  } else if (t_queryInput.find(delimiterSuchThat) == std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos && t_queryInput.find(delimiterWith) != std::string::npos) {
    selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterPattern) - 1);
    withStatement = t_queryInput.substr(t_queryInput.find(delimiterWith), t_queryInput.size());
  } else if (t_queryInput.find(delimiterSuchThat) == std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos) {
    selectStatement = t_queryInput;
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
      g1 = Grammar(queryType::GType::BOOLEAN, g1.getName());
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

          //Checks if Uses/Modifies contains the correct parameters and return false if true
          if (designAbstractionEntity == "Uses" && g1.getType() == queryType::GType::VAR
            || designAbstractionEntity == "Uses" && g1.getType() == queryType::GType::ST_LST
            || designAbstractionEntity == "Modifies" && g1.getType() == queryType::GType::VAR
            || designAbstractionEntity == "Modifies" && g1.getType() == queryType::GType::ST_LST) {
            return false;
          }

          //Checks if Calls contains the correct parameters: Procedure condition
          if (designAbstractionEntity == "Calls" && g1.getType() != queryType::GType::PROC
            || designAbstractionEntity == "Calls*" && g1.getType() != queryType::GType::PROC) {
            return false;
          }

          //Checks if Next contains the correct parameters
          if (designAbstractionEntity == "Next" && g1.getType() == queryType::GType::VAR
            || designAbstractionEntity == "Next" && g1.getType() == queryType::GType::PROC
            || designAbstractionEntity == "Next" && g1.getType() == queryType::GType::ST_LST
            || designAbstractionEntity == "Next" && g1.getType() == queryType::GType::CONST) {
            return false;
          }

          //Checks if Next* contains the correct parameters
          if (designAbstractionEntity == "Next*" && g1.getType() == queryType::GType::VAR
            || designAbstractionEntity == "Next*" && g1.getType() == queryType::GType::PROC
            || designAbstractionEntity == "Next*" && g1.getType() == queryType::GType::ST_LST
            || designAbstractionEntity == "Next*" && g1.getType() == queryType::GType::CONST) {
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
              if (designAbstractionEntity == "Next" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::PROC
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::ST_LST
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::CONST) {
                return false;
              }

              //Checks if Next* contains the correct parameters
              if (designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::PROC
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::ST_LST
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::CONST) {
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
              if (designAbstractionEntity == "Uses" && g2.getType() != queryType::GType::VAR
                || designAbstractionEntity == "Modifies" && g2.getType() != queryType::GType::VAR) {
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
              if (designAbstractionEntity == "Next" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::PROC
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::ST_LST
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::CONST) {
                return false;
              }

              //Checks if Next* contains the correct parameters
              if (designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::PROC
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::ST_LST
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::CONST) {
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
              if (designAbstractionEntity == "Uses" && g2.getType() != queryType::GType::VAR
                || designAbstractionEntity == "Modifies" && g2.getType() != queryType::GType::VAR) {
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
              if (designAbstractionEntity == "Next" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::PROC
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::ST_LST
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::CONST) {
                return false;
              }

              //Checks if Next* contains the correct parameters
              if (designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::PROC
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::ST_LST
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::CONST) {
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
              if (designAbstractionEntity == "Uses" && g2.getType() != queryType::GType::VAR
                || designAbstractionEntity == "Modifies" && g2.getType() != queryType::GType::VAR) {
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
              if (designAbstractionEntity == "Next" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::PROC
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::ST_LST
                || designAbstractionEntity == "Next" && g2.getType() == queryType::GType::CONST) {
                return false;
              }

              //Checks if Next* contains the correct parameters
              if (designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::VAR
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::PROC
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::ST_LST
                || designAbstractionEntity == "Next*" && g2.getType() == queryType::GType::CONST) {
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
              if (designAbstractionEntity == "Uses" && g2.getType() != queryType::GType::VAR
                || designAbstractionEntity == "Modifies" && g2.getType() != queryType::GType::VAR) {
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

    //assignment patterns
    if (patternOfGrammar.getType() == queryType::GType::ASGN) {
      std::string patternLeftName = patternVector.front();
      
      //left side: string
      if (patternLeftName.find('"') != std::string::npos) {
        removeCharsFromString(patternLeftName, "\\\" ");
        grammarPatternLeft = Grammar(queryType::GType::STR, patternLeftName);
      }

      //left side: _
      else if (patternLeftName == "_") {
        removeCharsFromString(patternLeftName, "\"");
        grammarPatternLeft = Grammar(queryType::GType::STR, patternLeftName);

      //left side: synonym
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

      std::string patternRightName = patternVector.back();

      patternRightName = m_stringUtil.trimString(patternRightName);

      //Check for equal number of matching brackets
      size_t n1 = std::count(patternRightName.begin(), patternRightName.end(), '(');
      size_t n2 = std::count(patternRightName.begin(), patternRightName.end(), ')');

      if (n1 != n2) {
        return false;
      }

      //Check if any ) exists before (
      if (patternRightName.find(')') < patternRightName.find('(')) {
        return false;
      }

      //Check for any brackets that are: ()
      //std::vector<size_t> positions; // holds all the positions that sub occurs within str

      std::string patternRightNameTemp = patternRightName;
      char sub = '(';

      size_t pos = patternRightNameTemp.find(sub, 0);
      while (pos != std::string::npos) {
        //positions.push_back(pos);
        pos = patternRightNameTemp.find(sub, pos + 1);
        if (patternRightNameTemp.find_first_not_of(" \t") == ')') {
          return false;
        }
      }

      patternRightName = m_stringUtil.trimString(patternRightName);
      if ((patternRightName.find('"') != std::string::npos) && patternRightName.front() == '_' && patternRightName.back() == '_') {
        removeCharsFromString(patternRightName, "\\\" _");
        grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
        isSubTree = true;
      } else if (patternRightName.find('"') != std::string::npos && patternRightName.front() != '_' && patternRightName.back() != '_') {
        removeCharsFromString(patternRightName, "\\\" ");
        grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
      } else if (patternRightName == "_") {
        grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
      } else {
        return false;
      }

    //while patterns
    } else if (patternOfGrammar.getType() == queryType::GType::WHILE) {
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

      std::string patternRightName = patternVector.back();
      patternRightName = m_stringUtil.trimString(patternRightName);
      if (patternRightName == "_") {
        grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
      } else {
        return false;
      }

    //if patterns: not done, require pattern.cpp to have 3rd _
    } else if (patternOfGrammar.getType() == queryType::GType::IF) {
      std::string patternLeftName = patternVector.front();
      if (patternLeftName.find('"') != std::string::npos) {
        removeCharsFromString(patternLeftName, "\\\" ");
        grammarPatternLeft = Grammar(queryType::GType::STR, patternLeftName);
      }

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

      std::string patternRightName = patternVector.back();
      patternRightName = m_stringUtil.trimString(patternRightName);
      if (patternRightName == "_") {
        grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
      } else {
        return false;
      }
    }

    //create pattern object
    Pattern patternObjectCreated(patternOfGrammar, grammarPatternLeft, grammarPatternRight, isSubTree);
    m_patternQueue.push(patternObjectCreated);
    
    }

  //creation of with object
  if (withStatement == "") {
    std::cout << "with statement is empty" << std::endl;
  } else {
    withStatement = m_stringUtil.trimString(withStatement);
    std::string delimiterSpace1 = " ";
    std::string withObject = withStatement.substr(withStatement.find(delimiterSpace1), withStatement.size());

    std::string withLeft = withObject.substr(0, withObject.find("="));
    std::string withRight = withObject.substr(withObject.find("="), withObject.size());

    withLeft = m_stringUtil.trimString(withLeft);
    withRight = m_stringUtil.trimString(withRight);

    Grammar withLeftGrammar;
    Grammar withRightGrammar;
    bool isWithTrue = true;

    int withLeftInt = 0;
    int withRightInt = 0;

    std::stringstream convertLeft(withLeft);
    if (!(convertLeft >> withLeftInt)) {
      withLeftInt = 0;
    }

    std::stringstream convertRight(withRight);
    if (!(convertRight >> withRightInt)) {
      withRightInt = 0;
    }

    //Check if both left and right are numbers
    if (withLeftInt > 0 && withRightInt > 0 && withLeftInt != withRightInt) {
      return false;
    } else if (withLeftInt > 0 && withRightInt > 0 && withLeftInt == withRightInt) {
      withLeftGrammar = Grammar(queryType::GType::CONST, withLeft);
      withRightGrammar = Grammar(queryType::GType::CONST, withRight);
    }

    //Check orientation of parameters: left integer, right attribute
    if (withLeftInt > 0 && withRight.find('.') != std::string::npos) {
      isWithTrue = withClauseAttNum(withRight, withLeft, withLeftGrammar, withRightGrammar);
      if (isWithTrue == false) {
        return false;
      }
    //Case 2: left string, right attribute
    } else if (withLeft.find('"') != std::string::npos && withRight.find('.') != std::string::npos) {
      isWithTrue = withClauseAttString(withRight, withLeft, withLeftGrammar, withRightGrammar);
      if (isWithTrue == false) {
        return false;
      }
    //Case 3: left attribute, right attribute
    } else if (withLeft.find('.') != std::string::npos && withRight.find('.') != std::string::npos) {
      withClauseAttAtt(withLeft, withRight, withLeftGrammar, withRightGrammar);

    //Case 4: left attribute, right integer
    } else if (withLeft.find('.') != std::string::npos && withRightInt > 0) {
      isWithTrue = withClauseAttNum(withLeft, withRight, withLeftGrammar, withRightGrammar);
      if (isWithTrue == false) {
        return false;
      }
    //Case 5: left attribute, right string
    } else if (withLeft.find('.') != std::string::npos && withRight.find('"') != std::string::npos) {
      isWithTrue = withClauseAttString(withLeft, withRight, withLeftGrammar, withRightGrammar);
      if (isWithTrue == false) {
        return false;
      }
    }
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

bool QueryPreProcessor::withClauseAttNum(std::string attribute, std::string integer, Grammar withLeftGrammar, Grammar withRightGrammar) {
  withAttributeProcessor(attribute, withLeftGrammar);
  
  //Case 2.1: GType:Stmt, asgn, while, if, call, GType: Stmt# attribute
  if (withLeftGrammar.getType() == queryType::GType::STMT && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::ASGN && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::WHILE && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::IF && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::CALL && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::PROG_LINE
    ) {

    withRightGrammar = Grammar(queryType::GType::STMT_NO, integer);

    //Case 2.2: GType: CONSTANT, AType: VALUE
  } else if (withLeftGrammar.getType() == queryType::GType::CONST
    && withLeftGrammar.getAttr() == queryType::AType::VALUE) {

    withRightGrammar = Grammar(queryType::GType::CONST, integer);
  } 

  return true;
}

bool QueryPreProcessor::withClauseAttString(std::string attribute, std::string inputString, Grammar withLeftGrammar, Grammar withRightGrammar) {
  withAttributeProcessor(attribute, withLeftGrammar);
  
  removeCharsFromString(inputString, "\"");
  withRightGrammar = Grammar(queryType::GType::STR, inputString);
  
  if (withLeftGrammar.getAttr() != queryType::AType::PROC_NAME
    || withLeftGrammar.getAttr() != queryType::AType::VAR_NAME) {
    return false;
  } else {
    return true;
  }
}

void QueryPreProcessor::withClauseAttAtt(std::string leftAttribute, std::string rightAttribute, Grammar withLeftGrammar, Grammar withRightGrammar) {
  withAttributeProcessor(leftAttribute, withLeftGrammar);
  withAttributeProcessor(rightAttribute, withRightGrammar);
}

void QueryPreProcessor::withAttributeProcessor(std::string attribute, Grammar withGrammar) {
  std::string withSynonym = attribute.substr(0, attribute.find("."));
  std::string withAttribute = attribute.substr(attribute.find("."), attribute.size());

  withSynonym = m_stringUtil.trimString(withSynonym);
  withAttribute = m_stringUtil.trimString(withAttribute);

  int counterS = 0;
  for (auto s = m_grammarVector.begin(); s != m_grammarVector.end(); s++, counterS++) {
    if (m_grammarVector.at(counterS).getName() == withSynonym) {
      if (withAttribute == PROCNAME) {
        withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        withGrammar.setAType(queryType::AType::PROC_NAME);
      } else if (withAttribute == VARNAME) {
        withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        withGrammar.setAType(queryType::AType::VAR_NAME);
      } else if (withAttribute == VALUE) {
        withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        withGrammar.setAType(queryType::AType::VALUE);
      } else if (withAttribute == STMT_NO) {
        withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        withGrammar.setAType(queryType::AType::STMT_NUM);
      }

    }
  }
}