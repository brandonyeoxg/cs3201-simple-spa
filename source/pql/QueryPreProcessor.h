/**
* Query preprocessing class. This class creates Grammar Objects, Relation Objects and Pattern Objects
* to be passed to the QueryEvaluator from the user input.
*/

#pragma once
#ifndef QUERYPREPROCESSOR_H
#define QUERYPREPROCESSOR_H
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include "../pql/query-types/Grammar.h"
#include "../pql/query-types/Relation.h"
#include "../pql/query-types/Pattern.h"
#include "../pql/query-types/With.h"
#include "../pkb/PKB.h"
#include "../util/StringUtil.h"
#include "../GlobalTypeDef.h"

class QueryPreProcessor
{
public:

  /**
  * Default QueryPreProcessor constructor.
  */
  QueryPreProcessor();

  /**
  * Default QueryPreProcessor deconstructor.
  */
  ~QueryPreProcessor();

////////////////////////////////////////////////////////////////////////////////
///////                             APIs                                 ///////
////////////////////////////////////////////////////////////////////////////////
  /**
  * Method takes in the declaration parsed string and generates the Grammar objects
  * from the declaration which is stored into a Grammar vector for further use.
  * @param t_declarationInput is the input declaration string.
  * @return true if there is no errors. else returns false.
  */
  BOOLEAN tokenizeDeclaration(STRING t_declarationInput);

  /**
  * Method takes in the query parsed string and generates the Grammar objects for 
  * Select Queue, Relation Queue and the Pattern Objects for the Pattern Queue.
  * Also generates a synonym unordered map for QueryEvaluator to use.
  * @param t_queryInput is the input query string.
  * @return true if there is no errors. else returns false.
  */
  BOOLEAN tokenizeQuery(STRING t_queryInput);

  /**
  * Method takes in a string containing both the declaration and query and returns declaration.
  * @param t_Input is the input string.
  * @return declaration portion of the query input.
  */
  STRING splitStringDeclaration(STRING t_input);

  /**
  * Method takes in a string containing both the declaration and query and returns query.
  * @param t_Input is the input string.
  * @return declaration portion of the query input.
  */
  STRING splitStringQuery(STRING t_input);


  /**
  * A boolean method for checking whether "Select BOOLEAN" exists or not.
  * @return true if exists, false if not.
  */
  BOOLEAN QueryPreProcessor::isSelectBoolean(void);

////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------Getter Methods------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////

  /**
  * A getter method for the Select vector variables.
  * @return a Grammar object Vector containing all variables within the Select clause.
  */
  std::vector<Grammar> QueryPreProcessor::getSelectVector(void);

  /**
  * A getter method for the Such That vector variables.
  * @return a Relation object Vector containing all variables within the Such That clause.
  */
  std::vector<Relation> QueryPreProcessor::getSuchThatVector(void);

  /**
  * A getter method for the Pattern vector variables.
  * @return a Pattern object Vector containing all variables within the Pattern clause.
  */
  std::vector<Pattern> QueryPreProcessor::getPatternVector(void);

  /**
  * A getter method for the With vector variables.
  * @return a With object Vector containing all variables within the With clause.
  */
  std::vector<With> QueryPreProcessor::getWithVector(void);

  /**
  * A getter method for the Synonym Unordered Map variables.
  * @return an unordered map containing 2 values(string synonym, int number of times it appears) declared within query statement.
  */
  std::unordered_map<std::string, int> QueryPreProcessor::getSynonym(void);

///////////////////////////////////////////////////////////////////////////////////////
////                              Internal methods                                /////
/////////////////////////////////////////////////////////////////////////////////////// 

  /**
  * A tokenizing method which removes the characters specified in the string character
  * in the targeted string t_targetString and then pushes them into the vector
  * @return a string vector t_vector containing the string tokens
  */
  std::vector<std::string> QueryPreProcessor::stringVectorTokenizer(char* t_charsToRemove, std::string t_targetString, std::vector<std::string> t_vector);

  /**
  * A tokenizing method tailored for patterns which removes the characters specified in the string character
  * in the targeted string t_patternRightName and then pushes them into the vector
  * @return a string vector t_vector containing the string tokens
  */
  std::vector<std::string> QueryPreProcessor::patternVectorTokenizer(char* t_charsToRemove, std::string t_patternRightName, std::vector<std::string> t_vector);

