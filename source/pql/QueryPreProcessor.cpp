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
std::string QueryPreProcessor::BOOLEAN_QPP = "BOOLEAN";
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


std::vector<Grammar> QueryPreProcessor::getSelectVector(void) {
  return m_selectVectorQE;
}

std::vector<Relation> QueryPreProcessor::getSuchThatVector(void) {
  return m_relationVectorQE;
}

std::vector<Pattern> QueryPreProcessor::getPatternVector(void) {
  return m_patternVectorQE;
}

std::vector<With> QueryPreProcessor::getWithVector(void) {
  return m_withVectorQE;
}

std::unordered_map<std::string, int> QueryPreProcessor::getSynonym(void) {
  return m_synonymMap;
}

std::vector<Grammar> QueryPreProcessor::getGrammarVector(void) {
  return m_grammarVector;
}

STRING QueryPreProcessor::splitStringDeclaration(STRING t_Input) {
  std::string delimiter = "Select";
  std::string declaration = t_Input.substr(0, t_Input.find(delimiter));

  declaration = m_stringUtil.trimString(declaration);
  declaration = m_stringUtil.reduceString(declaration);
  return declaration;
}

STRING QueryPreProcessor::splitStringQuery(STRING t_Input) {
  std::string delimiter = "Select";
  std::string query = t_Input.substr(t_Input.find(delimiter), t_Input.size()); //same for this as delimiter is "; Select"

  query = m_stringUtil.trimString(query);
  //query = m_stringUtil.reduceString(query);
  return query;
}


