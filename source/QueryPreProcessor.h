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

class QueryPreProcessor
{
public:
  QueryPreProcessor();
  ~QueryPreProcessor();
  std::vector<std::string> tokenizeDeclaration(std::string);
  std::vector<std::string> tokenizeQuery(std::string);
  std::string splitString(std::string);
  void QueryPreProcessor::printVector(std::vector<std::string>);
  std::queue<std::string> QueryPreProcessor::getSelect(void);
  std::queue<std::string> QueryPreProcessor::getSuchThat(void);
  std::queue<std::string> QueryPreProcessor::getPattern(void);

private:
  std::queue<std::string> m_selectQueue;
  std::queue<std::string> m_suchThatQueue;
  std::queue<std::string> m_patternQueue;
};

#endif QUERYPREPROCESSOR_H
