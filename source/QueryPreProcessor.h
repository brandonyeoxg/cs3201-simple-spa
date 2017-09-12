/**
* Query preprocessing class. This class creates Grammar Objects and Design Abstraction Objects
* from the user input.
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
#include "DesignAbstraction.h"
#include "Pattern.h"
#include "PKB.h"
#include "StringUtil.h"

class QueryPreProcessor
{
public:
  QueryPreProcessor();
  ~QueryPreProcessor();
  bool tokenizeDeclaration(std::string);
  bool tokenizeQuery(std::string);
  void removeCharsFromString(std::string & str, char * charsToRemove);
  std::string splitStringDeclaration(std::string);
  std::string splitStringQuery(std::string);
  void QueryPreProcessor::printVector(std::vector<std::string>);
  std::queue<Grammar> QueryPreProcessor::getSelect(void);
  std::queue<DesignAbstraction> QueryPreProcessor::getSuchThat(void);
  std::queue<Pattern> QueryPreProcessor::getPattern(void);
  std::vector<Grammar> QueryPreProcessor::getGrammarVector(void);
  std::unordered_map<std::string, int> QueryPreProcessor::getSynonym(void);
private:
  std::queue<Grammar> m_selectQueue;
  std::queue<DesignAbstraction> m_suchThatQueue;
  std::queue<Pattern> m_patternQueue;
  std::vector<Grammar> m_grammarVector;
  std::unordered_map<std::string, int> m_synonymMap;
  std::vector<DesignAbstraction> m_designAbstractionVector;
  StringUtil m_stringUtil;
};

#endif QUERYPREPROCESSOR_H