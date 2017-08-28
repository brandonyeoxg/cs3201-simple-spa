#pragma once
#ifndef QUERYPREPROCESSOR_H
#define QUERYPREPROCESSOR_H

class QueryPreProcessor
{
public:
  QueryPreProcessor();
  ~QueryPreProcessor();
  vector<string> tokenizeInput(string t_Input);
};

#endif QUERYPREPROCESSOR_H
