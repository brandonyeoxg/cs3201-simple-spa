#pragma once
using namespace std;

typedef int VAR_INDEX;
typedef int PROC_INDEX;
typedef int STMT_NUM;
typedef std::string VAR_NAME;
typedef int CONSTANT;

const int m_procedure = 0;
const int m_statementList = 1;
const int m_statement = 2;
const int m_assign = 3;
const int m_while = 4;
const int m_if = 5;
const int m_call = 6;
const int m_variable = 7;
const int m_constant = 8;
const int m_progline = 9;
const int m_statementNumber = 10;
const int m_string = 11;

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

/*Called in ProcTable.h, by brandon*/
const PROC_INDEX INVALID_PROC_NO = -1;