  /**
  * A method that takes in an attribute string and number and calls other methods to process
  * @param t_attribute being the side that has an attribute
  * @param t_integer being a constant for constant value or stmt#
  * @param t_withLeftGrammar being the Grammar to be formed on the left side
  * @param t_withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  BOOLEAN QueryPreProcessor::withClauseAttNum(std::string t_attribute, std::string t_integer, Grammar t_withLeftGrammar, Grammar t_withRightGrammar);

  /**
  * A method that takes in an attribute string and string and calls other methods to process
  * @param t_attribute being the side that has an attribute
  * @param t_string being a string for procName/varName
  * @param t_withLeftGrammar being the Grammar to be formed on the left side
  * @param t_withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  BOOLEAN QueryPreProcessor::withClauseAttString(std::string t_attribute, std::string t_string, Grammar t_withLeftGrammar, Grammar t_withRightGrammar);

  /**
  * A specialised method for synonyms with no attributes that takes in an attribute string and number and calls other methods to process
  * @param t_attribute being the side that has an attribute
  * @param t_integer being a constant for constant value or stmt#
  * @param t_withLeftGrammar being the Grammar to be formed on the left side
  * @param t_withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  BOOLEAN QueryPreProcessor::withClauseAttNumNoSynonymAtt(std::string t_attribute, std::string t_integer, Grammar t_withLeftGrammar, Grammar t_withRightGrammar);

  /**
  * A method that takes in 2 attribute strings and calls other methods to process
  * @param t_leftAttribute being the left side that has an attribute
  * @param t_rightAttribute being the left side that has an attribute
  * @param t_withLeftGrammar being the Grammar to be formed on the left side
  * @param t_withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  BOOLEAN QueryPreProcessor::withClauseAttAtt(std::string t_leftAttribute, std::string t_rightAttribute, Grammar t_withLeftGrammar, Grammar t_withRightGrammar);

  /**
  * A specialised method that takes in 2 synonyms that only has an attribute on one side and calls other methods to process
  * @param t_leftSynonym being the side that does not have an attribute
  * @param t_rightSynonym being the side that has an attribute
  * @param t_withLeftGrammar being the Grammar to be formed on the left side
  * @param t_withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  BOOLEAN QueryPreProcessor::withClauseSynAtt(std::string t_leftSynonym, std::string t_rightSynonym, Grammar t_withLeftGrammar, Grammar t_withRightGrammar);

  /**
  * A method that takes in 2 synonyms that does not have any attributes and processes it
  * @param t_leftSynonym being the synonym that does not have an attribute
  * @param t_rightSynonym being the synonym that does not have an attribute
  * @param t_withLeftGrammar being the Grammar to be formed on the left side
  * @param t_withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  BOOLEAN QueryPreProcessor::withClauseSynSyn(std::string t_leftSynonym, std::string t_rightSynonym, Grammar t_withLeftGrammar, Grammar t_withRightGrammar);

  /**
  * A method that takes in a string that contains an attribute and processes it
  * @param t_attribute being the string that contains the attribute
  * @param t_withGrammar being the Grammar to be formed on the left side
  * @return Grammar object with its attributes
  */
  Grammar QueryPreProcessor::withAttributeProcessor(std::string t_attribute, Grammar t_withGrammar);

  /**
  * A method that takes in the query that contains multiple clauses and processes it
  * @param t_secondStatement is the current statement that has been parsed through
  * @param t_whichClause being the type of clause that it is being parsed in
  * @return t_secondStatement that has been parsed and substringed.
  */
  STRING QueryPreProcessor::multiClauseProcessor(std::string t_secondStatement, std::string t_whichClause);

  /**
  * A getter method for the Grammar vector variables.
  * @return a Grammar object Vector containing all variables declared within the declaration.
  */
  std::vector<Grammar> QueryPreProcessor::getGrammarVector(void);

  /**
  * Method takes in a string and removes all characters specified in charsToRemove.
  * @param t_str is the input string.
  * @param t_charsToRemove are the characters to be removed from the string.
  */
  void removeCharsFromString(std::string & t_str, char * t_charsToRemove);
private:
  std::vector<std::string> m_selectVector;
  std::vector<std::string> m_relationVector;
  std::vector<std::string> m_patternVector;
  std::vector<std::string> m_withVector;

  std::string m_prevClause;
  std::string m_secondStatementTemp;
  std::vector<Grammar> m_selectVectorQE;
  std::vector<Relation> m_relationVectorQE;
  std::vector<Pattern> m_patternVectorQE;
  std::vector<With> m_withVectorQE;
  std::vector<Grammar> m_grammarVector;
  std::unordered_map<std::string, int> m_validationMap;

  std::unordered_map<std::string, int> m_synonymMap;

  bool m_selectBOOLEANExists = false;

  StringUtil m_stringUtil;
  static std::string PROCEDURE;
  static std::string STMTLST;
  static std::string STMT;
  static std::string ASSIGN;
  static std::string WHILE;
  static std::string IF;
  static std::string VARIABLE;
  static std::string CONSTANT;
  static std::string PROG_LINE;
  static std::string BOOLEAN_QPP;
  static std::string CALL;
  static std::string PROCNAME;
  static std::string VARNAME;
  static std::string VALUE;
  static std::string STMT_NO;
};

#endif QUERYPREPROCESSOR_H