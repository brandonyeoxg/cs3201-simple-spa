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

  /**
  * Method takes in the declaration parsed string and generates the Grammar objects
  * from the declaration which is stored into a Grammar vector for further use.
  * @param t_declarationInput is the input declaration string.
  * @return true if there is no errors. else returns false.
  */
  BOOLEAN tokenizeDeclaration(std::string t_declarationInput);

  /**
  * Method takes in the query parsed string and generates the Grammar objects for 
  * Select Queue, Relation Queue and the Pattern Objects for the Pattern Queue.
  * Also generates a synonym unordered map for QueryEvaluator to use.
  * @param t_queryInput is the input query string.
  * @return true if there is no errors. else returns false.
  */
  BOOLEAN tokenizeQuery(std::string t_queryInput);

  /**
  * Method takes in a string and removes all characters specified in charsToRemove.
  * @param str is the input string.
  * @param charsToRemove are the characters to be removed from the string.
  */
  void removeCharsFromString(std::string & str, char * charsToRemove);

  /**
  * Method takes in a string containing both the declaration and query and returns declaration.
  * @param str is the input string.
  * @return declaration portion of the query input.
  */
  std::string splitStringDeclaration(std::string t_Input);

  /**
  * Method takes in a string containing both the declaration and query and returns query.
  * @param str is the input string.
  * @return declaration portion of the query input.
  */
  std::string splitStringQuery(std::string t_Input);

  /**
  * Method takes in a vector containing strings and prints out the strings.
  * @param t_vector is the input vector containing strings.
  */
  void QueryPreProcessor::printVector(std::vector<std::string> t_vector);

  /**
  * A tokenizing method which removes the characters specified in the string character
  * in the targeted string targetString and then pushes them into the vector
  * @return a string vector containing the string tokens
  */
  std::vector<std::string> QueryPreProcessor::stringVectorTokenizer(char* charsToRemove, std::string targetString, std::vector<std::string> vector);

  /**
  * A tokenizing method tailored for patterns which removes the characters specified in the string character
  * in the targeted string patternRightName and then pushes them into the vector
  * @return a string vector containing the string tokens
  */
  std::vector<std::string> QueryPreProcessor::patternVectorTokenizer(char* charsToRemove, std::string patternRightName, std::vector<std::string> vector);

  /**
  * A method that takes in an attribute string and number and calls other methods to process
  * @param attribute being the side that has an attribute
  * @param integer being a constant for constant value or stmt#
  * @param withLeftGrammar being the Grammar to be formed on the left side
  * @param withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  bool QueryPreProcessor::withClauseAttNum(std::string attribute, std::string integer, Grammar withLeftGrammar, Grammar withRightGrammar);

  /**
  * A method that takes in an attribute string and string and calls other methods to process
  * @param attribute being the side that has an attribute
  * @param string being a string for procName/varName
  * @param withLeftGrammar being the Grammar to be formed on the left side
  * @param withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  bool QueryPreProcessor::withClauseAttString(std::string attribute, std::string string, Grammar withLeftGrammar, Grammar withRightGrammar);
  
  /**
  * A specialised method for synonyms with no attributes that takes in an attribute string and number and calls other methods to process
  * @param attribute being the side that has an attribute
  * @param integer being a constant for constant value or stmt#
  * @param withLeftGrammar being the Grammar to be formed on the left side
  * @param withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  bool QueryPreProcessor::withClauseAttNumNoSynonymAtt(std::string attribute, std::string integer, Grammar withLeftGrammar, Grammar withRightGrammar);

  /**
  * A method that takes in 2 attribute strings and calls other methods to process
  * @param leftAttribute being the left side that has an attribute
  * @param rightAttribute being the left side that has an attribute
  * @param withLeftGrammar being the Grammar to be formed on the left side
  * @param withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  bool QueryPreProcessor::withClauseAttAtt(std::string leftAttribute, std::string rightAttribute, Grammar withLeftGrammar, Grammar withRightGrammar);

  /**
  * A specialised method that takes in 2 synonyms that only has an attribute on one side and calls other methods to process
  * @param leftSynonym being the side that does not have an attribute
  * @param rightSynonym being the side that has an attribute
  * @param withLeftGrammar being the Grammar to be formed on the left side
  * @param withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  bool QueryPreProcessor::withClauseSynAtt(std::string leftSynonym, std::string rightSynonym, Grammar withLeftGrammar, Grammar withRightGrammar);

  /**
  * A method that takes in 2 synonyms that does not have any attributes and processes it
  * @param leftSynonym being the synonym that does not have an attribute
  * @param rightSynonym being the synonym that does not have an attribute
  * @param withLeftGrammar being the Grammar to be formed on the left side
  * @param withRightGrammar being the Grammar to be formed on the right side
  * @return true if with clause is parsed and processed properly. false if it fails.
  */
  bool QueryPreProcessor::withClauseSynSyn(std::string leftSynonym, std::string rightSynonym, Grammar withLeftGrammar, Grammar withRightGrammar);

  /**
  * A method that takes in a string that contains an attribute and processes it
  * @param attribute being the string that contains the attribute
  * @param withGrammar being the Grammar to be formed on the left side
  * @return Grammar object with its attributes
  */
  Grammar QueryPreProcessor::withAttributeProcessor(std::string attribute, Grammar withGrammar);

  /**
  * A method that takes in the query that contains multiple clauses and processes it 
  * @param attribute being the string that contains the attribute
  * @param withGrammar being the Grammar to be formed on the left side
  * @return string that has been parsed and substringed.
  */
  std::string QueryPreProcessor::multiClauseProcessor(std::string secondStatement, std::string whichClause);

  /**
  * A boolean method for checking whether "Select BOOLEAN" exists or not.
  * @return true if exists, false if not.
  */
  bool QueryPreProcessor::isSelectBoolean(void);

////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------Getter Methods------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////

  /**
  * A getter method for the Select vector variables.
  * @return a Grammar object Queue containing all variables within the Select clause.
  */
  std::vector<Grammar> QueryPreProcessor::getSelectVector(void);

  /**
  * A getter method for the Such That vector variables.
  * @return a Relation object Queue containing all variables within the Such That clause.
  */
  std::vector<Relation> QueryPreProcessor::getSuchThatVector(void);

  /**
  * A getter method for the Pattern vector variables.
  * @return a Pattern object Queue containing all variables within the Pattern clause.
  */
  std::vector<Pattern> QueryPreProcessor::getPatternVector(void);

  /**
  * A getter method for the With vector variables.
  * @return a With object Queue containing all variables within the With clause.
  */
  std::vector<With> QueryPreProcessor::getWithVector(void);

  /**
  * A getter method for the Grammar vector variables.
  * @return a Grammar object Vector containing all variables declared within the declaration.
  */
  std::vector<Grammar> QueryPreProcessor::getGrammarVector(void);

  /**
  * A getter method for the Synonym Unordered Map variables.
  * @return an unordered map containing 2 values(string synonym, int number of times it appears) declared within query statement.
  */
  std::unordered_map<std::string, int> QueryPreProcessor::getSynonym(void);

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