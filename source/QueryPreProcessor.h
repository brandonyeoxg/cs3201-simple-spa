#pragma once
#ifndef QUERYPREPROCESSOR_H
#define QUERYPREPROCESSOR_H

#include <vector>

class QueryPreProcessor
{
public:
  QueryPreProcessor();
  ~QueryPreProcessor();
  std::vector<std::string> tokenizeInput(std::string t_Input);
};

#endif QUERYPREPROCESSOR_H
