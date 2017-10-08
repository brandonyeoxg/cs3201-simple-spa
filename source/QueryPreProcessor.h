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
#include "Grammar.h"
#include "Relation.h"
#include "Pattern.h"
#include "PKB.h"
#include "StringUtil.h"
#include "GlobalTypeDef.h"

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
  bool tokenizeDeclaration(std::string t_declarationInput);

  /**
  * Method takes in the query parsed string and generates the Grammar objects for 
  * Select Queue, Relation Queue and the Pattern Objects for the Pattern Queue.
  * Also generates a synonym unordered map for QueryEvaluator to use.
  * @param t_queryInput is the input query string.
  * @return true if there is no errors. else returns false.
  */
  bool tokenizeQuery(std::string t_queryInput);

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
  * Method takes in a query string and checks if there are more than one instance of another string.
  * @param t_query is the input query string. toFind is the string that is to be found.
  * @return true if only one of the instance is found. else false
  */
  bool QueryPreProcessor::isContainsOne(std::string toFind, std::string t_query);

//--------------------------------Getter Methods------------------------------------------
  /**
  * A getter method for the Select Queue variables.
  * @return a Grammar object Queue containing all variables within the Select clause.
  */
  std::queue<Grammar> QueryPreProcessor::getSelect(void);

  /**
  * A getter method for the Such That Queue variables.
  * @return a Relation object Queue containing all variables within the Such That clause.
  */
  std::queue<Relation> QueryPreProcessor::getSuchThat(void);

  /**
  * A getter method for the Pattern Queue variables.
  * @return a Pattern object Queue containing all variables within the Pattern clause.
  */
  std::queue<Pattern> QueryPreProcessor::getPattern(void);

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

  /**
  * A tokenizing method which removes the characters specified in the string character 
  * in the targeted string targetString and then pushes them into the vector
  * @return a string vector containing the string tokens
  */
  std::vector<std::string> QueryPreProcessor::stringVectorTokenizer(char* charsToRemove, std::string targetString, std::vector<std::string> vector);

  /**
  * DEBUGGING METHODS: 
  * Check if with clause appears
  */
  std::string QueryPreProcessor::getWithStatement(void);

private:
  std::queue<Grammar> m_selectQueue;
  std::queue<Relation> m_suchThatQueue;
  std::queue<Pattern> m_patternQueue;
  std::vector<Grammar> m_grammarVector;
  std::unordered_map<std::string, int> m_synonymMap;
  std::vector<Relation> m_RelationVector;
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
  static std::string BOOLEAN;
  static std::string CALL;
};

#endif QUERYPREPROCESSOR_H