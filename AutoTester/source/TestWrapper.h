#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "Parser.h"
#include "QueryProcessor.h"
#include "DesignExtractor.h"
#include "pkb/PKB.h"

class TestWrapper : public AbstractWrapper {
public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);

private: 
  PKB *m_pkb;
  Parser* m_parser;
  QueryProcessor *m_qProcessor;
  DesignExtractor *m_designExtractor;
};

#endif
