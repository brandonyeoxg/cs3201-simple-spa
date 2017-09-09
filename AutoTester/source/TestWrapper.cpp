#include "TestWrapper.h"
#include "SyntaxErrorException.h"
// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  m_pkb = new PKB();
  m_parser = new Parser(m_pkb);
  m_qProcessor = new QueryProcessor(m_pkb);
  m_designExtractor = new DesignExtractor(m_pkb);
}

TestWrapper::~TestWrapper() {
  delete m_designExtractor;
  delete m_parser;
  delete m_qProcessor;
  delete m_pkb;
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // ...rest of your code...
  try {
    m_parser->parse(filename);
  } catch (SyntaxErrorException see) {
    std::cout << see.what() << "\n";
  }
  DesignExtractor* designExtractor = new DesignExtractor(m_pkb);
  designExtractor->extractRestOfDesignAbstractions();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  
   results = m_qProcessor->runQueryProcessor(query);
}
