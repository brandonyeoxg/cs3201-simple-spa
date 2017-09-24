#pragma once
using namespace std;

typedef int VAR_INDEX;
typedef int PROC_INDEX;
typedef int STMT_NUM;
typedef std::string VAR_NAME;
typedef int CONSTANT;

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
