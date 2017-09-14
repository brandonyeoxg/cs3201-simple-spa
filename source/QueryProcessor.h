#pragma once
#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H
#include "QueryPreProcessor.h"
#include "QueryEvaluator.h"
#include "QueryResultProjector.h"
#include <iostream>
#include <string>

class QueryProcessor {
public:
  QueryProcessor(PKB *t_pkb)
    : m_pkb(t_pkb) {};

  ~QueryProcessor();

  std::string QueryProcessor::runQueryProcessor(void);

private:
  PKB *m_pkb;
};

#endif QUERYPROCESSOR_H