BOOLEAN QueryPreProcessor::tokenizeDeclaration(STRING t_declarationInput) {
  bool isTokenized = false;
  if (t_declarationInput == "") {
    return true;
  } else {
    std::vector<std::string> declarationVector;

    std::string starterString;
    //tokens are split by ;
    std::size_t prev_pos = 0, pos;
    starterString = m_stringUtil.trimString(t_declarationInput);

    //for cases with only one relation(such that)
    if ((pos = starterString.find(";", prev_pos)) == std::string::npos) {
      declarationVector.push_back(starterString.substr(prev_pos, pos - prev_pos));
    }

    //parse declaration
    else {

      //Segment by semicolon first
      while ((pos = starterString.find(";", prev_pos)) != std::string::npos) {
        if (prev_pos == 0) {
          declarationVector.push_back(starterString.substr(prev_pos, pos - prev_pos));
          prev_pos = pos + 1;
        }
        if (pos > prev_pos) {        
          declarationVector.push_back(starterString.substr(prev_pos, pos - prev_pos));
        } prev_pos = pos + 1;
      }
      if (prev_pos < starterString.length()) {
        declarationVector.push_back(starterString.substr(prev_pos, std::string::npos));  //prev pos + 1 so that we can delete the trailing space in "; "
      }
    }

    std::string tempString;

    for (std::size_t j = 0; j != declarationVector.size(); ++j) {
      tempString = declarationVector.at(j);
      tempString = m_stringUtil.trimString(tempString);

      std::string entity = tempString.substr(0, tempString.find(WHITESPACE));
      std::string variables = tempString.substr(tempString.find(WHITESPACE) + 1, tempString.size()); //same for this as delimiter is "; Select" variables split individually

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
        std::string variableTemp;

        //Segment based on commas
        while ((pos = variables.find(",", prev_pos)) != std::string::npos) {
          if (prev_pos == 0) {
            variableTemp = variables.substr(prev_pos, pos - prev_pos);
            variableTemp = m_stringUtil.trimString(variableTemp);

            //Validation to ensure no space in between e.g: stmt s, s 1, s2,...
            if (variableTemp.find(WHITESPACE) != std::string::npos) {
              return false;
            }
            variableVector.push_back(variableTemp);
            prev_pos = pos + 1;

          } else if (pos > prev_pos) {

            variableTemp = variables.substr(prev_pos, pos - prev_pos);
            variableTemp = m_stringUtil.trimString(variableTemp);

            //Validation to ensure no space in between e.g: stmt s, s 1, s2,...
            if (variableTemp.find(WHITESPACE) != std::string::npos) {
              return false;
            }
            variableVector.push_back(variableTemp);
          }
          prev_pos = pos + 1;
        }
        if (prev_pos < variables.length()) {
          variableTemp = variables.substr(prev_pos, std::string::npos);
          variableTemp = m_stringUtil.trimString(variableTemp);

          //Validation to ensure no space in between e.g: stmt s, s 1, s2,...
          if (variableTemp.find(WHITESPACE) != std::string::npos) {
            return false;
          }
          variableVector.push_back(variableTemp);

        }

        int counterL = 0;
        for (auto l = variableVector.begin(); l != variableVector.end(); l++, counterL++) {          
          //Check for duplicates map iterator
          std::unordered_map<std::string, int>::const_iterator validationMapIterator = m_validationMap.find(variableVector.at(counterL));
          
          if (entity == PROCEDURE) {
            Grammar g(queryType::GType::PROC, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }

          } else if (entity == STMTLST) {
            Grammar g(queryType::GType::ST_LST, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }

          } else if (entity == STMT) {
            Grammar g(queryType::GType::STMT, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }


          } else if (entity == ASSIGN) {
            Grammar g(queryType::GType::ASGN, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }

          } else if (entity == WHILE) {
            Grammar g(queryType::GType::WHILE, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }

          } else if (entity == IF) {
            Grammar g(queryType::GType::IF, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }

          } else if (entity == VARIABLE) {
            Grammar g(queryType::GType::VAR, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }


          } else if (entity == CONSTANT) {
            Grammar g(queryType::GType::CONST, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }


          } else if (entity == PROG_LINE) {
            Grammar g(queryType::GType::PROG_LINE, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }

          } else if (entity == BOOLEAN_QPP) {
            Grammar g(queryType::GType::BOOLEAN, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }



          } else if (entity == CALL) {
            Grammar g(queryType::GType::CALL, variableVector.at(counterL));
            m_grammarVector.push_back(g);

            //New map for validation
            if (validationMapIterator == m_validationMap.end()) {
              m_validationMap.insert({ g.getName(), 1 });
            } else {
              return false;
            }


          } else {
            return false;
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

BOOLEAN QueryPreProcessor::tokenizeQuery(STRING t_queryInput) {
  bool isTokenized = false;
  std::vector<std::string> queryVector;
  std::string selectStatement;
  std::string suchThatStatement;
  std::string patternStatement;
  std::string delimiterSelect = "Select";
  std::string delimiterSuchThat = "such that";
  std::string delimiterPattern = "pattern";
  std::string delimiterWith = "with";
  std::string withStatement;
  std::string tempStatement;
  std::string tempStatement2;

  std::string secondStatement;

  std::string prevClause = "";
  std::string secondTempStatement;
  std::string withStatementLeft;
  std::string withStatementRight;

  std::vector<std::string> tempSynonymVector;
  int mapValue = 1;
  int mapDefaultValue = 1;

  //Check whether semicolon is at the end of the query
  if (t_queryInput.back() == ';' && t_queryInput.find("Select BOOLEAN") != std::string::npos) {
    m_selectBOOLEANExists = true;
    return false;
  } else if (t_queryInput.back() == ';' && t_queryInput.find("Select BOOLEAN") == std::string::npos) {
    return false;
  }

  //New Method:
  //Case 1: only select statements
  if (t_queryInput.find(delimiterSuchThat) == std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos && t_queryInput.find(delimiterWith) == std::string::npos) {
    selectStatement = t_queryInput;
    selectStatement = m_stringUtil.reduceString(selectStatement);
    selectStatement = m_stringUtil.trimString(selectStatement);

    if (selectStatement.find("Select BOOLEAN") != std::string::npos) {
      m_selectBOOLEANExists = true;
    }

    m_selectVector.push_back(selectStatement);
  } else {
    int tempSpaceLoc;

    //Case 1: no tuples
    if (t_queryInput.find('<') == std::string::npos && t_queryInput.find('>') == std::string::npos) {
      selectStatement = t_queryInput.substr(0, t_queryInput.find(WHITESPACE));
      t_queryInput = t_queryInput.substr(t_queryInput.find(WHITESPACE));
      t_queryInput = t_queryInput.substr(t_queryInput.find_first_not_of(WHITESPACE), t_queryInput.find_last_not_of(WHITESPACE));
      tempSpaceLoc = t_queryInput.find(WHITESPACE);
      std::string tempSelectStatement = t_queryInput.substr(0, t_queryInput.find(WHITESPACE));
      selectStatement.append(WHITESPACE);
      selectStatement.append(tempSelectStatement);

      if (selectStatement.find("Select BOOLEAN") != std::string::npos) {
        m_selectBOOLEANExists = true;
      }

      secondStatement = t_queryInput.substr(tempSpaceLoc, t_queryInput.size());
      secondStatement = m_stringUtil.trimString(secondStatement);

    //Case 2: with tuples
    } else if (t_queryInput.find('<') != std::string::npos && t_queryInput.find('>') != std::string::npos) {

      if (selectStatement.find("Select BOOLEAN") != std::string::npos) {
        return false;
      }

      selectStatement = t_queryInput.substr(0, t_queryInput.find('>') + 1);
      t_queryInput = t_queryInput.substr(t_queryInput.find('>') + 1);
      t_queryInput = m_stringUtil.trimString(t_queryInput);
      secondStatement = t_queryInput;
    }
    while (true) {
      
      //secondStatement = m_stringUtil.trimString(secondStatement);
      secondTempStatement = secondStatement.substr(0, secondStatement.find(WHITESPACE));
      if (secondTempStatement == "such") {
        secondStatement = secondStatement.substr(secondStatement.find(WHITESPACE), secondStatement.size());
        //secondStatement = secondStatement.substr(secondStatement.find_first_not_of(WHITESPACE));
        secondStatement = secondStatement.substr(1, secondStatement.size());
        secondTempStatement = secondStatement.substr(0, secondStatement.find(WHITESPACE));
        if (secondTempStatement == "that") {
          
          //New Method
          secondStatement = multiClauseProcessor(secondStatement, delimiterSuchThat);
        }
        
      } else if (secondTempStatement == "pattern") {

        //New Method
        secondStatement = multiClauseProcessor(secondStatement, delimiterPattern);
      } else if (secondTempStatement == "with") {

        //New Method
        secondStatement = multiClauseProcessor(secondStatement, delimiterWith);

      } else if (secondTempStatement == "and") {
        if (m_prevClause.compare(delimiterSuchThat) == 0) {

          //New Method
          secondStatement = multiClauseProcessor(secondStatement, delimiterSuchThat);
        } else if (m_prevClause.compare(delimiterPattern) == 0) {
  
          //New Method
          secondStatement = multiClauseProcessor(secondStatement, delimiterPattern);
        } else if (m_prevClause.compare(delimiterWith) == 0) {
 
          //New Method
          secondStatement = multiClauseProcessor(secondStatement, delimiterWith);
        }
      } else {
        if (m_secondStatementTemp == "" && secondStatement == "") {
          break;
        } else {
          return false;
        }
      }
    }
  }
  std::string synonymOriginal;

  //Case:Tuple does not exist, 
  if (selectStatement.find(WHITESPACE) != std::string::npos && selectStatement.find('<') == std::string::npos) {
    synonymOriginal = selectStatement.substr(selectStatement.find(WHITESPACE), selectStatement.size());

  //Case: Tuple exists
  } else if (selectStatement.find('<') != std::string::npos) {
    synonymOriginal = selectStatement.substr(selectStatement.find('<'), selectStatement.size());
  }
  synonymOriginal = m_stringUtil.trimString(synonymOriginal);
  std::string synonym;
  std::string synonymAttribute;

  //Case 1:Tuple exists
  if (synonymOriginal.find('<') != std::string::npos && synonymOriginal.find('>') != std::string::npos) {
    std::vector<std::string> synonymOriginalVector;
    int synonymCount = 0;

    //check whether '<>' or ' <  > ' exists
    std::string synonymOriginalTemp = synonymOriginal;
    char subLeft = '<';

    size_t pos = synonymOriginalTemp.find(subLeft, 0);
    while (pos != std::string::npos) {
    pos = synonymOriginalTemp.find(subLeft, pos + 1);
    if (synonymOriginalTemp.find_first_not_of(WHITESPACE) == '>') {
        return false;
      }
    }

    std::string synonymOriginalTemp2 = synonymOriginal;
    //Check whether '<s,>' exists
    if (synonymOriginalTemp2.find(',') != std::string::npos) {
      synonymOriginalTemp2 = synonymOriginalTemp2.substr(synonymOriginalTemp2.find('<') + 1, synonymOriginalTemp2.find('>') - 1);
      synonymOriginalTemp2 = m_stringUtil.trimString(synonymOriginalTemp2);
      if (synonymOriginalTemp2.at(0) == ',') {
        return false;
      }
      if (synonymOriginalTemp2.at(synonymOriginalTemp2.size()-1) == ',') {
        return false;
      }
    }

    synonymOriginalVector = stringVectorTokenizer(",<>", synonymOriginal, synonymOriginalVector);
    
    //For each synonym in < >, validate and process synonyms
    for (auto syn = synonymOriginalVector.begin(); syn != synonymOriginalVector.end(); syn++, synonymCount++) {
      synonymOriginal = synonymOriginalVector.at(synonymCount);
      synonymOriginal = m_stringUtil.trimString(synonymOriginal);
      synonymAttribute = "";

      //if after post trim string, there is still whitespace = " p1 p2 " -> "p1 p2" space in between
      if (synonymOriginal.find(WHITESPACE) != std::string::npos) {
        return false;
      }
      if (synonymOriginal.find('.') != std::string::npos) {
        synonym = synonymOriginal.substr(0, synonymOriginal.find('.'));
        synonymAttribute = synonymOriginal.substr(synonymOriginal.find('.') + 1, synonymOriginal.size());
      } else {
        synonym = synonymOriginal;
      }
      synonym = m_stringUtil.trimString(synonym);
      synonymAttribute = m_stringUtil.trimString(synonymAttribute);

      Grammar g1;

      //storing select queue synonyms

      //Case 1: synonym attribute exists
      if (synonymAttribute != "") {
        int counterU = 0;
        for (auto u = m_grammarVector.begin(); u != m_grammarVector.end(); u++, counterU++) {
          /*if (m_selectQueue.size() == 1) {
            break;
          }*/
          g1 = m_grammarVector.at(counterU);
          std::string grammarName = g1.getName();

          if (grammarName == synonym) {

            //validate attributes
            if (synonymAttribute == PROCNAME) {
              if (g1.getType() == queryType::GType::CALL || g1.getType() == queryType::GType::PROC) {
                g1.setAType(queryType::AType::PROC_NAME);
              } else {
                return false;
              }
            } else if (synonymAttribute == VARNAME) {
              if (g1.getType() == queryType::GType::VAR) {
                g1.setAType(queryType::AType::VAR_NAME);
              } else {
                return false;
              }

            } else if (synonymAttribute == VALUE) {
              if (g1.getType() == queryType::GType::CONST) {
                g1.setAType(queryType::AType::VALUE);
              } else {
                return false;
              }

            } else if (synonymAttribute == STMT_NO) {
              if (g1.getType() == queryType::GType::STMT
                || g1.getType() == queryType::GType::ASGN
                || g1.getType() == queryType::GType::CALL
                || g1.getType() == queryType::GType::IF
                || g1.getType() == queryType::GType::WHILE) {
                g1.setAType(queryType::AType::STMT_NUM);
              } else {
                return false;
              }
            }

            m_selectVectorQE.push_back(g1);
            m_synonymMap.insert({ g1.getName(), 1 });

          } else if (synonym == BOOLEAN_QPP) {
            return false;
          } 
        }

        //Case 2: synonym attribute does not exist
      } else {
        int counterM = 0;
        for (auto m = m_grammarVector.begin(); m != m_grammarVector.end(); m++, counterM++) {
          /*if (m_selectQueue.size() == 1) {
            break;
          }*/
          g1 = m_grammarVector.at(counterM);
          std::string grammarName = g1.getName();

          if (grammarName == synonym) {
            m_selectVectorQE.push_back(g1);
            m_synonymMap.insert({ g1.getName(), 1 });
          } else if (synonym == BOOLEAN_QPP) {
            return false;
          }
        }
      }
    }
    //Case 2: where either < or > exists only
  } else if (synonymOriginal.find('<') == std::string::npos && synonymOriginal.find('>') != std::string::npos
    || synonymOriginal.find('<') != std::string::npos && synonymOriginal.find('>') == std::string::npos) {
    return false;

    //Case 3: where no <> exists
  } else {
    synonymOriginal = m_stringUtil.trimString(synonymOriginal);
    //selecting synonym with attributes

    if (synonymOriginal.find('.') != std::string::npos) {
      synonym = synonymOriginal.substr(0, synonymOriginal.find('.'));
      synonymAttribute = synonymOriginal.substr(synonymOriginal.find('.') + 1, synonymOriginal.size());
    } else {
      synonym = synonymOriginal;
    }
    synonym = m_stringUtil.trimString(synonym);
    synonymAttribute = m_stringUtil.trimString(synonymAttribute);

    Grammar g1;

    //storing select queue synonyms

    //Case 1: synonym attribute exists
    if (synonymAttribute != "") {
      int counterU = 0;
      for (auto u = m_grammarVector.begin(); u != m_grammarVector.end(); u++, counterU++) {
        if (m_selectVectorQE.size() == 1) {
          break;
        }
        g1 = m_grammarVector.at(counterU);
        std::string grammarName = g1.getName();

        if (grammarName == synonym) {

          //validate attributes
          if (synonymAttribute == PROCNAME) {
            if (g1.getType() == queryType::GType::CALL || g1.getType() == queryType::GType::PROC) {
              g1.setAType(queryType::AType::PROC_NAME);
            } else {
              return false;
            }
          } else if (synonymAttribute == VARNAME) {
            if (g1.getType() == queryType::GType::VAR) {
              g1.setAType(queryType::AType::VAR_NAME);
            } else {
              return false;
            }

          } else if (synonymAttribute == VALUE) {
            if (g1.getType() == queryType::GType::CONST) {
              g1.setAType(queryType::AType::VALUE);
            } else {
              return false;
            }

          } else if (synonymAttribute == STMT_NO) {
            if (g1.getType() == queryType::GType::STMT
              || g1.getType() == queryType::GType::ASGN
              || g1.getType() == queryType::GType::CALL
              || g1.getType() == queryType::GType::IF
              || g1.getType() == queryType::GType::WHILE) {
              g1.setAType(queryType::AType::STMT_NUM);
            } else {
              return false;
            }
          } else {
            return false;
          }
          m_selectVectorQE.push_back(g1);

        } else if (synonym == BOOLEAN_QPP) {
          return false;
        }
      }

      //Case 2: synonym attribute does not exist
    } else {

      int counterM = 0;
      for (auto m = m_grammarVector.begin(); m != m_grammarVector.end(); m++, counterM++) {
        if (m_selectVectorQE.size() == 1) {
          break;
        }
        g1 = m_grammarVector.at(counterM);
        std::string grammarName = g1.getName();

        //std::cout << grammarName << " this is the grammarName" << std::endl;
        if (grammarName == synonym) {
          m_selectVectorQE.push_back(g1);
        } else if (synonym == BOOLEAN_QPP) {
          g1 = Grammar(queryType::GType::BOOLEAN, "BOOLEAN");
          m_selectVectorQE.push_back(g1);
        }
      }

      if (synonym == BOOLEAN_QPP && m_grammarVector.size() == 0) {
        g1 = Grammar(queryType::GType::BOOLEAN, g1.getName());
        m_selectVectorQE.push_back(g1);
      }
    }

    //Checks if the select statement synonym is not declared
    if (m_selectVectorQE.size() == 0) {
      return false;
    }

    Grammar selectGrammar = m_selectVectorQE.front();
    if (selectGrammar.getType() != queryType::GType::BOOLEAN) {
      m_synonymMap.insert({ selectGrammar.getName(), 1 });
    }
  }

  //Checks if the select statement synonym is not declared
  if (m_selectVectorQE.size() == 0) {
    return false;
  }
  //if design abstraction object does not exist
  if (m_relationVector.empty()) {
    std::cout << "relation vector is empty" << std::endl;
  }
  //if design abstraction object exists, create Relation object with parameters type, g1 g2
  else {
    int counterB = 0;
    for (auto b = m_relationVector.begin(); b != m_relationVector.end(); b++, counterB++) {
      std::string designAbstractions = m_relationVector.at(counterB);

      designAbstractions = m_stringUtil.reduceString(designAbstractions);
      designAbstractions = m_stringUtil.trimString(designAbstractions);

      std::string delimiterBracket = "(";
      //Validation checks

      //confirm only one ( and ) exists
      //Check for equal number of matching brackets
      size_t openBracketCheck = std::count(designAbstractions.begin(), designAbstractions.end(), '(');
      size_t closeBracketCheck = std::count(designAbstractions.begin(), designAbstractions.end(), ')');

      if ((openBracketCheck != closeBracketCheck) || openBracketCheck != 1) {
        return false;
      }

      std::string designAbstractionEntity = designAbstractions.substr(0, designAbstractions.find(delimiterBracket));
      std::string designAbstractionObject = designAbstractions.substr(designAbstractions.find(delimiterBracket), designAbstractions.size());

      designAbstractionEntity = m_stringUtil.trimString(designAbstractionEntity);
      designAbstractionObject = m_stringUtil.trimString(designAbstractionObject);

      //checks for invalid design abstractions
      if (designAbstractionEntity == "Follows"
        || designAbstractionEntity == "Follows*"
        || designAbstractionEntity == "Parent"
        || designAbstractionEntity == "Parent*"
        || designAbstractionEntity == "Uses"
        || designAbstractionEntity == "Modifies"
        || designAbstractionEntity == "Calls"
        || designAbstractionEntity == "Calls*"
        || designAbstractionEntity == "Next"
        || designAbstractionEntity == "Next*"
        || designAbstractionEntity == "Affects"
        || designAbstractionEntity == "Affects*") {

        std::vector<std::string> designAbstractionVectorNew;

        designAbstractionVectorNew = stringVectorTokenizer("() ,;\\", designAbstractionObject, designAbstractionVectorNew);

        if (designAbstractionVectorNew.size() != 2) {
          return false;
        }

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
        if (designAbstractionEntity == "Follows"
          || designAbstractionEntity == "Follows*"
          || designAbstractionEntity == "Parent"
          || designAbstractionEntity == "Parent*"
          || designAbstractionEntity == "Uses"
          || designAbstractionEntity == "Modifies"
          || designAbstractionEntity == "Calls"
          || designAbstractionEntity == "Calls*"
          || designAbstractionEntity == "Next") {
          if ((sTInt1 == sTInt2 && sTInt1 != 0) || (sTName1 == sTName2 
            && sTName1 != OPERATOR_UNDERSCORE && sTName2 != OPERATOR_UNDERSCORE
            && sTName1.find('"') == std::string::npos && sTName2.find('"') == std::string::npos)) {
            //return empty list
            return false;
          }
        }

        //Checks for no declaration first before moving on to check for synonyms
        //Follows and Follows*
        if (designAbstractionEntity == "Follows" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Follows*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          //Number, Number
          if (sTInt1 > 0 && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //Number, _
          } else if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //_, Number
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            // _, _
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

          }

          //Parent and Parent*
        } else if (designAbstractionEntity == "Parent" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Parent*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          //Number, Number
          if (sTInt1 > 0 && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //Number, _
          } else if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //_, Number
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            // _, _
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

          }

          //Uses, Modifies
        } else if (designAbstractionEntity == "Uses" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Modifies" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          //Number, String
          if (sTInt1 > 0 && sTName2.find('"') != std::string::npos) {
            removeCharsFromString(sTName2, "\\\"");
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //Number, _
          } else if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //String, String
          } else if (sTName1.find('"') != std::string::npos && sTName2.find('"') != std::string::npos) {
            removeCharsFromString(sTName1, "\\\"");
            removeCharsFromString(sTName2, "\\\"");
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);

            m_relationVectorQE.push_back(DAO);

            //String, _
          } else if (sTName1.find('"') != std::string::npos && sTName2 == OPERATOR_UNDERSCORE) {
            removeCharsFromString(sTName1, "\\\"");
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

          }

          //Call, Calls*
        } else if (designAbstractionEntity == "Calls" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Calls*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          //String, _
          if (sTName1.find('"') != std::string::npos && sTName2 == OPERATOR_UNDERSCORE) {
            removeCharsFromString(sTName1, "\\\"");
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //_, String
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTName2.find('"') != std::string::npos) {
            removeCharsFromString(sTName2, "\\\"");
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //String, String
          } else if (sTName1.find('"') != std::string::npos && sTName2.find('"') != std::string::npos) {
            removeCharsFromString(sTName1, "\\\"");
            removeCharsFromString(sTName2, "\\\"");
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);
            
            //_, _
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTName2 == OPERATOR_UNDERSCORE) {
            removeCharsFromString(sTName1, "\\\"");
            removeCharsFromString(sTName2, "\\\"");
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);
          }

          //Next, Next*
        } else if (designAbstractionEntity == "Next" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Next*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {
          // Number, _
          if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //_, Number
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //Number, Number
          } else if (sTInt1 > 0 && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //_, _
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);
          }

          //Affects, Affects*
        } else if (designAbstractionEntity == "Affects" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Affects*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          // Number, _
          if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //_, Number
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //Number, Number
          } else if (sTInt1 > 0 && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);

            //_, _
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_relationVectorQE.push_back(DAO);
          }
        }
        counterK = 0;
        bool moveOn = false;


        //Old Method
        for (auto k = m_grammarVector.begin(); k != m_grammarVector.end(); k++, counterK++) {
          Grammar tempGrammar = m_grammarVector.at(counterK);
          std::string grammarName = tempGrammar.getName();

          counterQ = 0;

          if (moveOn) {
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

            //Checks if Affects contains the correct parameters
            if (designAbstractionEntity == "Affects" && g1.getType() == queryType::GType::PROC
              || designAbstractionEntity == "Affects" && g1.getType() == queryType::GType::IF
              || designAbstractionEntity == "Affects" && g1.getType() == queryType::GType::ST_LST
              || designAbstractionEntity == "Affects" && g1.getType() == queryType::GType::WHILE
              || designAbstractionEntity == "Affects" && g1.getType() == queryType::GType::VAR
              || designAbstractionEntity == "Affects" && g1.getType() == queryType::GType::CONST
              || designAbstractionEntity == "Affects" && g1.getType() == queryType::GType::CALL) {
              return false;
            }

            //Checks if Affects* contains the correct parameters
            if (designAbstractionEntity == "Affects*" && g1.getType() == queryType::GType::PROC
              || designAbstractionEntity == "Affects*" && g1.getType() == queryType::GType::IF
              || designAbstractionEntity == "Affects*" && g1.getType() == queryType::GType::ST_LST
              || designAbstractionEntity == "Affects*" && g1.getType() == queryType::GType::WHILE
              || designAbstractionEntity == "Affects*" && g1.getType() == queryType::GType::VAR
              || designAbstractionEntity == "Affects*" && g1.getType() == queryType::GType::CONST
              || designAbstractionEntity == "Affects*" && g1.getType() == queryType::GType::CALL) {
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

                //Checks if Affects contains the correct parameters
                if (designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::PROC
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::IF
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::ST_LST
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::WHILE
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::VAR
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::CONST
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::CALL) {
                  return false;
                }

                //Checks if Affects* contains the correct parameters
                if (designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::PROC
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::IF
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::ST_LST
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::WHILE
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::VAR
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::CONST
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::CALL) {
                  return false;
                }

                //check is any design abstraction synonyms contains constant c
                if (g1.getType() == queryType::GType::CONST || g2.getType() == queryType::GType::CONST) {
                  return false;
                }

                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(sTName2);
                if (got == m_synonymMap.end()) {
                  m_synonymMap.insert({ sTName2, 1 });
                } else {
                  m_synonymMap[sTName2]++;
                }
                counterK = 0;
                moveOn = true;
                break;
              } else if (sTInt2 > 0) {

                //Checks if Calls contains the correct parameters: no numbers
                if (designAbstractionEntity == "Calls"
                  || designAbstractionEntity == "Calls*") {
                  return false;
                }

                g2 = Grammar(queryType::GType::STMT_NO, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
                break;

              } else if (sTName2.find('"') != std::string::npos) {

                //Checks if Next/Next*/Affects/AFfects* contains the correct parameters: no string
                if (designAbstractionEntity == "Next"
                  || designAbstractionEntity == "Next*"
                  || designAbstractionEntity == "Affects"
                  || designAbstractionEntity == "Affects*") {
                  return false;
                }

                removeCharsFromString(sTName2, "\\\"");
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
                break;

              } else if (sTName2 == OPERATOR_UNDERSCORE) {
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
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

            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
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

                //Checks if Affects contains the correct parameters
                if (designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::PROC
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::IF
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::ST_LST
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::WHILE
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::VAR
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::CONST
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::CALL) {
                  return false;
                }

                //Checks if Affects* contains the correct parameters
                if (designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::PROC
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::IF
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::ST_LST
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::WHILE
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::VAR
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::CONST
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::CALL) {
                  return false;
                }

                //check is any design abstraction synonyms contains constant c
                if (g1.getType() == queryType::GType::CONST || g2.getType() == queryType::GType::CONST) {
                  return false;
                }

                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(sTName2);
                if (got == m_synonymMap.end()) {
                  m_synonymMap.insert({ sTName2, 1 });
                } else {
                  m_synonymMap[sTName2]++;
                }
                counterK = 0;
                moveOn = true;
                break;
              } else if (sTInt2 > 0) {
                g2 = Grammar(queryType::GType::STMT_NO, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
                break;
              } else if (sTName2.find('"') != std::string::npos) {

                //Checks if Next/Next*/Affects/AFfects* contains the correct parameters: no string
                if (designAbstractionEntity == "Next"
                  || designAbstractionEntity == "Next*"
                  || designAbstractionEntity == "Affects"
                  || designAbstractionEntity == "Affects*") {
                  return false;
                }

                removeCharsFromString(sTName2, "\\\"");
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
                break;

              } else if (sTName2 == OPERATOR_UNDERSCORE) {
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
                break;
              }
            }
          } else if (sTName1.find('"') != std::string::npos) {

            //Checks if Next/Next*/Affects/AFfects* contains the correct parameters: no string
            if (designAbstractionEntity == "Next"
              || designAbstractionEntity == "Next*"
              || designAbstractionEntity == "Affects"
              || designAbstractionEntity == "Affects*") {
              return false;
            }

            removeCharsFromString(sTName1, "\\\"");
            counterQ = 0;
            g1 = Grammar(queryType::GType::STR, sTName1);
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
                m_relationVectorQE.push_back(DAO);
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
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
                break;
              } else if (sTName2.find('"') != std::string::npos) {

                //Checks if Next/Next*/Affects/AFfects* contains the correct parameters: no string
                if (designAbstractionEntity == "Next"
                  || designAbstractionEntity == "Next*"
                  || designAbstractionEntity == "Affects"
                  || designAbstractionEntity == "Affects*") {
                  return false;
                }

                removeCharsFromString(sTName2, "\\\"");
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                break;
              } else if (sTName2 == OPERATOR_UNDERSCORE) {
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
                break;
              }
            }

          } else if (sTName1 == OPERATOR_UNDERSCORE) {
            if (designAbstractionEntity == "Uses" || designAbstractionEntity == "Modifies") {
              return false;
            }
            counterQ = 0;
            g1 = Grammar(queryType::GType::STR, sTName1);
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

                //Checks if Affects contains the correct parameters
                if (designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::PROC
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::IF
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::ST_LST
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::WHILE
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::VAR
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::CONST
                  || designAbstractionEntity == "Affects" && g2.getType() == queryType::GType::CALL) {
                  return false;
                }

                //Checks if Affects* contains the correct parameters
                if (designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::PROC
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::IF
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::ST_LST
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::WHILE
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::VAR
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::CONST
                  || designAbstractionEntity == "Affects*" && g2.getType() == queryType::GType::CALL) {
                  return false;
                }

                //check is any design abstraction synonyms contains constant c
                if (g1.getType() == queryType::GType::CONST || g2.getType() == queryType::GType::CONST) {
                  return false;
                }

                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
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
                m_relationVectorQE.push_back(DAO);
                break;
              } else if (sTName2.find('"') != std::string::npos) {

                //Checks if Next/Next*/Affects/AFfects* contains the correct parameters: no string
                if (designAbstractionEntity == "Next"
                  || designAbstractionEntity == "Next*"
                  || designAbstractionEntity == "Affects"
                  || designAbstractionEntity == "Affects*") {
                  return false;
                }

                removeCharsFromString(sTName2, "\\\"");
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
                break;
              } else if (sTName2 == OPERATOR_UNDERSCORE) {
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_relationVectorQE.push_back(DAO);
                counterK = 0;
                moveOn = true;
                break;
              }
            }
          }
        }
        if (m_relationVectorQE.size() == 0) {
          return false;
        }
      } //newly added condition checking for properly defined DAO
      else {
        return false;
      }
    }
  }
  
  //creation of pattern object
  if (m_patternVector.empty()) {
    std::cout << "pattern vector is empty" << std::endl;
  } else {
    int counterC = 0;
    for (auto c = m_patternVector.begin(); c != m_patternVector.end(); c++, counterC++) {
      //std::string patternEntity = patternStatement.substr(0, patternStatement.find(WHITESPACE));
      //std::string patternObject = patternStatement.substr(patternStatement.find(WHITESPACE), patternStatement.size());
      std::string patternObject = m_patternVector.at(counterC);

      patternObject = m_stringUtil.reduceString(patternObject);
      patternObject = m_stringUtil.trimString(patternObject);

      std::string delimiterBracket1 = "(";
      if (patternObject.find('(') == std::string::npos || patternObject.find(')') == std::string::npos) {
        return false;
      }
      std::string patternSynonym = patternObject.substr(0, patternObject.find(delimiterBracket1));
      std::string patternParameters = patternObject.substr(patternObject.find(delimiterBracket1), patternObject.size());

      //std::cout << patternSynonym << std::endl;
      patternSynonym = m_stringUtil.trimString(patternSynonym);

      patternParameters = patternParameters.substr(patternParameters.find_first_of('(') + 1, patternParameters.find_last_of(')') - 1);

      std::vector<std::string> patternVector;

      patternVector = stringVectorTokenizer(",;", patternParameters, patternVector);

      //vector to be passed to jazlyn
      std::vector<std::string> patternExpressionVector;

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
        if (patternVector.size() == 2) {
          std::string patternLeftName = patternVector.front();

          patternLeftName = m_stringUtil.trimString(patternLeftName);

          //left side: string
          if (patternLeftName.find('"') != std::string::npos) {
            removeCharsFromString(patternLeftName, "\\\" ");
            grammarPatternLeft = Grammar(queryType::GType::STR, patternLeftName);
          }

          //left side: _
          else if (patternLeftName == OPERATOR_UNDERSCORE) {
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

          char validChars[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
            'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
            'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            '(', ')', '+', '-', '*', '"', '_', ' ',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
          //Check that expression only counters valid arguments: alphanumeric characters, valid operators and brackets
          size_t pos = patternRightName.find_first_not_of(validChars, 0, sizeof(validChars));
          if (pos != std::string::npos) {
            // username contains an invalid character at index pos
            return false;
          }

          //Check if any ) exists before (
          if (patternRightName.find(')') < patternRightName.find('(')) {
            return false;
          }

          //Check for any brackets that are: ()
          std::vector<size_t> positionsOpenBracket; // holds all the positions that sub occurs within str

          std::string patternRightNameTemp = patternRightName;

          //Subtree exists
          if ((patternRightName.find('"') != std::string::npos) && patternRightName.front() == '_' && patternRightName.back() == '_') {
            removeCharsFromString(patternRightName, "\\\" _");
            //grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
            patternExpressionVector = patternVectorTokenizer("()+-*", patternRightName, patternExpressionVector);

            //Checks if the first argument or last argument within the pattern RHS string is an operator
            if (patternExpressionVector.at(0) == OPERATOR_PLUS || patternExpressionVector.back() == OPERATOR_PLUS) {
              return false;
            } else if (patternExpressionVector.at(0) == OPERATOR_MINUS || patternExpressionVector.back() == OPERATOR_MINUS) {
              return false;
            } else if (patternExpressionVector.at(0) == OPERATOR_MULTIPLY || patternExpressionVector.back() == OPERATOR_MULTIPLY) {
              return false;
            }

            //Validation of patterns
            int patCount = 0;
            for (auto pat = patternExpressionVector.begin(); pat != patternExpressionVector.end(); pat++, patCount++) {
              //for cases with only single element(operators are already caugh above)
              if (patternExpressionVector.size() == 1) {
                if (patternExpressionVector.front() == BRACKET_OPEN) {
                  return false;
                } else if (patternExpressionVector.front() == BRACKET_CLOSE) {
                  return false;
                }

                //cases where patternvector size contains more than one element
              } else {
                if (patCount == 0) {
                  //at the start
                  if (patternExpressionVector.at(patCount) == BRACKET_OPEN) {
                    if (patternExpressionVector.at(patCount + 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MULTIPLY) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == BRACKET_CLOSE) {
                      return false;
                    }
                  } else if (patternExpressionVector.at(patCount) == BRACKET_CLOSE) {
                    return false;

                    //everything else, numbers, strings
                  } else {
                    if (patternExpressionVector.at(patCount + 1) == BRACKET_OPEN) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == BRACKET_CLOSE) {
                      return false;
                    }
                  }

                } else if (patCount < patternExpressionVector.size() - 1) {
                  if (patternExpressionVector.at(patCount) == BRACKET_OPEN) {
                    if (patternExpressionVector.at(patCount + 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MULTIPLY) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == BRACKET_CLOSE) {
                      return false;
                    }
                  } else if (patternExpressionVector.at(patCount) == BRACKET_CLOSE) {
                    if (patternExpressionVector.at(patCount - 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount - 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount - 1) == OPERATOR_MULTIPLY) {
                      return false;
                    }
                  } else if (patternExpressionVector.at(patCount) == OPERATOR_PLUS
                    || patternExpressionVector.at(patCount) == OPERATOR_MINUS
                    || patternExpressionVector.at(patCount) == OPERATOR_MULTIPLY) {
                    if (patternExpressionVector.at(patCount + 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MULTIPLY) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == BRACKET_CLOSE) {
                      return false;
                    }
                  }

                } else if (patCount == patternExpressionVector.size() - 1) {
                  if (patternExpressionVector.at(patCount) == BRACKET_OPEN) {
                    return false;
                  } else if (patternExpressionVector.at(patCount) == BRACKET_CLOSE) {
                    if (patternExpressionVector.at(patCount - 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount - 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount - 1) == OPERATOR_MULTIPLY) {
                      return false;
                    }
                  } else if (patternExpressionVector.at(patCount) == OPERATOR_PLUS
                    || patternExpressionVector.at(patCount) == OPERATOR_MINUS
                    || patternExpressionVector.at(patCount) == OPERATOR_MULTIPLY) {
                    return false;
                  }
                }
              }
            }


            grammarPatternRight = Grammar(patternExpressionVector, patternRightName, queryType::GType::STR);
            isSubTree = true;

            //Subtree does not exist
          } else if (patternRightName.find('"') != std::string::npos && patternRightName.front() != '_' && patternRightName.back() != '_') {
            removeCharsFromString(patternRightName, "\\\" ");
            //grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
            patternExpressionVector = patternVectorTokenizer("()+-*", patternRightName, patternExpressionVector);

            //Checks if the first argument or last argument within the pattern RHS string is an operator
            if (patternExpressionVector.at(0) == OPERATOR_PLUS || patternExpressionVector.back() == OPERATOR_PLUS) {
              return false;
            } else if (patternExpressionVector.at(0) == OPERATOR_MINUS || patternExpressionVector.back() == OPERATOR_MINUS) {
              return false;
            } else if (patternExpressionVector.at(0) == OPERATOR_MULTIPLY || patternExpressionVector.back() == OPERATOR_MULTIPLY) {
              return false;
            }

            //Validation of patterns
            int patCount = 0;
            for (auto pat = patternExpressionVector.begin(); pat != patternExpressionVector.end(); pat++, patCount++) {
              //for cases with only single element(operators are already caugh above)
              if (patternExpressionVector.size() == 1) {
                if (patternExpressionVector.front() == BRACKET_OPEN) {
                  return false;
                } else if (patternExpressionVector.front() == BRACKET_CLOSE) {
                  return false;
                }

                //cases where patternvector size contains more than one element
              } else {
                if (patCount == 0) {
                  //at the start
                  if (patternExpressionVector.at(patCount) == BRACKET_OPEN) {
                    if (patternExpressionVector.at(patCount + 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MULTIPLY) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == BRACKET_CLOSE) {
                      return false;
                    }
                  } else if (patternExpressionVector.at(patCount) == BRACKET_CLOSE) {
                    return false;

                    //everything else, numbers, strings
                  } else {
                    if (patternExpressionVector.at(patCount + 1) == BRACKET_OPEN) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == BRACKET_CLOSE) {
                      return false;
                    }
                  }

                } else if (patCount < patternExpressionVector.size() - 1) {
                  if (patternExpressionVector.at(patCount) == BRACKET_OPEN) {
                    if (patternExpressionVector.at(patCount + 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MULTIPLY) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == BRACKET_CLOSE) {
                      return false;
                    }
                  } else if (patternExpressionVector.at(patCount) == BRACKET_CLOSE) {
                    if (patternExpressionVector.at(patCount - 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount - 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount - 1) == OPERATOR_MULTIPLY) {
                      return false;
                    }
                  } else if (patternExpressionVector.at(patCount) == OPERATOR_PLUS
                    || patternExpressionVector.at(patCount) == OPERATOR_MINUS
                    || patternExpressionVector.at(patCount) == OPERATOR_MULTIPLY) {
                    if (patternExpressionVector.at(patCount + 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == OPERATOR_MULTIPLY) {
                      return false;
                    } else if (patternExpressionVector.at(patCount + 1) == BRACKET_CLOSE) {
                      return false;
                    }
                  }

                } else if (patCount == patternExpressionVector.size() - 1) {
                  if (patternExpressionVector.at(patCount) == BRACKET_OPEN) {
                    return false;
                  } else if (patternExpressionVector.at(patCount) == BRACKET_CLOSE) {
                    if (patternExpressionVector.at(patCount - 1) == OPERATOR_PLUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount - 1) == OPERATOR_MINUS) {
                      return false;
                    } else if (patternExpressionVector.at(patCount - 1) == OPERATOR_MULTIPLY) {
                      return false;
                    }
                  } else if (patternExpressionVector.at(patCount) == OPERATOR_PLUS
                    || patternExpressionVector.at(patCount) == OPERATOR_MINUS
                    || patternExpressionVector.at(patCount) == OPERATOR_MULTIPLY) {
                    return false;
                  }
                }
              }
            }


            grammarPatternRight = Grammar(patternExpressionVector, patternRightName, queryType::GType::STR);
          } else if (patternRightName == OPERATOR_UNDERSCORE) {
            grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
          } else {
            return false;
          }
        }
        //while patterns
      } else if (patternOfGrammar.getType() == queryType::GType::WHILE) {
        if (patternVector.size() == 2) {
          std::string patternLeftName = patternVector.front();
          if (patternLeftName.find('"') != std::string::npos) {
            removeCharsFromString(patternLeftName, "\\\" ");
            grammarPatternLeft = Grammar(queryType::GType::STR, patternLeftName);
            patternLeftName = m_stringUtil.trimString(patternLeftName);
          } else if (patternLeftName == OPERATOR_UNDERSCORE) {
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
          if (patternRightName == OPERATOR_UNDERSCORE) {
            grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
          } else {
            return false;
          }

        } else {
          return false;
        }
        //if patterns
      } else if (patternOfGrammar.getType() == queryType::GType::IF) {
        std::string ifPatternParam1;
        std::string ifPatternParam2;
        //check there are 3 parameters for if pattern clause
        if (patternVector.size() == 3) {

          ifPatternParam1 = patternVector.at(1);
          ifPatternParam2 = patternVector.at(2);

          ifPatternParam1 = m_stringUtil.trimString(ifPatternParam1);
          ifPatternParam2 = m_stringUtil.trimString(ifPatternParam2);

          //check to make sure both pattern parameters are _
          if (ifPatternParam1 != OPERATOR_UNDERSCORE || ifPatternParam2 != OPERATOR_UNDERSCORE) {
            return false;
          }

          std::string patternLeftName = patternVector.front();
          if (patternLeftName.find('"') != std::string::npos) {
            removeCharsFromString(patternLeftName, "\\\" ");
            grammarPatternLeft = Grammar(queryType::GType::STR, patternLeftName);
          }

          else if (patternLeftName == OPERATOR_UNDERSCORE) {
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

          std::string patternRightName = ifPatternParam2;
          patternRightName = m_stringUtil.trimString(patternRightName);
          if (patternRightName == OPERATOR_UNDERSCORE) {
            grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
          } else {
            return false;
          }
        } else {
          return false;
        }
      } else {
        return false;
      }

      //create pattern object
      Pattern patternObjectCreated(patternOfGrammar, grammarPatternLeft, grammarPatternRight, isSubTree);
      m_patternVectorQE.push_back(patternObjectCreated);

    }
  }

  //creation of with object
  if (m_withVector.empty()) {
    std::cout << "with vector is empty" << std::endl;
  } else {
    int counterD=0;
    for (auto d = m_withVector.begin(); d != m_withVector.end(); d++, counterD++) {
      //withStatement = m_stringUtil.trimString(withStatement);
      //std::string withObject = withStatement.substr(withStatement.find(WHITESPACE), withStatement.size());
      std::string withObject = m_withVector.at(counterD);
      withObject = m_stringUtil.reduceString(withObject);
      withObject = m_stringUtil.trimString(withObject);

      //Check if with expression does not contains =
      if (withObject.find('=') == std::string::npos) {
        return false;
      }

      std::string withLeft = withObject.substr(0, withObject.find(OPERATOR_EQUAL));
      std::string withRight = withObject.substr(withObject.find(OPERATOR_EQUAL) + 1, withObject.size());

      withLeft = m_stringUtil.trimString(withLeft);
      withRight = m_stringUtil.trimString(withRight);

      //Check if with expression is empty on either side =
      if (withLeft == "" || withRight == "") {
        return false;
      }

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
        //Case 1: Check orientation of parameters: left integer, right attribute
      if (withLeftInt > 0 && withRight.find('.') != std::string::npos) {
        isWithTrue = withClauseAttNum(withRight, withLeft, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 2: left string, right attribute
      } else if (withLeft.find('"') != std::string::npos && withRight.find('.') != std::string::npos && withLeftInt == 0 && withRightInt == 0) {
        isWithTrue = withClauseAttString(withRight, withLeft, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 3: left attribute, right attribute
      } else if (withLeft.find('.') != std::string::npos && withRight.find('.') != std::string::npos) {
        isWithTrue = withClauseAttAtt(withLeft, withRight, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 4: left attribute, right integer
      } else if (withLeft.find('.') != std::string::npos && withRightInt > 0) {
        isWithTrue = withClauseAttNum(withLeft, withRight, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 5: left attribute, right string
      } else if (withLeft.find('.') != std::string::npos && withRight.find('"') != std::string::npos && withLeftInt == 0 && withRightInt == 0) {
        isWithTrue = withClauseAttString(withLeft, withRight, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 6: SPECIAL CASE synonym with no attributes used. number = pl
      } else if (withLeftInt > 0 && withRight.find('"') == std::string::npos && withRight.find('.') == std::string::npos && withRightInt == 0) {
        isWithTrue = withClauseAttNumNoSynonymAtt(withRight, withLeft, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 7: SPECIAL CASE synonym with no attributes used. pl = number
      } else if (withRightInt > 0 && withLeft.find('"') == std::string::npos && withLeft.find('.') == std::string::npos && withLeftInt == 0) {
        isWithTrue = withClauseAttNumNoSynonymAtt(withLeft, withRight, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 8: SPECIAL CASE left syn no attribute, right syn attribute
      } else if (withLeft.find('"') == std::string::npos && withLeft.find('.') == std::string::npos && withRight.find('.') != std::string::npos && withLeftInt == 0 && withRightInt == 0) {
        isWithTrue = withClauseSynAtt(withLeft, withRight, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 9: SPECIAL CASE left syn attribute, right syn no attribute
      } else if (withRight.find('"') == std::string::npos && withRight.find('.') == std::string::npos && withLeft.find('.') != std::string::npos && withLeftInt == 0 && withRightInt == 0) {
        isWithTrue = withClauseSynAtt(withRight, withLeft, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 10: SPECIAL CASE both sides pl synonyms
      } else if (withLeft.find('"') == std::string::npos && withLeft.find('.') == std::string::npos && withRight.find('"') == std::string::npos && withRight.find('.') == std::string::npos && withLeftInt == 0 && withRightInt == 0) {
        isWithTrue = withClauseSynSyn(withLeft, withRight, withLeftGrammar, withRightGrammar);
        if (isWithTrue == false) {
          return false;
        }
        //Case 11: Both sides strings
      } else if (withLeft.find('"') != std::string::npos && withRight.find('"') != std::string::npos && withLeftInt == 0 && withRightInt == 0) {
        if (withLeft != withRight) {
          return false;
        } else if (withLeft == withRight) {
          removeCharsFromString(withLeft, "\\\" ");
          removeCharsFromString(withRight, "\\\" ");
          withLeftGrammar = Grammar(queryType::GType::STR, withLeft);
          withRightGrammar = Grammar(queryType::GType::STR, withRight);

          With withObjectCreated(withLeftGrammar, withRightGrammar);
          m_withVectorQE.push_back(withObjectCreated);
        }
        //Case 12: Both sides integers
      } else if (withLeftInt > 0 && withRightInt > 0) {
        if (withLeftInt != withRightInt) {
          return false;
        } else if (withLeftInt == withRightInt) {
          withLeftGrammar = Grammar(queryType::GType::STMT_NO, withLeft);
          withRightGrammar = Grammar(queryType::GType::STMT_NO, withRight);

          With withObjectCreated(withLeftGrammar, withRightGrammar);
          m_withVectorQE.push_back(withObjectCreated);

        }
        //Case 13: string left side int right side
      } else if (withLeftInt > 0 && withRight.find('"') != std::string::npos) {
        return false;
        //Case 14: int right side string left side
      } else if (withRightInt > 0 && withLeft.find('"') != std::string::npos) {
        return false;
        //Case 15: all other cases
      } else {
        return false;
      }
    }
  }

  isTokenized = true;
  return isTokenized;
}

void QueryPreProcessor::removeCharsFromString(std::string &t_str, char* t_charsToRemove) {
  for (unsigned int i = 0; i < strlen(t_charsToRemove); ++i) {
    t_str.erase(remove(t_str.begin(), t_str.end(), t_charsToRemove[i]), t_str.end());
  }
}

std::vector<std::string> QueryPreProcessor::stringVectorTokenizer(char* t_charsToRemove, std::string t_targetString, std::vector<std::string> t_vector) {
  int pos1;
  int prev_pos_new = 0;

  while ((pos1 = t_targetString.find_first_of(t_charsToRemove, prev_pos_new)) != std::string::npos) {
    if (pos1 > prev_pos_new) {
      t_vector.push_back(t_targetString.substr(prev_pos_new, pos1 - prev_pos_new));
    }
    prev_pos_new = pos1 + 1;
  }
  if (prev_pos_new < t_targetString.length()) {
    t_vector.push_back(t_targetString.substr(prev_pos_new, std::string::npos));
  }
  return t_vector;
}

std::vector<std::string> QueryPreProcessor::patternVectorTokenizer(char* t_charsToRemove, std::string t_patternRightName, std::vector<std::string> t_vector) {
  int pos1 = 0;
  int prev_pos_new = 0;
  std::string c;

  while ((pos1 = t_patternRightName.find_first_of(t_charsToRemove, prev_pos_new)) != std::string::npos) {
    if (prev_pos_new == 0) {
      c = t_patternRightName.at(prev_pos_new);
      if (c == "+") {
        t_vector.push_back(c);
      } else if (c == "-") {
        t_vector.push_back(c);
      } else if (c == "*") {
        t_vector.push_back(c);
      } else if (c == "(") {
        t_vector.push_back(c);
      } else if (c == ")") {
        t_vector.push_back(c);
      }

    }
    if (pos1 > prev_pos_new) {
      t_vector.push_back(t_patternRightName.substr(prev_pos_new, pos1 - prev_pos_new));
      c = t_patternRightName.at(pos1);
      if (c == "+") {
        t_vector.push_back(c);
      } else if (c == "-") {
        t_vector.push_back(c);
      } else if (c == "*") {
        t_vector.push_back(c);
      } else if (c == "(") {
        t_vector.push_back(c);
      } else if (c == ")") {
        t_vector.push_back(c);
      }

    //character next to previous token is a bracket, operator
    } else if (pos1 = prev_pos_new) {
      c = t_patternRightName.at(pos1);
      if (c == "+") {
        t_vector.push_back(c);
      } else if (c == "-") {
        t_vector.push_back(c);
      } else if (c == "*") {
        t_vector.push_back(c);
      } else if (c == "(") {
        t_vector.push_back(c);
      } else if (c == ")") {
        t_vector.push_back(c);
      }
    } 
    prev_pos_new = pos1 + 1;
  }
  if (prev_pos_new < t_patternRightName.length()) {
    t_vector.push_back(t_patternRightName.substr(prev_pos_new, std::string::npos));
  }
  return t_vector;
}

bool QueryPreProcessor::withClauseAttNum(std::string t_attribute, std::string t_integer, Grammar t_withLeftGrammar, Grammar t_withRightGrammar) {
  std::string tempAttribute = t_attribute;
  std::string withTempSynonym = tempAttribute.substr(0, tempAttribute.find("."));
  std::string withTempAttribute = tempAttribute.substr(tempAttribute.find(".") + 1, tempAttribute.size());

  withTempSynonym = m_stringUtil.trimString(withTempSynonym);
  withTempAttribute = m_stringUtil.trimString(withTempAttribute);

  t_withLeftGrammar = withAttributeProcessor(t_attribute, t_withLeftGrammar);

  if (t_withLeftGrammar.getAttr() == queryType::AType::INVALID) {
    return false;
  }
  
  //Case 2.1: GType:Stmt, asgn, while, if, call, GType: Stmt# attribute
  if (t_withLeftGrammar.getType() == queryType::GType::STMT && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::ASGN && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::WHILE && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::IF && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::CALL && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::PROG_LINE
    ) {

    t_withRightGrammar = Grammar(queryType::GType::STMT_NO, t_integer);

    //Case 2.2: GType: CONSTANT, AType: VALUE
  } else if (t_withLeftGrammar.getType() == queryType::GType::CONST
    && t_withLeftGrammar.getAttr() == queryType::AType::VALUE) {

    t_withRightGrammar = Grammar(queryType::GType::STMT_NO, t_integer);
  }
  if (t_withLeftGrammar.getAttr() == queryType::AType::VALUE && t_withLeftGrammar.getType() == queryType::GType::CONST
    || t_withLeftGrammar.getType() == queryType::GType::STMT && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::ASGN && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::WHILE && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::IF && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::CALL && t_withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || t_withLeftGrammar.getType() == queryType::GType::PROG_LINE && withTempAttribute == "") {
    With withObjectCreated(t_withLeftGrammar, t_withRightGrammar); 
    m_withVectorQE.push_back(withObjectCreated);

    return true;
  } else {
    return false;
  }
}

bool QueryPreProcessor::withClauseAttNumNoSynonymAtt(std::string t_withSynonym, std::string t_integer, Grammar t_withLeftGrammar, Grammar t_withRightGrammar) {

  int counterS = 0;
  for (auto s = m_grammarVector.begin(); s != m_grammarVector.end(); s++, counterS++) {
    if (m_grammarVector.at(counterS).getName() == t_withSynonym) {
      t_withLeftGrammar = Grammar(m_grammarVector.at(counterS).getType(), t_withSynonym);
    }
  }

  t_withRightGrammar = Grammar(queryType::GType::STMT_NO, t_integer);

  
  if(t_withLeftGrammar.getType() == queryType::GType::PROG_LINE) {
    With withObjectCreated(t_withLeftGrammar, t_withRightGrammar);
    m_withVectorQE.push_back(withObjectCreated);

    std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(t_withSynonym);
    if (got == m_synonymMap.end()) {
      m_synonymMap.insert({ t_withSynonym, 1 });
    } else {
      m_synonymMap[t_withSynonym]++;
    }
    return true;
  } else {
    return false;
  }
}

bool QueryPreProcessor::withClauseAttString(std::string t_attribute, std::string t_inputString, Grammar t_withLeftGrammar, Grammar t_withRightGrammar) {
  std::string tempAttribute = t_attribute;
  std::string withTempSynonym = tempAttribute.substr(0, tempAttribute.find("."));

  std::string withTempAttribute = tempAttribute.substr(tempAttribute.find(".") + 1, tempAttribute.size());

  withTempSynonym = m_stringUtil.trimString(withTempSynonym);
  withTempAttribute = m_stringUtil.trimString(withTempAttribute);

  t_withLeftGrammar = withAttributeProcessor(t_attribute, t_withLeftGrammar);

  if (t_withLeftGrammar.getAttr() == queryType::AType::INVALID) {
    return false;
  }

  removeCharsFromString(t_inputString, "\"");
  t_inputString = m_stringUtil.trimString(t_inputString);
  t_withRightGrammar = Grammar(queryType::GType::STR, t_inputString);

  if (t_withLeftGrammar.getAttr() == queryType::AType::PROC_NAME && t_withLeftGrammar.getType() == queryType::GType::PROC
    || t_withLeftGrammar.getAttr() == queryType::AType::PROC_NAME && t_withLeftGrammar.getType() == queryType::GType::CALL
    || t_withLeftGrammar.getAttr() == queryType::AType::VAR_NAME && t_withLeftGrammar.getType() == queryType::GType::VAR) {
    With withObjectCreated(t_withLeftGrammar, t_withRightGrammar);
    m_withVectorQE.push_back(withObjectCreated);

    return true;
  } else {
    return false;
  }
}

bool QueryPreProcessor::withClauseAttAtt(std::string t_leftAttribute, std::string t_rightAttribute, Grammar t_withLeftGrammar, Grammar t_withRightGrammar) {
  t_withLeftGrammar = withAttributeProcessor(t_leftAttribute, t_withLeftGrammar);
  t_withRightGrammar = withAttributeProcessor(t_rightAttribute, t_withRightGrammar);

  With withObjectCreated;

  if (t_withLeftGrammar.getAttr() == queryType::AType::INVALID || t_withRightGrammar.getAttr() == queryType::AType::INVALID) {
    return false;
  }

  withObjectCreated = With(t_withLeftGrammar, t_withRightGrammar);
  m_withVectorQE.push_back(withObjectCreated);


  return true;
  
}

bool QueryPreProcessor::withClauseSynAtt(std::string t_leftSynonym, std::string t_rightSynonym, Grammar t_withLeftGrammar, Grammar t_withRightGrammar) {
  int counterS = 0;
  for (auto s = m_grammarVector.begin(); s != m_grammarVector.end(); s++, counterS++) {
    if (m_grammarVector.at(counterS).getName() == t_leftSynonym) {
      t_withLeftGrammar = Grammar(m_grammarVector.at(counterS).getType(), t_leftSynonym);
    }
  }

  t_withRightGrammar = withAttributeProcessor(t_rightSynonym, t_withRightGrammar);

  if (t_withRightGrammar.getAttr() == queryType::AType::INVALID) {
    return false;
  }

  if (t_withLeftGrammar.getType() == queryType::GType::PROG_LINE) {
    With withObjectCreated(t_withLeftGrammar, t_withRightGrammar);
    m_withVectorQE.push_back(withObjectCreated);

    std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(t_leftSynonym);
    if (got == m_synonymMap.end()) {
      m_synonymMap.insert({ t_leftSynonym, 1 });
    } else {
      m_synonymMap[t_leftSynonym]++;
    }
    return true;
  } else {
    return false;
  }

  With withObjectCreated(t_withLeftGrammar, t_withRightGrammar);
  m_withVectorQE.push_back(withObjectCreated);

}

bool QueryPreProcessor::withClauseSynSyn(std::string t_leftSynonym, std::string t_rightSynonym, Grammar t_withLeftGrammar, Grammar t_withRightGrammar) {
  int counterS = 0;
  for (auto s = m_grammarVector.begin(); s != m_grammarVector.end(); s++, counterS++) {
    if (m_grammarVector.at(counterS).getName() == t_leftSynonym) {
      t_withLeftGrammar = Grammar(m_grammarVector.at(counterS).getType(), t_leftSynonym);
    }
  }

  int counterS2 = 0;
  for (auto s2 = m_grammarVector.begin(); s2 != m_grammarVector.end(); s2++, counterS2++) {
    if (m_grammarVector.at(counterS2).getName() == t_rightSynonym) {
      t_withRightGrammar = Grammar(m_grammarVector.at(counterS2).getType(), t_rightSynonym);
    }
  }

  if (t_withLeftGrammar.getType() == queryType::GType::PROG_LINE && t_withRightGrammar.getType() == queryType::GType::PROG_LINE) {
    With withObjectCreated(t_withLeftGrammar, t_withRightGrammar);
    m_withVectorQE.push_back(withObjectCreated);

    std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(t_leftSynonym);
    if (got == m_synonymMap.end()) {
      m_synonymMap.insert({ t_leftSynonym, 1 });
    } else {
      m_synonymMap[t_leftSynonym]++;
    }
    got = m_synonymMap.find(t_rightSynonym);
    if (got == m_synonymMap.end()) {
      m_synonymMap.insert({ t_rightSynonym, 1 });
    } else {
      m_synonymMap[t_rightSynonym]++;
    }
    return true;
  } else {
    return false;
  }

  With withObjectCreated(t_withLeftGrammar, t_withRightGrammar);
  m_withVectorQE.push_back(withObjectCreated);

}

Grammar QueryPreProcessor::withAttributeProcessor(std::string t_attribute, Grammar t_withGrammar) {
  std::string withSynonym = t_attribute.substr(0, t_attribute.find("."));
  std::string withAttribute = t_attribute.substr(t_attribute.find(".") + 1, t_attribute.size());

  withSynonym = m_stringUtil.trimString(withSynonym);
  withAttribute = m_stringUtil.trimString(withAttribute);

  int counterS = 0;
  for (auto s = m_grammarVector.begin(); s != m_grammarVector.end(); s++, counterS++) {
    if (m_grammarVector.at(counterS).getName() == withSynonym) {
      if (withAttribute == PROCNAME) {
        t_withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        t_withGrammar.setAType(queryType::AType::PROC_NAME);
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(withSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ withSynonym, 1 });
        } else {
          m_synonymMap[withSynonym]++;
        }
      } else if (withAttribute == VARNAME) {
        t_withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        t_withGrammar.setAType(queryType::AType::VAR_NAME);
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(withSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ withSynonym, 1 });
        } else {
          m_synonymMap[withSynonym]++;
        }
      } else if (withAttribute == VALUE) {
        t_withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        t_withGrammar.setAType(queryType::AType::VALUE);
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(withSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ withSynonym, 1 });
        } else {
          m_synonymMap[withSynonym]++;
        }
      } else if (withAttribute == STMT_NO) {
        t_withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        t_withGrammar.setAType(queryType::AType::STMT_NUM);
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(withSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ withSynonym, 1 });
        } else {
          m_synonymMap[withSynonym]++;
        }
      } else {
        t_withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        t_withGrammar.setAType(queryType::AType::INVALID);
      }
    }
  }
  return t_withGrammar;
}

std::string QueryPreProcessor::multiClauseProcessor(std::string t_secondStatement, std::string t_whichClause) {
  std::string delimiterSelect = "Select";
  std::string delimiterSuchThat = "such that";
  std::string delimiterPattern = "pattern";
  std::string delimiterWith = "with";

  std::string tempStatement;

  t_secondStatement = t_secondStatement.substr(t_secondStatement.find(WHITESPACE), t_secondStatement.size());

  t_secondStatement = t_secondStatement.substr(t_secondStatement.find_first_not_of(WHITESPACE), t_secondStatement.size());

  //case 1: where with, such that, pattern still exists and such that is the next token
  if (t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find(delimiterWith)
    && t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find(delimiterPattern)
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterSuchThat));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterSuchThat), t_secondStatement.size());

    //Case 2: where with, such that, pattern still exists and pattern is the next token
  } else if (t_secondStatement.find(delimiterPattern) < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find(delimiterPattern) < t_secondStatement.find(delimiterWith)
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterPattern));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterPattern), t_secondStatement.size());

    //Case 3: where with, such that, pattern still exists and with is the next token
  } else if (t_secondStatement.find(delimiterWith) < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find(delimiterWith) < t_secondStatement.find(delimiterPattern)
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterWith));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterWith), t_secondStatement.size());
    

    //Case 4: where with, such that only exists and such that is the next token
  } else if (t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find(delimiterWith)
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterSuchThat));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterSuchThat), t_secondStatement.size());
    

    //Case 5: where with, such that only exists and with is the next token
  } else if (t_secondStatement.find(delimiterWith) < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterWith));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterWith), t_secondStatement.size());
    

    //Case 6: where with, pattern only exists and pattern is the next token
  } else if (t_secondStatement.find(delimiterPattern) < t_secondStatement.find(delimiterWith)
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterPattern));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterPattern), t_secondStatement.size());
    

    //Case 7: where with, pattern only exists and with is the next token
  } else if (t_secondStatement.find(delimiterWith) < t_secondStatement.find(delimiterPattern)
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterWith));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterWith), t_secondStatement.size());
    

    //Case 8: where such that, pattern only exists and pattern is the next token
  } else if (t_secondStatement.find(delimiterPattern) < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterPattern));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterPattern), t_secondStatement.size());
    

    //Case 9: where such that, pattern only exists and such that is the next token
  } else if (t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find(delimiterPattern)
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterSuchThat));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterSuchThat), t_secondStatement.size());
    

    //Case 10: where such that exists only
  } else if (t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterSuchThat));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterSuchThat), t_secondStatement.size());
    

    //Case 11: where pattern exists only
  } else if (t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterPattern));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterPattern), t_secondStatement.size());
    

    //Case 12: where with exists only
  } else if (t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") == std::string::npos) {

    tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterWith));
    tempStatement = m_stringUtil.trimString(tempStatement);
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
    }
    t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterWith), t_secondStatement.size());
    

    //Case 13: where and exists after pattern and there is such that, pattern, with after it
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterPattern)
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterWith)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find("and"));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find("and"), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 14: where and exists and such that and pattern exists and and is the next token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterPattern)
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterWith)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find("and"));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find("and"), t_secondStatement.size());

    } else {
      return t_secondStatement;
    }

    //Case 15: where and exists and such that and pattern exists and with and such that next token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find("and")
    && t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find(delimiterPattern)
    && t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find(delimiterWith)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterSuchThat));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterSuchThat), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 16: where and exists and such that and pattern and with exists and pattern is the next token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find(delimiterPattern) < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find(delimiterPattern) < t_secondStatement.find("and")
    && t_secondStatement.find(delimiterPattern) < t_secondStatement.find(delimiterWith)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterPattern));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterPattern), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 17: where and exists and such that and pattern exists and with and with is the next token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find(delimiterWith) < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find(delimiterWith) < t_secondStatement.find(delimiterPattern)
    && t_secondStatement.find(delimiterWith) < t_secondStatement.find("and")) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterWith));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterWith), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 18: where and and such that and pattern exists and and is the first token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterPattern)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find("and"));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find("and"), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 19: where and and such that and pattern exists and such that is the first token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find("and")
    && t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find(delimiterPattern)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterSuchThat));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterSuchThat), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 20: where and and such that and pattern exists and pattern is the first token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find(delimiterPattern) < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find(delimiterPattern) < t_secondStatement.find("and")) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterPattern));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterPattern), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 21: where and and such that and with exists and and is the first token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterSuchThat)
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterWith)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find("and"));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find("and"), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 22: where and and such that and with exists and such that is the first token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find("and")
    && t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find(delimiterWith)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterSuchThat));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterSuchThat), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 23: where and and such that and with exists and with is the first token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find(delimiterWith) < t_secondStatement.find("and")
    && t_secondStatement.find(delimiterWith) < t_secondStatement.find(delimiterSuchThat)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterWith));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterWith), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 24: where and and pattern and with exists and and is the first token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterPattern)
    && t_secondStatement.find("and") < t_secondStatement.find(delimiterWith)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find("and"));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find("and"), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 25: where and and pattern and with exists and pattern is the first token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find(delimiterPattern) < t_secondStatement.find("and")
    && t_secondStatement.find(delimiterPattern) < t_secondStatement.find(delimiterWith)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterPattern));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterPattern), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 26: where and and pattern and with exists and with is the first token
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find(delimiterWith) < t_secondStatement.find("and")
    && t_secondStatement.find(delimiterWith) < t_secondStatement.find(delimiterPattern)) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterWith));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterWith), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 27: where and and such that exists only and such that first
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find(delimiterSuchThat) < t_secondStatement.find("and")) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterSuchThat));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterSuchThat), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 28: where and and such that exists only and and first
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) != std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find(delimiterSuchThat) > t_secondStatement.find("and")) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find("and"));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find("and"), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 29: where and and pattern exists only and and first
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find(delimiterPattern) > t_secondStatement.find("and")) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find("and"));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find("and"), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 30: where and and pattern exists only and pattern first
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) != std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos
    && t_secondStatement.find(delimiterPattern) < t_secondStatement.find("and")) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterPattern));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterPattern), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 31: where and and with exists only and with first
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find(delimiterWith) < t_secondStatement.find("and")) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find(delimiterWith));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find(delimiterWith), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 32: where and and with exists only and and first
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) != std::string::npos
    && t_secondStatement.find(delimiterWith) > t_secondStatement.find("and")) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find("and"));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find("and"), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 32: where and exists only
  } else if (t_secondStatement.find("and") != std::string::npos
    && t_secondStatement.find(delimiterSuchThat) == std::string::npos
    && t_secondStatement.find(delimiterPattern) == std::string::npos
    && t_secondStatement.find(delimiterWith) == std::string::npos) {

    int andValidate = 0;

    andValidate = t_secondStatement.find("and");

    if (t_secondStatement.at(andValidate - 1) == ')' || t_secondStatement.at(andValidate - 1) == ' ') {

      tempStatement = t_secondStatement.substr(0, t_secondStatement.find("and"));
      tempStatement = m_stringUtil.trimString(tempStatement);
      if (t_whichClause == "pattern") {
        m_patternVector.push_back(tempStatement);
        m_prevClause = delimiterPattern;
      } else if (t_whichClause == "with") {
        m_withVector.push_back(tempStatement);
        m_prevClause = delimiterWith;
      } else if (t_whichClause == "such that") {
        m_relationVector.push_back(tempStatement);
        m_prevClause = delimiterSuchThat;
      }
      t_secondStatement = t_secondStatement.substr(t_secondStatement.find("and"), t_secondStatement.size());
    } else {
      return t_secondStatement;
    }

    //Case 33: no more other clause behind
  } else {
    tempStatement = t_secondStatement;
    if (t_whichClause == "pattern") {
      m_patternVector.push_back(tempStatement);
      m_prevClause = delimiterPattern;
      m_secondStatementTemp = "";
      t_secondStatement = "";
    } else if (t_whichClause == "with") {
      m_withVector.push_back(tempStatement);
      m_prevClause = delimiterWith;
      m_secondStatementTemp = "";
      t_secondStatement = "";
    } else if (t_whichClause == "such that") {
      m_relationVector.push_back(tempStatement);
      m_prevClause = delimiterSuchThat;
      m_secondStatementTemp = "";
      t_secondStatement = "";
    }
  }

  return t_secondStatement;
}

/**
* A boolean method for checking whether "Select BOOLEAN" exists or not.
* @return true if exists, false if not.
*/
BOOLEAN QueryPreProcessor::isSelectBoolean(void) {
  return m_selectBOOLEANExists;
}