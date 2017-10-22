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

std::queue<Grammar> QueryPreProcessor::getSelect(void) {
  return m_selectQueue;
}

std::queue<Relation> QueryPreProcessor::getSuchThat(void) {
  return m_suchThatQueue;
}

std::queue<Pattern> QueryPreProcessor::getPattern(void) {
  return m_patternQueue;
}

std::queue<With> QueryPreProcessor::getWith(void) {
  return m_withQueue;
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
  declaration = m_stringUtil.reduceString(declaration);
  return declaration;
}

std::string QueryPreProcessor::splitStringQuery(std::string t_Input) {
  std::string delimiter = "Select";
  std::string query = t_Input.substr(t_Input.find(delimiter), t_Input.size()); //same for this as delimiter is "; Select"

  query = m_stringUtil.trimString(query);
  query = m_stringUtil.reduceString(query);
  return query;
}


BOOLEAN QueryPreProcessor::tokenizeDeclaration(std::string t_declarationInput) {
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
          } else if (entity == BOOLEAN_QPP) {
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

BOOLEAN QueryPreProcessor::tokenizeQuery(std::string t_queryInput) {
  bool isTokenized = false;
  std::vector<std::string> queryVector;
  std::string selectStatement;
  std::string suchThatStatement;
  std::string patternStatement;
  std::string delimiterSpace = " ";
  std::string withStatement;
  std::string delimiterSelect = "Select";
  std::string delimiterSuchThat = "such that";
  std::string delimiterPattern = "pattern";
  std::string delimiterWith = "with";
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
  if (t_queryInput.back() == ';') {
    return false;
  }

  //New Method:
  //Case 1: only select statements
  if (t_queryInput.find(delimiterSuchThat) == std::string::npos && t_queryInput.find(delimiterPattern) == std::string::npos && t_queryInput.find(delimiterWith) == std::string::npos) {
    selectStatement = t_queryInput;
    m_selectVector.push_back(selectStatement);
  } else {
    int tempSpaceLoc;

    selectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSpace));
    t_queryInput = t_queryInput.substr(t_queryInput.find(delimiterSpace));
    t_queryInput = t_queryInput.substr(t_queryInput.find_first_not_of(delimiterSpace), t_queryInput.find_last_not_of(delimiterSpace));
    tempSpaceLoc = t_queryInput.find(delimiterSpace);
    std::string tempSelectStatement = t_queryInput.substr(0, t_queryInput.find(delimiterSpace));
    selectStatement.append(" ");
    selectStatement.append(tempSelectStatement);
    secondStatement = t_queryInput.substr(tempSpaceLoc, t_queryInput.size());
    //secondStatement = t_queryInput.substr(secondStatement.find_first_not_of(delimiterSpace) + 1, secondStatement.size());
    secondStatement = m_stringUtil.trimString(secondStatement);

    while (true) {
      //secondStatement = m_stringUtil.trimString(secondStatement);
      secondTempStatement = secondStatement.substr(0, secondStatement.find(delimiterSpace));
      if (secondTempStatement == "such") {
        secondStatement = secondStatement.substr(secondStatement.find(delimiterSpace), secondStatement.size());
        secondStatement = secondStatement.substr(secondStatement.find_first_not_of(delimiterSpace));
        secondTempStatement = secondStatement.substr(0, secondStatement.find(delimiterSpace));
        if (secondTempStatement == "that") {
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSpace), secondStatement.size());
         
          secondStatement = secondStatement.substr(secondStatement.find_first_not_of(delimiterSpace), secondStatement.size());
          suchThatStatement = secondStatement.substr(0, secondStatement.find(BRACKET_CLOSE) + 1);
          m_relationVector.push_back(suchThatStatement);

          secondStatement = secondStatement.substr(secondStatement.find(BRACKET_CLOSE) + 1, secondStatement.size());
          if (secondStatement == "") {
            break;
          }
          //
          secondStatement = secondStatement.substr(secondStatement.find_first_not_of(delimiterSpace), secondStatement.size());
          prevClause = delimiterSuchThat;

        }

      } else if (secondTempStatement == "pattern") {
        secondStatement = secondStatement.substr(secondStatement.find(delimiterSpace), secondStatement.size());

        secondStatement = secondStatement.substr(secondStatement.find_first_not_of(delimiterSpace), secondStatement.size());

        //case 1: where with, such that, pattern still exists and such that is the next token
        if (secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterWith)
          && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterPattern)
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 2: where with, such that, pattern still exists and pattern is the next token
        } else if (secondStatement.find(delimiterPattern) < secondStatement.find(delimiterSuchThat)
          && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterWith)
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 3: where with, such that, pattern still exists and with is the next token
        } else if (secondStatement.find(delimiterWith) < secondStatement.find(delimiterSuchThat)
          && secondStatement.find(delimiterWith) < secondStatement.find(delimiterPattern)
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 4: where with, such that only exists and such that is the next token
        } else if (secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterWith)
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 5: where with, such that only exists and with is the next token
        } else if (secondStatement.find(delimiterWith) < secondStatement.find(delimiterSuchThat)
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 6: where with, pattern only exists and pattern is the next token
        } else if (secondStatement.find(delimiterPattern) < secondStatement.find(delimiterWith)
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 7: where with, pattern only exists and with is the next token
        } else if (secondStatement.find(delimiterWith) < secondStatement.find(delimiterPattern)
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 8: where such that, pattern only exists and pattern is the next token
        } else if (secondStatement.find(delimiterPattern) < secondStatement.find(delimiterSuchThat)
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 9: where such that, pattern only exists and such that is the next token
        } else if (secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterPattern)
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 10: where such that exists only
        } else if (secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 11: where pattern exists only
        } else if (secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 12: where with exists only
        } else if (secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 13: where and exists after pattern and there is such that, pattern, with after it
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") < secondStatement.find(delimiterSuchThat)
          && secondStatement.find("and") < secondStatement.find(delimiterPattern)
          && secondStatement.find("and") < secondStatement.find(delimiterWith)) {

          patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 14: where and exists and such that and pattern exists and and is the next token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") < secondStatement.find(delimiterSuchThat)
          && secondStatement.find("and") < secondStatement.find(delimiterPattern)
          && secondStatement.find("and") < secondStatement.find(delimiterWith)) {

          patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 15: where and exists and such that and pattern exists and with and such that next token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find(delimiterSuchThat) < secondStatement.find("and")
          && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterPattern)
          && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterWith)) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 16: where and exists and such that and pattern and with exists and pattern is the next token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterSuchThat)
          && secondStatement.find(delimiterPattern) < secondStatement.find("and")
          && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterWith)) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 17: where and exists and such that and pattern exists and with and with is the next token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find(delimiterWith) < secondStatement.find(delimiterSuchThat)
          && secondStatement.find(delimiterWith) < secondStatement.find(delimiterPattern)
          && secondStatement.find(delimiterWith) < secondStatement.find("and")) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 18: where and and such that and pattern exists and and is the first token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find("and") < secondStatement.find(delimiterSuchThat)
          && secondStatement.find("and") < secondStatement.find(delimiterPattern)) {

          patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 19: where and and such that and pattern exists and such that is the first token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find(delimiterSuchThat) < secondStatement.find("and")
          && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterPattern)) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 20: where and and such that and pattern exists and pattern is the first token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterSuchThat)
          && secondStatement.find(delimiterPattern) < secondStatement.find("and")) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 21: where and and such that and with exists and and is the first token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") < secondStatement.find(delimiterSuchThat)
          && secondStatement.find("and") < secondStatement.find(delimiterWith)) {

          patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 22: where and and such that and with exists and such that is the first token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find(delimiterSuchThat) < secondStatement.find("and")
          && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterWith)) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 23: where and and such that and with exists and with is the first token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find(delimiterWith) < secondStatement.find("and")
          && secondStatement.find(delimiterWith) < secondStatement.find(delimiterSuchThat)) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 24: where and and pattern and with exists and and is the first token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find("and") < secondStatement.find(delimiterPattern)
          && secondStatement.find("and") < secondStatement.find(delimiterWith)) {

          patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 25: where and and pattern and with exists and pattern is the first token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find(delimiterPattern) < secondStatement.find("and")
          && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterWith)) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 26: where and and pattern and with exists and with is the first token
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find(delimiterWith) < secondStatement.find("and")
          && secondStatement.find(delimiterWith) < secondStatement.find(delimiterPattern)) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 27: where and and such that exists only and such that first
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find(delimiterSuchThat) < secondStatement.find("and")) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 28: where and and such that exists only and and first
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) != std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find(delimiterSuchThat) > secondStatement.find("and")) {

          patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 29: where and and pattern exists only and and first
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find(delimiterPattern) > secondStatement.find("and")) {

          patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 30: where and and pattern exists only and pattern first
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) != std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos
          && secondStatement.find(delimiterPattern) < secondStatement.find("and")) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 31: where and and with exists only and with first
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find(delimiterWith) < secondStatement.find("and")) {

          patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 32: where and and with exists only and and first
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) != std::string::npos
          && secondStatement.find(delimiterWith) > secondStatement.find("and")) {

          patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 32: where and exists only
        } else if (secondStatement.find("and") != std::string::npos
          && secondStatement.find(delimiterSuchThat) == std::string::npos
          && secondStatement.find(delimiterPattern) == std::string::npos
          && secondStatement.find(delimiterWith) == std::string::npos) {

          patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
          m_patternVector.push_back(patternStatement);
          secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
          prevClause = delimiterPattern;

          //Case 33: no more other clause behind
        } else {
          patternStatement = secondStatement;
          m_patternVector.push_back(patternStatement);

        }

      } else if (secondTempStatement == "with") {

        secondStatement = secondStatement.substr(secondStatement.find(delimiterSpace), secondStatement.size());
        secondStatement = secondStatement.substr(secondStatement.find_first_not_of(delimiterSpace), secondStatement.size());
        withStatementLeft = secondStatement.substr(0, secondStatement.find(OPERATOR_EQUAL));
        secondStatement = secondStatement.substr(secondStatement.find(OPERATOR_EQUAL) + 1, secondStatement.size());
        secondStatement.erase(0, secondStatement.find_first_not_of(delimiterSpace));

        withStatementRight = secondStatement.substr(0, secondStatement.find(delimiterSpace));

        withStatement.append(withStatementLeft);
        withStatement.append("=");
        withStatement.append(withStatementRight);

        m_withVector.push_back(withStatement);
        withStatement = "";
        //push with statement into a data structure;
        if (secondStatement.find(delimiterSpace) == std::string::npos) {
          break;
        }
        secondStatement = secondStatement.substr(secondStatement.find(delimiterSpace), secondStatement.size());
        secondStatement.erase(0, secondStatement.find_first_not_of(delimiterSpace));
        prevClause = delimiterWith;

      } else if (secondTempStatement == "and") {
        if (prevClause.compare(delimiterSuchThat) == 0) {
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSpace), secondStatement.size());
          //
          secondStatement = secondStatement.substr(secondStatement.find_first_not_of(delimiterSpace), secondStatement.size());
          suchThatStatement = secondStatement.substr(0, secondStatement.find(BRACKET_CLOSE) + 1);

          m_relationVector.push_back(suchThatStatement);
          secondStatement = secondStatement.substr(secondStatement.find(BRACKET_CLOSE) + 1, secondStatement.size());
          if (secondStatement == "") {
            break;
          }
          //
          secondStatement = secondStatement.substr(secondStatement.find_first_not_of(delimiterSpace), secondStatement.size());
          prevClause = delimiterSuchThat;;
        } else if (prevClause.compare(delimiterPattern) == 0) {
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSpace), secondStatement.size());
          //

          secondStatement = secondStatement.substr(secondStatement.find_first_not_of(delimiterSpace), secondStatement.size());

          //case 1: where with, such that, pattern still exists and such that is the next token
          if (secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterWith)
            && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterPattern)
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 2: where with, such that, pattern still exists and pattern is the next token
          } else if (secondStatement.find(delimiterPattern) < secondStatement.find(delimiterSuchThat)
            && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterWith)
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 3: where with, such that, pattern still exists and with is the next token
          } else if (secondStatement.find(delimiterWith) < secondStatement.find(delimiterSuchThat)
            && secondStatement.find(delimiterWith) < secondStatement.find(delimiterPattern)
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 4: where with, such that only exists and such that is the next token
          } else if (secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterWith)
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 5: where with, such that only exists and with is the next token
          } else if (secondStatement.find(delimiterWith) < secondStatement.find(delimiterSuchThat)
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 6: where with, pattern only exists and pattern is the next token
          } else if (secondStatement.find(delimiterPattern) < secondStatement.find(delimiterWith)
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 7: where with, pattern only exists and with is the next token
          } else if (secondStatement.find(delimiterWith) < secondStatement.find(delimiterPattern)
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 8: where such that, pattern only exists and pattern is the next token
          } else if (secondStatement.find(delimiterPattern) < secondStatement.find(delimiterSuchThat)
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 9: where such that, pattern only exists and such that is the next token
          } else if (secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterPattern)
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 10: where such that exists only
          } else if (secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 11: where pattern exists only
          } else if (secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 12: where with exists only
          } else if (secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 13: where and exists after pattern and there is such that, pattern, with after it
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") < secondStatement.find(delimiterSuchThat)
            && secondStatement.find("and") < secondStatement.find(delimiterPattern)
            && secondStatement.find("and") < secondStatement.find(delimiterWith)) {

            patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 14: where and exists and such that and pattern exists and and is the next token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") < secondStatement.find(delimiterSuchThat)
            && secondStatement.find("and") < secondStatement.find(delimiterPattern)
            && secondStatement.find("and") < secondStatement.find(delimiterWith)) {

            patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 15: where and exists and such that and pattern exists and with and such that next token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find(delimiterSuchThat) < secondStatement.find("and")
            && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterPattern)
            && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterWith)) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 16: where and exists and such that and pattern and with exists and pattern is the next token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterSuchThat)
            && secondStatement.find(delimiterPattern) < secondStatement.find("and")
            && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterWith)) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 17: where and exists and such that and pattern exists and with and with is the next token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find(delimiterWith) < secondStatement.find(delimiterSuchThat)
            && secondStatement.find(delimiterWith) < secondStatement.find(delimiterPattern)
            && secondStatement.find(delimiterWith) < secondStatement.find("and")) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 18: where and and such that and pattern exists and and is the first token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find("and") < secondStatement.find(delimiterSuchThat)
            && secondStatement.find("and") < secondStatement.find(delimiterPattern)) {

            patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 19: where and and such that and pattern exists and such that is the first token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find(delimiterSuchThat) < secondStatement.find("and")
            && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterPattern)) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 20: where and and such that and pattern exists and pattern is the first token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterSuchThat)
            && secondStatement.find(delimiterPattern) < secondStatement.find("and")) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 21: where and and such that and with exists and and is the first token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") < secondStatement.find(delimiterSuchThat)
            && secondStatement.find("and") < secondStatement.find(delimiterWith)) {

            patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 22: where and and such that and with exists and such that is the first token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find(delimiterSuchThat) < secondStatement.find("and")
            && secondStatement.find(delimiterSuchThat) < secondStatement.find(delimiterWith)) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 23: where and and such that and with exists and with is the first token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find(delimiterWith) < secondStatement.find("and")
            && secondStatement.find(delimiterWith) < secondStatement.find(delimiterSuchThat)) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 24: where and and pattern and with exists and and is the first token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find("and") < secondStatement.find(delimiterPattern)
            && secondStatement.find("and") < secondStatement.find(delimiterWith)) {

            patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 25: where and and pattern and with exists and pattern is the first token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find(delimiterPattern) < secondStatement.find("and")
            && secondStatement.find(delimiterPattern) < secondStatement.find(delimiterWith)) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 26: where and and pattern and with exists and with is the first token
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find(delimiterWith) < secondStatement.find("and")
            && secondStatement.find(delimiterWith) < secondStatement.find(delimiterPattern)) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 27: where and and such that exists only and such that first
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find(delimiterSuchThat) < secondStatement.find("and")) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterSuchThat) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterSuchThat), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 28: where and and such that exists only and and first
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) != std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find(delimiterSuchThat) > secondStatement.find("and")) {

            patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 29: where and and pattern exists only and and first
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find(delimiterPattern) > secondStatement.find("and")) {

            patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 30: where and and pattern exists only and pattern first
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) != std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos
            && secondStatement.find(delimiterPattern) < secondStatement.find("and")) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterPattern) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterPattern), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 31: where and and with exists only and with first
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find(delimiterWith) < secondStatement.find("and")) {

            patternStatement = secondStatement.substr(0, secondStatement.find(delimiterWith) - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find(delimiterWith), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 32: where and and with exists only and and first
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) != std::string::npos
            && secondStatement.find(delimiterWith) > secondStatement.find("and")) {

            patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 32: where and exists only
          } else if (secondStatement.find("and") != std::string::npos
            && secondStatement.find(delimiterSuchThat) == std::string::npos
            && secondStatement.find(delimiterPattern) == std::string::npos
            && secondStatement.find(delimiterWith) == std::string::npos) {

            patternStatement = secondStatement.substr(0, secondStatement.find("and") - 1);
            m_patternVector.push_back(patternStatement);
            secondStatement = secondStatement.substr(secondStatement.find("and"), secondStatement.size());
            prevClause = delimiterPattern;

            //Case 33: no more other clause behind
          } else {

            patternStatement = secondStatement.substr(0, secondStatement.size());
            m_patternVector.push_back(patternStatement);

          }
        } else if (prevClause.compare(delimiterWith) == 0) {

          secondStatement = secondStatement.substr(secondStatement.find(delimiterSpace), secondStatement.size());
          secondStatement = secondStatement.substr(secondStatement.find_first_not_of(delimiterSpace), secondStatement.size());
          withStatementLeft = secondStatement.substr(0, secondStatement.find(OPERATOR_EQUAL));
          secondStatement = secondStatement.substr(secondStatement.find(OPERATOR_EQUAL) + 1, secondStatement.size());
          secondStatement.erase(0, secondStatement.find_first_not_of(delimiterSpace));

          withStatementRight = secondStatement.substr(0, secondStatement.find(delimiterSpace));

          withStatement.append(withStatementLeft);
          withStatement.append("=");
          withStatement.append(withStatementRight);

          m_withVector.push_back(withStatement);
          withStatement = "";
          //push with statement into a data structure;
          if (secondStatement.find(delimiterSpace) == std::string::npos) {
            break;
          }
          secondStatement = secondStatement.substr(secondStatement.find(delimiterSpace), secondStatement.size());
          secondStatement.erase(0, secondStatement.find_first_not_of(delimiterSpace));
          prevClause = delimiterWith;

        }
      } else {
        break;
      }

    }
  }
  std::cout << "Relation Vector size: " << m_relationVector.size() << std::endl;
  std::cout << "pattern Vector size: " << m_patternVector.size() << std::endl;
  std::cout << "with Vector size: " << m_withVector.size() << std::endl;

  //parsing selectStatement: this code will only work for iteration 1. use find_first_of for future iterations
  std::string synonymOriginal = selectStatement.substr(selectStatement.find(" "), selectStatement.size());
  synonymOriginal = m_stringUtil.trimString(synonymOriginal);

  std::string synonym;
  std::string synonymAttribute;

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
      if (m_selectQueue.size() == 1) {
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
        }

        m_selectQueue.push(g1);

      } else if (synonym == BOOLEAN_QPP) {
        return false;
      }
    }

  //Case 2: synonym attribute does not exist
  } else {
    
    int counterM = 0;
    for (auto m = m_grammarVector.begin(); m != m_grammarVector.end(); m++, counterM++) {
      if (m_selectQueue.size() == 1) {
        break;
      }
      g1 = m_grammarVector.at(counterM);
      std::string grammarName = g1.getName();

      //std::cout << grammarName << " this is the grammarName" << std::endl;
      if (grammarName == synonym) {
        m_selectQueue.push(g1);
        //std::cout << "This is select queue size currently: " << m_selectQueue.size() << std::endl;
        //std::cout << "pushed " << grammarName << " into select queue" << std::endl;
      } else if (synonym == BOOLEAN_QPP) {
        g1 = Grammar(queryType::GType::BOOLEAN, "BOOLEAN");
        m_selectQueue.push(g1);
      }
    }

    if (synonym == BOOLEAN_QPP && m_grammarVector.size() == 0) {
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
  if (selectGrammar.getType() != queryType::GType::BOOLEAN) {
    m_synonymMap.insert({ selectGrammar.getName(), 1 });
  }
  //std::cout << "Select queue front Grammar name: " << selectGrammar.getName() << std::endl;

  //if design abstraction object does not exist
  if (m_relationVector.empty()) {
    std::cout << "relation vector is empty" << std::endl;
  }
  //if design abstraction object exists, create Relation object with parameters type, g1 g2
  else {
    int counterB = 0;
    for (auto b = m_relationVector.begin(); b != m_relationVector.end(); b++, counterB++) {
      //Trim whitespaces for such that statement.
      /*suchThatStatement = m_stringUtil.trimString(suchThatStatement);
      std::string delimiterDesignAbstraction = "such that";*/
      //std::string designAbstractions = suchThatStatement.substr(suchThatStatement.find(delimiterDesignAbstraction) + 10, suchThatStatement.size()); //"such that " is 10 characters
      std::string designAbstractions = m_relationVector.at(counterB);                                                                                                                                              //IMPT: to be worked on in the future for more complex queries
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
      if ((sTInt1 == sTInt2 && sTInt1 != 0) || (sTName1 == sTName2 && sTName1 != OPERATOR_UNDERSCORE)) {
        //return empty list
        std::cout << "return an empty list of strings" << std::endl;
        return false;
      }

      else {
        //Checks for no declaration first before moving on to check for synonyms
        //Follows and Follows*
        if (designAbstractionEntity == "Follows" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Follows*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          //Number, Number
          if (sTInt1 > 0 && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //Number, _
          } else if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //_, Number
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            // _, _
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

          }

          //Parent and Parent*
        } else if (designAbstractionEntity == "Parent" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Parent*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          //Number, Number
          if (sTInt1 > 0 && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //Number, _
          } else if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //_, Number
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            // _, _
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

          }

          //Uses, Modifies
        } else if (designAbstractionEntity == "Uses" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Modifies" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          //Number, String
          if (sTInt1 > 0 && sTName2.find('"') != std::string::npos) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //Number, _
          } else if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //String, String
          } else if (sTName1.find('"') != std::string::npos && sTName2.find('"') != std::string::npos) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //String, _
          } else if (sTName1.find('"') != std::string::npos && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

          }

          //Call, Calls*
        } else if (designAbstractionEntity == "Calls" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Calls*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          //String, _
          if (sTName1.find('"') != std::string::npos && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //_, String
          } else if (sTName1 == OPERATOR_UNDERSCORE && sTName2.find('"') != std::string::npos) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

          }

        } else if (designAbstractionEntity == "Next" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Next*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {
          // Number, _
          if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //_, Number
          } if (sTName1 == OPERATOR_UNDERSCORE && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }

          //Affects, Affects*
        } else if (designAbstractionEntity == "Affects" && m_grammarVector.empty() && synonym == BOOLEAN_QPP
          || designAbstractionEntity == "Affects*" && m_grammarVector.empty() && synonym == BOOLEAN_QPP) {

          // Number, _
          if (sTInt1 > 0 && sTName2 == OPERATOR_UNDERSCORE) {
            g1 = Grammar(queryType::GType::STMT_NO, sTName1);
            g2 = Grammar(queryType::GType::STR, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);

            //_, Number
          } if (sTName1 == OPERATOR_UNDERSCORE && sTInt2 > 0) {
            g1 = Grammar(queryType::GType::STR, sTName1);
            g2 = Grammar(queryType::GType::STMT_NO, sTName2);
            Relation DAO(designAbstractionEntity, g1, g2);
            m_suchThatQueue.push(DAO);
          }
        }
        counterK = 0;
        bool moveOn = false;
        for (auto k = m_grammarVector.begin(); k != m_grammarVector.end(); k++, counterK++) {
          Grammar tempGrammar = m_grammarVector.at(counterK);
          std::string grammarName = tempGrammar.getName();

          counterQ = 0;
          //if (m_suchThatQueue.size() == 1 && counterK > 2) {
          //  break;
          //}
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

                //std::cout << "created new grammar2 object: " << g2.getName() << std::endl;
                Relation DAO(designAbstractionEntity, g1, g2);
                m_suchThatQueue.push(DAO);
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
                m_suchThatQueue.push(DAO);
                counterK = 0;
                moveOn = true;
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
                counterK = 0;
                moveOn = true;
                break;
              } else if (sTName2 == OPERATOR_UNDERSCORE) {
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_suchThatQueue.push(DAO);
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
                } else {
                  m_synonymMap[sTName2]++;
                }
                counterK = 0;
                moveOn = true;
                break;
              } else if (sTInt2 > 0) {
                g2 = Grammar(queryType::GType::STMT_NO, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_suchThatQueue.push(DAO);
                counterK = 0;
                moveOn = true;
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
                counterK = 0;
                moveOn = true;
                break;
              } else if (sTName2 == OPERATOR_UNDERSCORE) {
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_suchThatQueue.push(DAO);
                counterK = 0;
                moveOn = true;
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
                counterK = 0;
                moveOn = true;
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
              } else if (sTName2 == OPERATOR_UNDERSCORE) {
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_suchThatQueue.push(DAO);
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
                counterK = 0;
                moveOn = true;
                break;
              } else if (sTName2 == OPERATOR_UNDERSCORE) {
                g2 = Grammar(queryType::GType::STR, sTName2);
                Relation DAO(designAbstractionEntity, g1, g2);
                m_suchThatQueue.push(DAO);
                counterK = 0;
                moveOn = true;
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
  }
  std::cout << "suchThatQueue size: " << m_suchThatQueue.size() << std::endl;
  
  //creation of pattern object
  if (m_patternVector.empty()) {
    std::cout << "pattern vector is empty" << std::endl;
  } else {
    int counterC = 0;
    for (auto c = m_patternVector.begin(); c != m_patternVector.end(); c++, counterC++) {
      //std::string patternEntity = patternStatement.substr(0, patternStatement.find(delimiterSpace));
      //std::string patternObject = patternStatement.substr(patternStatement.find(delimiterSpace), patternStatement.size());
      std::string patternObject = m_patternVector.at(counterC);

      patternObject = m_stringUtil.trimString(patternObject);

      std::cout << patternObject << std::endl;

      std::string delimiterBracket1 = "(";
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
        //std::vector<size_t> positions; // holds all the positions that sub occurs within str

        std::string patternRightNameTemp = patternRightName;
        char sub = '(';

        size_t pos3 = patternRightNameTemp.find(sub, 0);
        while (pos3 != std::string::npos) {
          //positions.push_back(pos);
          pos3 = patternRightNameTemp.find(sub, pos3 + 1);
          if (patternRightNameTemp.find_first_not_of(" \t") == ')') {
            return false;
          }
        }

        if ((patternRightName.find('"') != std::string::npos) && patternRightName.front() == '_' && patternRightName.back() == '_') {
          removeCharsFromString(patternRightName, "\\\" _");
          //grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
          patternExpressionVector = patternVectorTokenizer("()+-*", patternRightName, patternExpressionVector);
          grammarPatternRight = Grammar(patternExpressionVector, patternRightName, queryType::GType::STR);
          isSubTree = true;
        } else if (patternRightName.find('"') != std::string::npos && patternRightName.front() != '_' && patternRightName.back() != '_') {
          removeCharsFromString(patternRightName, "\\\" ");
          //grammarPatternRight = Grammar(queryType::GType::STR, patternRightName);
          patternExpressionVector = patternVectorTokenizer("()+-*", patternRightName, patternExpressionVector);
          grammarPatternRight = Grammar(patternExpressionVector, patternRightName, queryType::GType::STR);
        } else if (patternRightName == OPERATOR_UNDERSCORE) {
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
      }

      //create pattern object
      Pattern patternObjectCreated(patternOfGrammar, grammarPatternLeft, grammarPatternRight, isSubTree);
      m_patternQueue.push(patternObjectCreated);

    }
  }

  std::cout << "patternQueue size: " << m_patternQueue.size() << std::endl;

  //creation of with object
  if (m_withVector.empty()) {
    std::cout << "with vector is empty" << std::endl;
  } else {
    int counterD=0;
    for (auto d = m_withVector.begin(); d != m_withVector.end(); d++, counterD++) {
      //withStatement = m_stringUtil.trimString(withStatement);
      //std::string withObject = withStatement.substr(withStatement.find(delimiterSpace), withStatement.size());
      std::string withObject = m_withVector.at(counterD);
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
        withClauseAttAtt(withLeft, withRight, withLeftGrammar, withRightGrammar);

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
          m_withQueue.push(withObjectCreated);
        }
        //Case 12: Both sides integers
      } else if (withLeftInt > 0 && withRightInt > 0) {
        if (withLeftInt != withRightInt) {
          return false;
        } else if (withLeftInt == withRightInt) {
          withLeftGrammar = Grammar(queryType::GType::STMT_NO, withLeft);
          withRightGrammar = Grammar(queryType::GType::STMT_NO, withRight);

          With withObjectCreated(withLeftGrammar, withRightGrammar);
          m_withQueue.push(withObjectCreated);
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
  std::cout << "withQueue size: " << m_withQueue.size() << std::endl;


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

std::vector<std::string> QueryPreProcessor::patternVectorTokenizer(char* charsToRemove, std::string patternRightName, std::vector<std::string> vector) {
  int pos1 = 0;
  int prev_pos_new = 0;
  std::string c;

  while ((pos1 = patternRightName.find_first_of(charsToRemove, prev_pos_new)) != std::string::npos) {
    if (prev_pos_new == 0) {
      c = patternRightName.at(prev_pos_new);
      if (c == "+") {
        vector.push_back(c);
      } else if (c == "-") {
        vector.push_back(c);
      } else if (c == "*") {
        vector.push_back(c);
      } else if (c == "(") {
        vector.push_back(c);
      } else if (c == ")") {
        vector.push_back(c);
      }

    }
    if (pos1 > prev_pos_new) {
      vector.push_back(patternRightName.substr(prev_pos_new, pos1 - prev_pos_new));
      c = patternRightName.at(pos1);
      if (c == "+") {
        vector.push_back(c);
      } else if (c == "-") {
        vector.push_back(c);
      } else if (c == "*") {
        vector.push_back(c);
      } else if (c == "(") {
        vector.push_back(c);
      } else if (c == ")") {
        vector.push_back(c);
      }

    //character next to previous token is a bracket, operator
    } else if (pos1 = prev_pos_new) {
      c = patternRightName.at(pos1);
      if (c == "+") {
        vector.push_back(c);
      } else if (c == "-") {
        vector.push_back(c);
      } else if (c == "*") {
        vector.push_back(c);
      } else if (c == "(") {
        vector.push_back(c);
      } else if (c == ")") {
        vector.push_back(c);
      }
    } 
    prev_pos_new = pos1 + 1;
  }
  if (prev_pos_new < patternRightName.length()) {
    vector.push_back(patternRightName.substr(prev_pos_new, std::string::npos));
  }
  return vector;
}

bool QueryPreProcessor::withClauseAttNum(std::string attribute, std::string integer, Grammar withLeftGrammar, Grammar withRightGrammar) {
  std::string tempAttribute = attribute;
  std::string withTempSynonym = tempAttribute.substr(0, tempAttribute.find("."));
  std::string withTempAttribute = tempAttribute.substr(tempAttribute.find(".") + 1, tempAttribute.size());

  withTempSynonym = m_stringUtil.trimString(withTempSynonym);
  withTempAttribute = m_stringUtil.trimString(withTempAttribute);

  withLeftGrammar = withAttributeProcessor(attribute, withLeftGrammar);
  
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

    withRightGrammar = Grammar(queryType::GType::STMT_NO, integer);
  }
  if (withLeftGrammar.getAttr() == queryType::AType::VALUE && withLeftGrammar.getType() == queryType::GType::CONST
    || withLeftGrammar.getType() == queryType::GType::STMT && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::ASGN && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::WHILE && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::IF && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::CALL && withLeftGrammar.getAttr() == queryType::AType::STMT_NUM
    || withLeftGrammar.getType() == queryType::GType::PROG_LINE && withTempAttribute == "") {
    With withObjectCreated(withLeftGrammar, withRightGrammar); 
    m_withQueue.push(withObjectCreated);
    return true;
  } else {
    return false;
  }
}

bool QueryPreProcessor::withClauseAttNumNoSynonymAtt(std::string withSynonym, std::string integer, Grammar withLeftGrammar, Grammar withRightGrammar) {

  int counterS = 0;
  for (auto s = m_grammarVector.begin(); s != m_grammarVector.end(); s++, counterS++) {
    if (m_grammarVector.at(counterS).getName() == withSynonym) {
      withLeftGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
    }
  }

  withRightGrammar = Grammar(queryType::GType::STMT_NO, integer);
  
  if(withLeftGrammar.getType() == queryType::GType::PROG_LINE) {
    With withObjectCreated(withLeftGrammar, withRightGrammar);
    m_withQueue.push(withObjectCreated);
    return true;
  } else {
    return false;
  }
}

bool QueryPreProcessor::withClauseAttString(std::string attribute, std::string inputString, Grammar withLeftGrammar, Grammar withRightGrammar) {
  std::string tempAttribute = attribute;
  std::string withTempSynonym = tempAttribute.substr(0, tempAttribute.find("."));
  std::string withTempAttribute = tempAttribute.substr(tempAttribute.find(".") + 1, tempAttribute.size());

  withTempSynonym = m_stringUtil.trimString(withTempSynonym);
  withTempAttribute = m_stringUtil.trimString(withTempAttribute);

  withLeftGrammar = withAttributeProcessor(attribute, withLeftGrammar);

  removeCharsFromString(inputString, "\"");
  withRightGrammar = Grammar(queryType::GType::STR, inputString);

  if (withLeftGrammar.getAttr() == queryType::AType::PROC_NAME && withLeftGrammar.getType() == queryType::GType::PROC
    || withLeftGrammar.getAttr() == queryType::AType::PROC_NAME && withLeftGrammar.getType() == queryType::GType::CALL
    || withLeftGrammar.getAttr() == queryType::AType::VAR_NAME && withLeftGrammar.getType() == queryType::GType::VAR) {
    With withObjectCreated(withLeftGrammar, withRightGrammar);
    m_withQueue.push(withObjectCreated);
    return true;
  } else {
    return false;
  }
}

void QueryPreProcessor::withClauseAttAtt(std::string leftAttribute, std::string rightAttribute, Grammar withLeftGrammar, Grammar withRightGrammar) {
  withLeftGrammar = withAttributeProcessor(leftAttribute, withLeftGrammar);
  withRightGrammar = withAttributeProcessor(rightAttribute, withRightGrammar);
  With withObjectCreated(withLeftGrammar, withRightGrammar);
  m_withQueue.push(withObjectCreated);
}

bool QueryPreProcessor::withClauseSynAtt(std::string leftSynonym, std::string rightSynonym, Grammar withLeftGrammar, Grammar withRightGrammar) {
  int counterS = 0;
  for (auto s = m_grammarVector.begin(); s != m_grammarVector.end(); s++, counterS++) {
    if (m_grammarVector.at(counterS).getName() == leftSynonym) {
      withLeftGrammar = Grammar(m_grammarVector.at(counterS).getType(), leftSynonym);
      std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(leftSynonym);
      if (got == m_synonymMap.end()) {
        m_synonymMap.insert({ leftSynonym, 1 });
      } else {
        m_synonymMap[leftSynonym]++;
      }
    }
  }

  withRightGrammar = withAttributeProcessor(rightSynonym, withRightGrammar);

  if (withLeftGrammar.getType() == queryType::GType::PROG_LINE) {
    With withObjectCreated(withLeftGrammar, withRightGrammar);
    m_withQueue.push(withObjectCreated);
    return true;
  } else {
    return false;
  }

  With withObjectCreated(withLeftGrammar, withRightGrammar);
  m_withQueue.push(withObjectCreated);
}

bool QueryPreProcessor::withClauseSynSyn(std::string leftSynonym, std::string rightSynonym, Grammar withLeftGrammar, Grammar withRightGrammar) {
  int counterS = 0;
  for (auto s = m_grammarVector.begin(); s != m_grammarVector.end(); s++, counterS++) {
    if (m_grammarVector.at(counterS).getName() == leftSynonym) {
      withLeftGrammar = Grammar(m_grammarVector.at(counterS).getType(), leftSynonym);
      std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(leftSynonym);
      if (got == m_synonymMap.end()) {
        m_synonymMap.insert({ leftSynonym, 1 });
      } else {
        m_synonymMap[leftSynonym]++;
      }
    }
  }

  int counterS2 = 0;
  for (auto s2 = m_grammarVector.begin(); s2 != m_grammarVector.end(); s2++, counterS2++) {
    if (m_grammarVector.at(counterS2).getName() == rightSynonym) {
      withRightGrammar = Grammar(m_grammarVector.at(counterS2).getType(), rightSynonym);
      std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(rightSynonym);
      if (got == m_synonymMap.end()) {
        m_synonymMap.insert({ rightSynonym, 1 });
      } else {
        m_synonymMap[rightSynonym]++;
      }
    }
  }

  if (withLeftGrammar.getType() == queryType::GType::PROG_LINE && withRightGrammar.getType() == queryType::GType::PROG_LINE) {
    With withObjectCreated(withLeftGrammar, withRightGrammar);
    m_withQueue.push(withObjectCreated);
    return true;
  } else {
    return false;
  }

  With withObjectCreated(withLeftGrammar, withRightGrammar);
  m_withQueue.push(withObjectCreated);
}

Grammar QueryPreProcessor::withAttributeProcessor(std::string attribute, Grammar withGrammar) {
  std::string withSynonym = attribute.substr(0, attribute.find("."));
  std::string withAttribute = attribute.substr(attribute.find(".") + 1, attribute.size());

  withSynonym = m_stringUtil.trimString(withSynonym);
  withAttribute = m_stringUtil.trimString(withAttribute);

  int counterS = 0;
  for (auto s = m_grammarVector.begin(); s != m_grammarVector.end(); s++, counterS++) {
    if (m_grammarVector.at(counterS).getName() == withSynonym) {
      if (withAttribute == PROCNAME) {
        withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        withGrammar.setAType(queryType::AType::PROC_NAME);
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(withSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ withSynonym, 1 });
        } else {
          m_synonymMap[withSynonym]++;
        }
      } else if (withAttribute == VARNAME) {
        withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        withGrammar.setAType(queryType::AType::VAR_NAME);
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(withSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ withSynonym, 1 });
        } else {
          m_synonymMap[withSynonym]++;
        }
      } else if (withAttribute == VALUE) {
        withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        withGrammar.setAType(queryType::AType::VALUE);
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(withSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ withSynonym, 1 });
        } else {
          m_synonymMap[withSynonym]++;
        }
      } else if (withAttribute == STMT_NO) {
        withGrammar = Grammar(m_grammarVector.at(counterS).getType(), withSynonym);
        withGrammar.setAType(queryType::AType::STMT_NUM);
        std::unordered_map<std::string, int>::const_iterator got = m_synonymMap.find(withSynonym);
        if (got == m_synonymMap.end()) {
          m_synonymMap.insert({ withSynonym, 1 });
        } else {
          m_synonymMap[withSynonym]++;
        }
      }
    }
  }
  return withGrammar;
}