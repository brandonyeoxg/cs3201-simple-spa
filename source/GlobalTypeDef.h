#pragma once

#include <vector>
#include <unordered_map>

typedef int VAR_INDEX;
typedef int PROC_INDEX;
typedef int STMT_NUM;
typedef std::string VAR_NAME;
typedef int CONSTANT;
typedef std::vector<int> LIST_OF_STMT_NUMS;
typedef std::vector<std::string> LIST_OF_VAR_NAMES;
typedef std::vector<std::string> LIST_OF_RESULTS;
typedef std::unordered_map<std::string, std::vector<std::string>> SET_OF_RESULTS;

/*Called in QueryPreProcessor.h, by aaron*/
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

/*Called in Parser.h, by brandon*/
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

namespace queryType {
  /**
  * Represents a clauseType.
  * An enum type representing the clauseType.
  * Called in QueryEvaluator.h, by verbena.
  */
  enum clauseType {
    SELECT, /**< enum value SELECT. */
    RELATION, /**< enum value RELATION. */
    PATTERN, /**< enum value PATTERN. */
    WITH /**< enum value WITH. */
  };
}