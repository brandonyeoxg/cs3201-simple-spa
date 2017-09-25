#pragma once
using namespace std;
#include <vector>

typedef int VAR_INDEX;
typedef int PROC_INDEX;
typedef int STMT_NUM;
typedef std::string VAR_NAME;
typedef int CONSTANT;
typedef std::vector<int> LIST_OF_STMT_NUMS;
typedef std::vector<std::string> LIST_OF_VAR_NAMES;

const int INVALID_INDEX = -1;

/*Called in parser.h, by brandon*/
namespace tokentype {
  enum tokenType {
    PROC_NAME,
    VAR_NAME,
    CONSTANT,
    EXPR,
  };
};