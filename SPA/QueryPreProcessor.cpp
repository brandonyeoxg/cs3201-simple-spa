#include "QueryPreProcessor.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

QueryPreProcessor::QueryPreProcessor()
{
}


QueryPreProcessor::~QueryPreProcessor()
{
}

vector<string> QueryPreProcessor::tokenizeInput(string t_Input) {
  vector<string> results;
  string buf;
 
  string delimiter = "; Select";
  string declaration = t_Input.substr(0, t_Input.find(delimiter) + 1);  //.find(delimiter) finds starting position of the delimiter, hence need to + 1
  string query = t_Input.substr(t_Input.find(delimiter) + 2, t_Input.size()); //same for this as delimiter is "; Select"

  stringstream ss(declaration);

  return results;
}