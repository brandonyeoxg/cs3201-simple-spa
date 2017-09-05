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
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // ...rest of your code...
  Parser *parser = new Parser(m_pkb);
  try {
    parser->parse(filename);
    delete parser;
    parser = nullptr;
  } catch (SyntaxErrorException see) {
    std::cout << see.what() << "\n";
  }
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  
   QueryProcessor *q1 = new QueryProcessor(m_pkb);
   std::string resultString = q1->runQueryProcessor();
   std::cout << "queryRead" << "\n";
   results.push_back("");
}
